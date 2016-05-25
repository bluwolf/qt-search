/****************************************************************************
** Meta object code from reading C++ file 'mainwin.h'
**
** Created: Sun May 4 10:24:27 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "headers/mainwin.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWin[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,    9,    8,    8, 0x08,
      43,    9,    8,    8, 0x08,
      68,    8,    8,    8, 0x08,
      80,    8,    8,    8, 0x08,
      95,    8,    8,    8, 0x08,
     106,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWin[] = {
    "MainWin\0\0item\0openResult(QListWidgetItem*)\0"
    "openAd(QListWidgetItem*)\0showAbout()\0"
    "singleSearch()\0orSearch()\0andSearch()\0"
};

void MainWin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWin *_t = static_cast<MainWin *>(_o);
        switch (_id) {
        case 0: _t->openResult((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 1: _t->openAd((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 2: _t->showAbout(); break;
        case 3: _t->singleSearch(); break;
        case 4: _t->orSearch(); break;
        case 5: _t->andSearch(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWin::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWin::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWin,
      qt_meta_data_MainWin, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWin::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWin))
        return static_cast<void*>(const_cast< MainWin*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
