/****************************************************************************
** Meta object code from reading C++ file 'xtreewidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../xtreewidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'xtreewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_xTreeWidget_t {
    QByteArrayData data[22];
    char stringdata0[265];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_xTreeWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_xTreeWidget_t qt_meta_stringdata_xTreeWidget = {
    {
QT_MOC_LITERAL(0, 0, 11), // "xTreeWidget"
QT_MOC_LITERAL(1, 12, 15), // "sigShowMaxIndex"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 3), // "url"
QT_MOC_LITERAL(4, 33, 14), // "sigClickString"
QT_MOC_LITERAL(5, 48, 4), // "text"
QT_MOC_LITERAL(6, 53, 6), // "column"
QT_MOC_LITERAL(7, 60, 8), // "sigClick"
QT_MOC_LITERAL(8, 69, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(9, 86, 4), // "item"
QT_MOC_LITERAL(10, 91, 14), // "sigDoubleClick"
QT_MOC_LITERAL(11, 106, 44), // "on_stuTableWidget_customConte..."
QT_MOC_LITERAL(12, 151, 3), // "pos"
QT_MOC_LITERAL(13, 155, 12), // "sortByColumn"
QT_MOC_LITERAL(14, 168, 3), // "col"
QT_MOC_LITERAL(15, 172, 7), // "popMenu"
QT_MOC_LITERAL(16, 180, 9), // "itemClick"
QT_MOC_LITERAL(17, 190, 15), // "itemDoubleClick"
QT_MOC_LITERAL(18, 206, 14), // "slotDeleteItem"
QT_MOC_LITERAL(19, 221, 11), // "slotAddItem"
QT_MOC_LITERAL(20, 233, 16), // "slotAddItemAfter"
QT_MOC_LITERAL(21, 250, 14) // "slotItemUpdate"

    },
    "xTreeWidget\0sigShowMaxIndex\0\0url\0"
    "sigClickString\0text\0column\0sigClick\0"
    "QTreeWidgetItem*\0item\0sigDoubleClick\0"
    "on_stuTableWidget_customContextMenuRequested\0"
    "pos\0sortByColumn\0col\0popMenu\0itemClick\0"
    "itemDoubleClick\0slotDeleteItem\0"
    "slotAddItem\0slotAddItemAfter\0"
    "slotItemUpdate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_xTreeWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       4,    2,   82,    2, 0x06 /* Public */,
       7,    2,   87,    2, 0x06 /* Public */,
      10,    2,   92,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    1,   97,    2, 0x08 /* Private */,
      13,    1,  100,    2, 0x08 /* Private */,
      15,    1,  103,    2, 0x08 /* Private */,
      16,    2,  106,    2, 0x08 /* Private */,
      17,    2,  111,    2, 0x08 /* Private */,
      18,    0,  116,    2, 0x08 /* Private */,
      19,    0,  117,    2, 0x08 /* Private */,
      20,    0,  118,    2, 0x08 /* Private */,
      21,    0,  119,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    5,    6,
    QMetaType::Void, 0x80000000 | 8, QMetaType::Int,    9,    6,
    QMetaType::Void, 0x80000000 | 8, QMetaType::Int,    9,    6,

 // slots: parameters
    QMetaType::Void, QMetaType::QPoint,   12,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::QPoint,    2,
    QMetaType::Void, 0x80000000 | 8, QMetaType::Int,    9,    6,
    QMetaType::Void, 0x80000000 | 8, QMetaType::Int,    9,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void xTreeWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        xTreeWidget *_t = static_cast<xTreeWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigShowMaxIndex((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->sigClickString((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->sigClick((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->sigDoubleClick((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->on_stuTableWidget_customContextMenuRequested((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 5: _t->sortByColumn((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->popMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 7: _t->itemClick((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->itemDoubleClick((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->slotDeleteItem(); break;
        case 10: _t->slotAddItem(); break;
        case 11: _t->slotAddItemAfter(); break;
        case 12: _t->slotItemUpdate(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (xTreeWidget::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&xTreeWidget::sigShowMaxIndex)) {
                *result = 0;
            }
        }
        {
            typedef void (xTreeWidget::*_t)(QString , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&xTreeWidget::sigClickString)) {
                *result = 1;
            }
        }
        {
            typedef void (xTreeWidget::*_t)(QTreeWidgetItem * , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&xTreeWidget::sigClick)) {
                *result = 2;
            }
        }
        {
            typedef void (xTreeWidget::*_t)(QTreeWidgetItem * , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&xTreeWidget::sigDoubleClick)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject xTreeWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_xTreeWidget.data,
      qt_meta_data_xTreeWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *xTreeWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *xTreeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_xTreeWidget.stringdata0))
        return static_cast<void*>(const_cast< xTreeWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int xTreeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void xTreeWidget::sigShowMaxIndex(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void xTreeWidget::sigClickString(QString _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void xTreeWidget::sigClick(QTreeWidgetItem * _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void xTreeWidget::sigDoubleClick(QTreeWidgetItem * _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
