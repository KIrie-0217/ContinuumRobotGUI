/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[27];
    char stringdata0[269];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 5), // "start"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 4), // "stop"
QT_MOC_LITERAL(4, 23, 8), // "periodic"
QT_MOC_LITERAL(5, 32, 19), // "execParameterDialog"
QT_MOC_LITERAL(6, 52, 19), // "execReferenceDialog"
QT_MOC_LITERAL(7, 72, 15), // "applyReferences"
QT_MOC_LITERAL(8, 88, 11), // "sendCommand"
QT_MOC_LITERAL(9, 100, 2), // "id"
QT_MOC_LITERAL(10, 103, 8), // "send_ref"
QT_MOC_LITERAL(11, 112, 3), // "cmd"
QT_MOC_LITERAL(12, 116, 8), // "reload_q"
QT_MOC_LITERAL(13, 125, 14), // "reload_radians"
QT_MOC_LITERAL(14, 140, 22), // "operateCartesianMotion"
QT_MOC_LITERAL(15, 163, 7), // "trigger"
QT_MOC_LITERAL(16, 171, 9), // "direction"
QT_MOC_LITERAL(17, 181, 11), // "eventFilter"
QT_MOC_LITERAL(18, 193, 3), // "obj"
QT_MOC_LITERAL(19, 197, 7), // "QEvent*"
QT_MOC_LITERAL(20, 205, 5), // "event"
QT_MOC_LITERAL(21, 211, 13), // "write_logfile"
QT_MOC_LITERAL(22, 225, 4), // "data"
QT_MOC_LITERAL(23, 230, 6), // "size_t"
QT_MOC_LITERAL(24, 237, 4), // "size"
QT_MOC_LITERAL(25, 242, 12), // "open_logfile"
QT_MOC_LITERAL(26, 255, 13) // "close_logfile"

    },
    "MainWindow\0start\0\0stop\0periodic\0"
    "execParameterDialog\0execReferenceDialog\0"
    "applyReferences\0sendCommand\0id\0send_ref\0"
    "cmd\0reload_q\0reload_radians\0"
    "operateCartesianMotion\0trigger\0direction\0"
    "eventFilter\0obj\0QEvent*\0event\0"
    "write_logfile\0data\0size_t\0size\0"
    "open_logfile\0close_logfile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x08 /* Private */,
       3,    0,   90,    2, 0x08 /* Private */,
       4,    0,   91,    2, 0x08 /* Private */,
       5,    0,   92,    2, 0x08 /* Private */,
       6,    0,   93,    2, 0x08 /* Private */,
       7,    0,   94,    2, 0x08 /* Private */,
       8,    1,   95,    2, 0x08 /* Private */,
      10,    1,   98,    2, 0x08 /* Private */,
      12,    0,  101,    2, 0x08 /* Private */,
      13,    0,  102,    2, 0x08 /* Private */,
      14,    3,  103,    2, 0x08 /* Private */,
      17,    2,  110,    2, 0x08 /* Private */,
      21,    2,  115,    2, 0x08 /* Private */,
      25,    0,  120,    2, 0x08 /* Private */,
      26,    0,  121,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    9,   15,   16,
    QMetaType::Bool, QMetaType::QObjectStar, 0x80000000 | 19,   18,   20,
    QMetaType::Bool, QMetaType::VoidStar, 0x80000000 | 23,   22,   24,
    QMetaType::Int,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { bool _r = _t->start();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->stop(); break;
        case 2: _t->periodic(); break;
        case 3: _t->execParameterDialog(); break;
        case 4: _t->execReferenceDialog(); break;
        case 5: _t->applyReferences(); break;
        case 6: _t->sendCommand((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->send_ref((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->reload_q(); break;
        case 9: _t->reload_radians(); break;
        case 10: _t->operateCartesianMotion((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 11: { bool _r = _t->eventFilter((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< QEvent*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 12: { bool _r = _t->write_logfile((*reinterpret_cast< void*(*)>(_a[1])),(*reinterpret_cast< size_t(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 13: { int _r = _t->open_logfile();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 14: _t->close_logfile(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &KQt::MainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "KQt::LogWriter"))
        return static_cast< KQt::LogWriter*>(this);
    return KQt::MainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KQt::MainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
