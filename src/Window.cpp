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
#pragma clang diagnostic pop

#ifdef _OPENMP
#include <omp.h>
#endif

#include "GridAARayIterator.h"

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
    
    QDockWidget * controlDockWidget = new QDockWidget (this);
    initControlWidget ();
    
    controlDockWidget->setWidget (controlWidget);
    controlDockWidget->adjustSize ();
    addDockWidget (Qt::RightDockWidgetArea, controlDockWidget);
    controlDockWidget->setFloating(true);
    controlDockWidget->setFeatures (QDockWidget::AllDockWidgetFeatures);
    controlDockWidget->setGeometry(frameGeometry().x() - 200, frameGeometry().y() - 28, 0, 0);
    controlDockWidget->adjustSize();
    
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

RayIterator* Window::getIterator()
{

    RayIterator *r = NULL;
    
    switch (rayIteratorComboBox->currentIndex()) {

    case 1:
        r = new GridAARayIterator();
        ((GridAARayIterator*)r)->gridSize = 2;
        break;
    case 2:
        r = new GridAARayIterator();
        ((GridAARayIterator*)r)->gridSize = 3;
        break;
    default:
        r = new RayIterator();
    }
    
    return r;
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
    
    // Render the image
    RayTracer * rayTracer = RayTracer::getInstance ();
    rayTracer->rayIterator = getIterator();
    
    rayTracer->enableCastShadows = shadowCheckBox->isChecked();
    rayTracer->enableMirrorEffet = mirrorCheckBox->isChecked();
    rayTracer->shadingFunction = getShadingFunction();
    rayTracer->bounces = (int)bouncesSlider->getValue();
    
    QTime timer;
    timer.start ();
    int lastTime = timer.elapsed();
    //Time between refreshing image
    int refreshTime = 500;//  ms
    bool over = false;
    QImage image(screenWidth, screenHeight, QImage::Format_RGB888);

#ifdef _OPENMP
    
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
# else
    rayTracer->render(camPos, viewDirection, upVector, rightVector, fieldOfView, aspectRatio, screenWidth, screenHeight, image);
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
        RayTracer::getInstance ()->setBackgroundColor (Vec3Df (c.red (), c.green (), c.blue ()));
        viewer->setBackgroundColor (c);
        viewer->updateGL ();
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

void Window::about () {
    QMessageBox::about (this,
                        "About This Program",
                        "<b>RayMini</b> <br> by <i>Jean Caille, Florian Denis, Audrey Fourneret & Simon Martin</i>.");
}

void Window::initControlWidget () {
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
    
    
    QGroupBox * rayGroupBox = new QGroupBox ("Ray Tracing", controlWidget);
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

    bouncesSlider = new DoubleWidget(QString("Ray bounces number"), 0.0, 1000.0, 0, this);
    rayLayout->addWidget(bouncesSlider);
    
    mirrorCheckBox = new QCheckBox;
    mirrorCheckBox->setText(QString("Mirror"));
    mirrorCheckBox->setChecked(true);
    rayLayout->addWidget(mirrorCheckBox);
    
    QPushButton * rayButton = new QPushButton ("Render", rayGroupBox);
    rayLayout->addWidget (rayButton);
    connect (rayButton, SIGNAL (clicked ()), this, SLOT (renderRayImage ()));
    QPushButton * showButton = new QPushButton ("Show", rayGroupBox);
    rayLayout->addWidget (showButton);
    connect (showButton, SIGNAL (clicked ()), this, SLOT (showRayImage ()));
    QPushButton * saveButton  = new QPushButton ("Save", rayGroupBox);
    connect (saveButton, SIGNAL (clicked ()) , this, SLOT (exportRayImage ()));
    rayLayout->addWidget (saveButton);

    layout->addWidget (rayGroupBox);
    
    QGroupBox * globalGroupBox = new QGroupBox ("Global Settings", controlWidget);
    QVBoxLayout * globalLayout = new QVBoxLayout (globalGroupBox);
    
    QPushButton * bgColorButton  = new QPushButton ("Background Color", globalGroupBox);
    connect (bgColorButton, SIGNAL (clicked()) , this, SLOT (setBGColor()));
    globalLayout->addWidget (bgColorButton);
    
    QPushButton * aboutButton  = new QPushButton ("About", globalGroupBox);
    connect (aboutButton, SIGNAL (clicked()) , this, SLOT (about()));
    globalLayout->addWidget (aboutButton);
    
    QPushButton * quitButton  = new QPushButton ("Quit", globalGroupBox);
    connect (quitButton, SIGNAL (clicked()) , qApp, SLOT (closeAllWindows()));
    globalLayout->addWidget (quitButton);

    layout->addWidget (globalGroupBox);

    layout->addStretch (0);
}
