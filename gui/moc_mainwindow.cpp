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
    QByteArrayData data[25];
    char stringdata0[245];
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
QT_MOC_LITERAL(12, 116, 22), // "operateCartesianMotion"
QT_MOC_LITERAL(13, 139, 7), // "trigger"
QT_MOC_LITERAL(14, 147, 9), // "direction"
QT_MOC_LITERAL(15, 157, 11), // "eventFilter"
QT_MOC_LITERAL(16, 169, 3), // "obj"
QT_MOC_LITERAL(17, 173, 7), // "QEvent*"
QT_MOC_LITERAL(18, 181, 5), // "event"
QT_MOC_LITERAL(19, 187, 13), // "write_logfile"
QT_MOC_LITERAL(20, 201, 4), // "data"
QT_MOC_LITERAL(21, 206, 6), // "size_t"
QT_MOC_LITERAL(22, 213, 4), // "size"
QT_MOC_LITERAL(23, 218, 12), // "open_logfile"
QT_MOC_LITERAL(24, 231, 13) // "close_logfile"

    },
    "MainWindow\0start\0\0stop\0periodic\0"
    "execParameterDialog\0execReferenceDialog\0"
    "applyReferences\0sendCommand\0id\0send_ref\0"
    "cmd\0operateCartesianMotion\0trigger\0"
    "direction\0eventFilter\0obj\0QEvent*\0"
    "event\0write_logfile\0data\0size_t\0size\0"
    "open_logfile\0close_logfile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x08 /* Private */,
       3,    0,   80,    2, 0x08 /* Private */,
       4,    0,   81,    2, 0x08 /* Private */,
       5,    0,   82,    2, 0x08 /* Private */,
       6,    0,   83,    2, 0x08 /* Private */,
       7,    0,   84,    2, 0x08 /* Private */,
       8,    1,   85,    2, 0x08 /* Private */,
      10,    1,   88,    2, 0x08 /* Private */,
      12,    3,   91,    2, 0x08 /* Private */,
      15,    2,   98,    2, 0x08 /* Private */,
      19,    2,  103,    2, 0x08 /* Private */,
      23,    0,  108,    2, 0x08 /* Private */,
      24,    0,  109,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    9,   13,   14,
    QMetaType::Bool, QMetaType::QObjectStar, 0x80000000 | 17,   16,   18,
    QMetaType::Bool, QMetaType::VoidStar, 0x80000000 | 21,   20,   22,
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
        case 8: _t->operateCartesianMotion((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 9: { bool _r = _t->eventFilter((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< QEvent*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 10: { bool _r = _t->write_logfile((*reinterpret_cast< void*(*)>(_a[1])),(*reinterpret_cast< size_t(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 11: { int _r = _t->open_logfile();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 12: _t->close_logfile(); break;
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
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
