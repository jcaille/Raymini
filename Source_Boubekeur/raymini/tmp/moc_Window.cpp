/****************************************************************************
** Meta object code from reading C++ file 'Window.h'
**
** Created: Wed 16. May 12:14:25 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Window.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Window[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x0a,
      25,    7,    7,    7, 0x0a,
      38,    7,    7,    7, 0x0a,
      53,    7,    7,    7, 0x0a,
      69,    7,    7,    7, 0x0a,
      86,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Window[] = {
    "Window\0\0renderRayImage()\0setBGColor()\0"
    "showRayImage()\0exportGLImage()\0"
    "exportRayImage()\0about()\0"
};

const QMetaObject Window::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Window,
      qt_meta_data_Window, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Window::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Window::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Window::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Window))
        return static_cast<void*>(const_cast< Window*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Window::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: renderRayImage(); break;
        case 1: setBGColor(); break;
        case 2: showRayImage(); break;
        case 3: exportGLImage(); break;
        case 4: exportRayImage(); break;
        case 5: about(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
