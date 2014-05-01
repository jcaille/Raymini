#ifndef WINDOW_H
#define WINDOW_H

#include "GLViewer.h"
#include "RayTracer.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-register"
#pragma clang diagnostic ignored "-Wconversion"
#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include <QCheckBox>
#include <QGroupBox>
#include <QComboBox>
#include <QSlider>
#include <QLCDNumber>
#include <QSpinBox>
#include <QImage>
#include <QLabel>
#pragma clang diagnostic pop


#include <vector>
#include <string>

#include "QTUtils.h"


class RayIterator;

class Window : public QMainWindow {
    Q_OBJECT
public:
    Window();
    virtual ~Window();

    // WTF does this do ? Why is it static ?
    static void showStatusMessage (const QString & msg);
    
    bool getShadowCheckboxState();
    
public slots :
    void renderRayImage ();
    void setBGColor ();
    void showRayImage ();
    void exportGLImage ();
    void exportRayImage ();
    void about ();
    
   // bool getShadowCheckBoxState();
    //bool getMirrorCheckBoxState();
    
private :
    void initControlWidget();
    RayIterator *getIterator();
    void resampleScenesLights();
    ShadingFunction getShadingFunction();

    QActionGroup * actionGroup;
    QGroupBox * controlWidget;
    QString currentDirectory;

    QComboBox* shadingComboBox;
    QComboBox* rayIteratorComboBox;
    QCheckBox* shadowCheckBox;
    DoubleWidget* lightSampleSlider;
    DoubleWidget* bouncesSlider;
    QCheckBox* mirrorCheckBox;
    
    GLViewer* viewer;
};

#endif // WINDOW_H


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
