/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[16];
    char stringdata0[248];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 27), // "sigShowTaskForTaskNumParent"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 40), // "pmap<pstring,plist<TASK_STATU..."
QT_MOC_LITERAL(4, 81, 8), // "mapTasks"
QT_MOC_LITERAL(5, 90, 11), // "sigShowTask"
QT_MOC_LITERAL(6, 102, 25), // "plist<TASK_STATUS_STRUCT>"
QT_MOC_LITERAL(7, 128, 13), // "sigMessageBox"
QT_MOC_LITERAL(8, 142, 7), // "pstring"
QT_MOC_LITERAL(9, 150, 4), // "info"
QT_MOC_LITERAL(10, 155, 21), // "slotClickTreeLeftItem"
QT_MOC_LITERAL(11, 177, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(12, 194, 4), // "item"
QT_MOC_LITERAL(13, 199, 6), // "column"
QT_MOC_LITERAL(14, 206, 28), // "slotShowTaskForTaskNumParent"
QT_MOC_LITERAL(15, 235, 12) // "slotShowTask"

    },
    "MainWindow\0sigShowTaskForTaskNumParent\0"
    "\0pmap<pstring,plist<TASK_STATUS_STRUCT> >\0"
    "mapTasks\0sigShowTask\0plist<TASK_STATUS_STRUCT>\0"
    "sigMessageBox\0pstring\0info\0"
    "slotClickTreeLeftItem\0QTreeWidgetItem*\0"
    "item\0column\0slotShowTaskForTaskNumParent\0"
    "slotShowTask"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       5,    1,   47,    2, 0x06 /* Public */,
       7,    1,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    2,   53,    2, 0x08 /* Private */,
      14,    1,   58,    2, 0x08 /* Private */,
      15,    1,   61,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    4,
    QMetaType::Void, 0x80000000 | 8,    9,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 11, QMetaType::Int,   12,   13,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    4,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigShowTaskForTaskNumParent((*reinterpret_cast< pmap<pstring,plist<TASK_STATUS_STRUCT> >(*)>(_a[1]))); break;
        case 1: _t->sigShowTask((*reinterpret_cast< plist<TASK_STATUS_STRUCT>(*)>(_a[1]))); break;
        case 2: _t->sigMessageBox((*reinterpret_cast< pstring(*)>(_a[1]))); break;
        case 3: _t->slotClickTreeLeftItem((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->slotShowTaskForTaskNumParent((*reinterpret_cast< pmap<pstring,plist<TASK_STATUS_STRUCT> >(*)>(_a[1]))); break;
        case 5: _t->slotShowTask((*reinterpret_cast< plist<TASK_STATUS_STRUCT>(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainWindow::*_t)(pmap<pstring,plist<TASK_STATUS_STRUCT>> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::sigShowTaskForTaskNumParent)) {
                *result = 0;
            }
        }
        {
            typedef void (MainWindow::*_t)(plist<TASK_STATUS_STRUCT> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::sigShowTask)) {
                *result = 1;
            }
        }
        {
            typedef void (MainWindow::*_t)(pstring );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::sigMessageBox)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::sigShowTaskForTaskNumParent(pmap<pstring,plist<TASK_STATUS_STRUCT>> _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::sigShowTask(plist<TASK_STATUS_STRUCT> _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MainWindow::sigMessageBox(pstring _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
