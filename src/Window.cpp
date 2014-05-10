#include "Window.h"

#include <vector>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-register"
#pragma clang diagnostic ignored "-Wconversion"
#include <QDockWidget>
#include <QGroupBox>
#include <QButtonGroup>
#include <QMenuBar>
#include <QApplication>
#include <QLayout>
#include <QLabel>
#include <QProgressBar>
#include <QCheckBox>
#include <QRadioButton>
#include <QColorDialog>
#include <QLCDNumber>
#include <QPixmap>
#include <QFrame>
#include <QSplitter>
#include <QMenu>
#include <QScrollArea>
#include <QCoreApplication>
#include <QFont>
#include <QSizePolicy>
#include <QImageReader>
#include <QStatusBar>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QStatusBar>
#include <QDesktopWidget>
#pragma clang diagnostic pop

#include "LensBlurIterator.h"

#ifdef _OPENMP
#include <omp.h>
#endif


#ifdef __APPLE__
#include <dispatch/dispatch.h>
#endif
using namespace std;

Window::Window () : QMainWindow (NULL) {
    try {
        viewer = new GLViewer;
    } catch (GLViewer::Exception e) {
        cerr << e.getMessage () << endl;
        exit (1);
    }
    setCentralWidget (viewer);

#if DEBUG
    cout << "Building in Debug mode" << endl << " Small window to allow easy testing" << endl;;
    viewer->setBaseSize(150, 150);
#else
    cout << "Release" << endl;
    viewer->setBaseSize(500, 500);
#endif
    
    initControlWidget ();
    
    QDockWidget * controlDockWidget = new QDockWidget (this);
    
    controlDockWidget->setWidget (controlWidget);
    controlDockWidget->adjustSize ();
    addDockWidget (Qt::RightDockWidgetArea, controlDockWidget);
    controlDockWidget->setFloating(true);
    controlDockWidget->setFeatures (QDockWidget::AllDockWidgetFeatures);
    controlDockWidget->adjustSize();
    controlDockWidget->move(0, 0);
    
    QDockWidget * focalDockWidget = new QDockWidget (this);
    focalDockWidget->setWidget(focalWidget);
    focalDockWidget->adjustSize ();
    addDockWidget (Qt::LeftDockWidgetArea, focalDockWidget);
    focalDockWidget->setFloating(true);
    focalDockWidget->setFeatures (QDockWidget::AllDockWidgetFeatures);
    focalDockWidget->adjustSize();
    focalDockWidget->move(200, 0);

    QDockWidget* rayTracerDockWidget = new QDockWidget(this);
    rayTracerDockWidget->setWidget(rayTracerWidget);
    rayTracerDockWidget->adjustSize ();
    addDockWidget (Qt::TopDockWidgetArea, rayTracerDockWidget);
    rayTracerDockWidget->setFloating(true);
    rayTracerDockWidget->setFeatures (QDockWidget::AllDockWidgetFeatures);
    rayTracerDockWidget->adjustSize();

    // Move widgets around the raytracer
    
    QDesktopWidget desktop;
    QRect mainScreenSize = desktop.availableGeometry(desktop.primaryScreen());
    
    controlDockWidget->move(mainScreenSize.width() - 200, 0);
    rayTracerDockWidget->move(0, 0);
    focalDockWidget->move(0, mainScreenSize.height() - focalDockWidget->height());
    
    statusBar()->showMessage("");
}

Window::~Window () {

}

#pragma mark - Options methods

// Those methods are public and might be called by outside objects to get user options.

void Window::resampleScenesLights()
{
    Scene* scene = Scene::getInstance();
    scene->resampleLights((float) lightSampleSlider->getValue());
}

void Window::setRayIteratorOptions(RayIterator *r)
{
    LensBlurIterator* lr = (LensBlurIterator*) r;
    lr->setFocalLength(focalLengthSlider->getValue());
    lr->setDensity(lensBlurDensitySlider->getValue());
    
    Mesh apertureMesh;
    
    switch (apertureComboBox->currentIndex()) {
        case 0:
            lr->setFocalLength(-1.0);
            break;
        case 1:
            // Small square

            apertureMesh.makePlane(Vec3Df(0, 0, 0), Vec3Df(0, 0, 1), Vec3Df(1, 0, 0), 0.05, 0.05);
            lr->setGeometry(Geometry(apertureMesh));
            break;
        case 2:
            // Medium square
            apertureMesh.makePlane(Vec3Df(0, 0, 0), Vec3Df(0, 0, 1), Vec3Df(1, 0, 0), 0.15, 0.15);
            lr->setGeometry(Geometry(apertureMesh));
            break;
        case 3:
            // Large square
            apertureMesh.makePlane(Vec3Df(0, 0, 0), Vec3Df(0, 0, 1), Vec3Df(1, 0, 0), 0.3, 0.3);
            lr->setGeometry(Geometry(apertureMesh));
            break;
        default:
            lr->setFocalLength(-1.0);
            break;
    }
    
    switch (focusOnComboBox->currentIndex()) {
        case 0:
            // Using focal length
            break;
        default:
            Scene* s = Scene::getInstance();
            std::vector<std::pair<std::string, Vec3Df> > poi = s->getPOI();
            lr->focusOn(poi[focusOnComboBox->currentIndex()-1].second);
            break;
    }
    
    switch (rayIteratorComboBox->currentIndex()) {
        case 1:
            lr->gridSize = 2;
            break;
        case 2:
            lr->gridSize = 3;
            break;
        default:
            lr->gridSize = 1;
            break;
    }
}

ShadingFunction Window::getShadingFunction()
{
    switch (shadingComboBox->currentIndex()) {
    case 0:
        return CONSTANT;
        break;
    case 1 :
        return PHONG;
        break;
    case 2 :
        return COOK;
        break;
    case 3 :
        return CARTOON;
        break;
    default:
        return CONSTANT;
        break;
    }
}

#pragma mark - Rendering

void Window::renderRayImage () {
    // Modify scene to apply options if necessary
    resampleScenesLights();

    // Get camera informations
    qglviewer::Camera * cam = viewer->camera ();
    qglviewer::Vec p = cam->position ();
    qglviewer::Vec d = cam->viewDirection ();
    qglviewer::Vec u = cam->upVector ();
    qglviewer::Vec r = cam->rightVector ();
    Vec3Df camPos (p[0], p[1], p[2]);
    Vec3Df viewDirection (d[0], d[1], d[2]);
    Vec3Df upVector (u[0], u[1], u[2]);
    Vec3Df rightVector (r[0], r[1], r[2]);
    float fieldOfView = cam->fieldOfView ();
    float aspectRatio = cam->aspectRatio ();
    unsigned int screenWidth = cam->screenWidth ();
    unsigned int screenHeight = cam->screenHeight ();
    
    // Create a ray Iterator
    RayIterator* rayIterator = new LensBlurIterator();
    rayIterator->setCameraInformation(camPos, viewDirection, upVector, rightVector, fieldOfView, aspectRatio, screenWidth, screenHeight);
    setRayIteratorOptions(rayIterator);

    
    // Create the ray tracer
    RayTracer * rayTracer = RayTracer::getInstance ();
    rayTracer->rayIterator = rayIterator;
    
    // Pass options down
    rayTracer->enableCastShadows = shadowCheckBox->isChecked();
    rayTracer->enableRefraction = transparencyCheckBox->isChecked();
    rayTracer->enableMirrorEffet = mirrorCheckBox->isChecked();
    rayTracer->shadingFunction = getShadingFunction();
    rayTracer->bounces = (int)bouncesSlider->getValue();
    
    QTime timer;
    timer.start ();

#ifdef _OPENMP
    QImage image(screenWidth, screenHeight, QImage::Format_RGB888);
    bool over = false;
#pragma omp parallel num_threads(2)
    {
        int i = omp_get_thread_num();

        //First thread is calculating image
        if(i == 0)
           over = rayTracer->render(camPos, viewDirection, upVector, rightVector, fieldOfView, aspectRatio, screenWidth, screenHeight, image);

        //Second thread is getting image every refreshingTime
        if(i == 1)
        {
            while (!over)
            {
                if(timer.elapsed() > lastTime + refreshTime)
                {
                    lastTime = timer.elapsed();
                    viewer->setRayImage(image);
                    viewer->setDisplayMode (GLViewer::RayDisplayMode);
                }
            }
        }
    }
# elif __APPLE__
    QImage image(screenWidth, screenHeight, QImage::Format_RGB888);
    for(int i = 0 ; i < screenWidth ; i++)
    {
        for(int j = 0 ; j < screenHeight  ; j++)
        {
            image.setPixel(i, j, qRgb(0, 0, 0));
        }
    }
    
    __block bool over = false;
    __block QImage& imgRef = image;
    
    dispatch_queue_t rayTracerQueue = dispatch_queue_create("com.raymini.queue", 0);
    
    dispatch_async(rayTracerQueue, ^{
        over = rayTracer->render(camPos, viewDirection, upVector, rightVector, fieldOfView, aspectRatio, screenWidth, screenHeight, imgRef);
    });
    
    while(!over)
    {
        viewer->setDisplayMode (GLViewer::RayDisplayMode);
        viewer->setRayImage(image);
    }
#else
    rayTracer->render(camPos, viewDirection, upVector, rightVector, fieldOfView, aspectRatio, screenWidth, screenHeight, imgRef);
#endif
    
    viewer->setRayImage(image);
    viewer->setDisplayMode (GLViewer::RayDisplayMode);
    statusBar()->showMessage(QString ("Raytracing performed in ") +
                             QString::number (timer.elapsed ()) +
                             QString ("ms at ") +
                             QString::number (screenWidth) + QString ("x") + QString::number (screenHeight) +
                             QString (" screen resolution"));
    viewer->setDisplayMode (GLViewer::RayDisplayMode);
}

void Window::setBGColor () {
    QColor c = QColorDialog::getColor (QColor (133, 152, 181), this);
    if (c.isValid () == true) {
        cout << c.red () << endl;
        RayTracer::getInstance()->setBackgroundColor(1/255.0f * Vec3Df(c.red(), c.green(), c.blue()));
        viewer->setBackgroundColor(c);
        viewer->updateGL();
    }
}

void Window::showRayImage () {
    viewer->setDisplayMode (GLViewer::RayDisplayMode);
}

void Window::exportGLImage () {
    viewer->saveSnapshot (false, false);
}

void Window::exportRayImage () {
    QString filename = QFileDialog::getSaveFileName (this,
                                                     "Save ray-traced image",
                                                     ".",
                                                     "*.jpg *.bmp *.png");
    if (!filename.isNull () && !filename.isEmpty ())
        viewer->getRayImage().save (filename);
}

#pragma mark - Interface

void Window::about () {
    // I can't add Signals and Slot, so i'm highjacking this one to update the scene being displayed

    Scene* s = Scene::getInstance();
    
    AvailableScene selectedScene = (AvailableScene)availableScenesComboBox->currentIndex();
    s->setCurrentScene(selectedScene);
    
    updateFocusOnComboBox();
    viewer->init();
    viewer->draw();
}

void Window::initControlWidget () {
    
// Boubekeur's controls
    
    controlWidget = new QGroupBox ();
    QVBoxLayout * layout = new QVBoxLayout (controlWidget);
    
    QGroupBox * previewGroupBox = new QGroupBox ("Preview", controlWidget);
    QVBoxLayout * previewLayout = new QVBoxLayout (previewGroupBox);
    
    QCheckBox * wireframeCheckBox = new QCheckBox ("Wireframe", previewGroupBox);
    connect (wireframeCheckBox, SIGNAL (toggled (bool)), viewer, SLOT (setWireframe (bool)));
    previewLayout->addWidget (wireframeCheckBox);

    QButtonGroup * modeButtonGroup = new QButtonGroup (previewGroupBox);
    modeButtonGroup->setExclusive (true);
    QRadioButton * flatButton = new QRadioButton ("Flat", previewGroupBox);
    QRadioButton * smoothButton = new QRadioButton ("Smooth", previewGroupBox);
    modeButtonGroup->addButton (flatButton, static_cast<int>(GLViewer::Flat));
    modeButtonGroup->addButton (smoothButton, static_cast<int>(GLViewer::Smooth));
    connect (modeButtonGroup, SIGNAL (buttonClicked (int)), viewer, SLOT (setRenderingMode (int)));
    previewLayout->addWidget (flatButton);
    previewLayout->addWidget (smoothButton);
    
    QPushButton * snapshotButton  = new QPushButton ("Save preview", previewGroupBox);
    connect (snapshotButton, SIGNAL (clicked ()) , this, SLOT (exportGLImage ()));
    previewLayout->addWidget (snapshotButton);

    layout->addWidget (previewGroupBox);
    QGroupBox * globalGroupBox = new QGroupBox ("Global Settings", controlWidget);
    QVBoxLayout * globalLayout = new QVBoxLayout (globalGroupBox);
    
    QPushButton * bgColorButton  = new QPushButton ("Background Color", globalGroupBox);
    connect (bgColorButton, SIGNAL (clicked()) , this, SLOT (setBGColor()));
    globalLayout->addWidget (bgColorButton);
    
    availableScenesComboBox = new QComboBox;
    availableScenesComboBox->addItem("Cornell Box - Animals");
    availableScenesComboBox->addItem("Cornell Box - Sphere");
    availableScenesComboBox->addItem("Chess");
    availableScenesComboBox->addItem("Random Spheres");
    globalLayout->addWidget(availableScenesComboBox);
    
    QPushButton * aboutButton  = new QPushButton ("Update Scene", globalGroupBox);
    connect (aboutButton, SIGNAL (clicked()) , this, SLOT (about()));
    globalLayout->addWidget (aboutButton);
    
    QPushButton * quitButton  = new QPushButton ("Quit", globalGroupBox);
    connect (quitButton, SIGNAL (clicked()) , qApp, SLOT (closeAllWindows()));
    globalLayout->addWidget (quitButton);
    
    layout->addWidget (globalGroupBox);
    
    layout->addStretch (0);

// Ray Tracer controls
    
    rayTracerWidget = new QGroupBox ();
    QVBoxLayout * rayTracerLayout = new QVBoxLayout (rayTracerWidget);

    QGroupBox * rayGroupBox = new QGroupBox ("Ray Tracing", rayTracerWidget);
    QVBoxLayout * rayLayout = new QVBoxLayout (rayGroupBox);
    
    shadingComboBox = new QComboBox;
    shadingComboBox->addItem(tr("Constant - No shading"));
    shadingComboBox->addItem(tr("Phong"));
    shadingComboBox->addItem(tr("Cook Torrance"));
    shadingComboBox->addItem(tr("Cartoon"));
    shadingComboBox->setCurrentIndex(1);
    rayLayout->addWidget(shadingComboBox);
    
    
    rayIteratorComboBox = new QComboBox;
    rayIteratorComboBox->addItem(tr("No AA"));
    rayIteratorComboBox->addItem(tr("AA : 4x"));
    rayIteratorComboBox->addItem(tr("AA : 9x"));
    rayLayout->addWidget(rayIteratorComboBox);
    
    shadowCheckBox = new QCheckBox;
    shadowCheckBox->setText(QString("Cast Shadows"));
    shadowCheckBox->setChecked(true);
    rayLayout->addWidget(shadowCheckBox);
    
    lightSampleSlider = new DoubleWidget(QString("Light samples density"), 0.0, 500.0, 50, this);
    rayLayout->addWidget(lightSampleSlider);

    bouncesSlider = new DoubleWidget(QString("Ray bounces number"), 0.0, 100.0, 0, this);
    rayLayout->addWidget(bouncesSlider);
    
    mirrorCheckBox = new QCheckBox;
    mirrorCheckBox->setText(QString("Mirror"));
    mirrorCheckBox->setChecked(true);
    rayLayout->addWidget(mirrorCheckBox);

    transparencyCheckBox = new QCheckBox;
    transparencyCheckBox->setText(QString("Transparency"));
    transparencyCheckBox->setChecked(true);
    rayLayout->addWidget(transparencyCheckBox);
    
    
    QPushButton * rayButton = new QPushButton ("Render", rayGroupBox);
    rayLayout->addWidget (rayButton);
    connect (rayButton, SIGNAL (clicked ()), this, SLOT (renderRayImage ()));
    QPushButton * showButton = new QPushButton ("Show", rayGroupBox);
    rayLayout->addWidget (showButton);
    connect (showButton, SIGNAL (clicked ()), this, SLOT (showRayImage ()));
    QPushButton * saveButton  = new QPushButton ("Save", rayGroupBox);
    connect (saveButton, SIGNAL (clicked ()) , this, SLOT (exportRayImage ()));
    rayLayout->addWidget (saveButton);

    rayTracerLayout->addWidget (rayGroupBox);
    
 // Focal controls
    focalWidget = new QGroupBox ();
    QVBoxLayout * focalLayout = new QVBoxLayout (focalWidget);

    apertureComboBox = new QComboBox;
    apertureComboBox->addItem(tr("Pinhole camera"));
    apertureComboBox->addItem(tr("Small square aperture"));
    apertureComboBox->addItem(tr("Medium square aperture"));
    apertureComboBox->addItem(tr("Large square aperture"));
    focalLayout->addWidget(apertureComboBox);
    
    QLabel* label = new QLabel("Focus On ...");
    focalLayout->addWidget(label);

    focusOnComboBox = new QComboBox;
    updateFocusOnComboBox();
    focalLayout->addWidget(focusOnComboBox);
    
    focalLengthSlider = new DoubleWidget(QString("Focal Length"), 0.0, 20.0, 10.0, this);
    focalLayout->addWidget(focalLengthSlider);
    
    lensBlurDensitySlider = new DoubleWidget(QString("Lens blur samples density"), 0.0, 10000, 2500);
    focalLayout->addWidget(lensBlurDensitySlider);
}

void Window::updateFocusOnComboBox()
{
    Scene* s = Scene::getInstance();
    
    int n = focusOnComboBox->count();
    for (int i = n; i >= 0; --i) {
        focusOnComboBox->removeItem(i);
    }
    
    focusOnComboBox->addItem(tr("Use focal Length"));
    
    std::vector<std::pair<std::string, Vec3Df>> poi = s->getPOI();
    for (std::pair<std::string, Vec3Df> p : poi) {
        focusOnComboBox->addItem(tr(p.first.c_str()));
        std::cout << p.first.c_str() << std::endl;
    }
    
    update();
    repaint();
}


