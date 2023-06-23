/****************************************************************************
** Meta object code from reading C++ file 'QGVScene.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../Qt Graphiz Visualizer/QGVCore/QGVScene.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QGVScene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSQGVSceneENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSQGVSceneENDCLASS = QtMocHelpers::stringData(
    "QGVScene",
    "nodeContextMenu",
    "",
    "QGVNode*",
    "node",
    "nodeDoubleClick",
    "edgeContextMenu",
    "QGVEdge*",
    "edge",
    "edgeDoubleClick",
    "subGraphContextMenu",
    "QGVSubGraph*",
    "graph",
    "subGraphDoubleClick",
    "graphContextMenuEvent"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSQGVSceneENDCLASS_t {
    uint offsetsAndSizes[30];
    char stringdata0[9];
    char stringdata1[16];
    char stringdata2[1];
    char stringdata3[9];
    char stringdata4[5];
    char stringdata5[16];
    char stringdata6[16];
    char stringdata7[9];
    char stringdata8[5];
    char stringdata9[16];
    char stringdata10[20];
    char stringdata11[13];
    char stringdata12[6];
    char stringdata13[20];
    char stringdata14[22];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSQGVSceneENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSQGVSceneENDCLASS_t qt_meta_stringdata_CLASSQGVSceneENDCLASS = {
    {
        QT_MOC_LITERAL(0, 8),  // "QGVScene"
        QT_MOC_LITERAL(9, 15),  // "nodeContextMenu"
        QT_MOC_LITERAL(25, 0),  // ""
        QT_MOC_LITERAL(26, 8),  // "QGVNode*"
        QT_MOC_LITERAL(35, 4),  // "node"
        QT_MOC_LITERAL(40, 15),  // "nodeDoubleClick"
        QT_MOC_LITERAL(56, 15),  // "edgeContextMenu"
        QT_MOC_LITERAL(72, 8),  // "QGVEdge*"
        QT_MOC_LITERAL(81, 4),  // "edge"
        QT_MOC_LITERAL(86, 15),  // "edgeDoubleClick"
        QT_MOC_LITERAL(102, 19),  // "subGraphContextMenu"
        QT_MOC_LITERAL(122, 12),  // "QGVSubGraph*"
        QT_MOC_LITERAL(135, 5),  // "graph"
        QT_MOC_LITERAL(141, 19),  // "subGraphDoubleClick"
        QT_MOC_LITERAL(161, 21)   // "graphContextMenuEvent"
    },
    "QGVScene",
    "nodeContextMenu",
    "",
    "QGVNode*",
    "node",
    "nodeDoubleClick",
    "edgeContextMenu",
    "QGVEdge*",
    "edge",
    "edgeDoubleClick",
    "subGraphContextMenu",
    "QGVSubGraph*",
    "graph",
    "subGraphDoubleClick",
    "graphContextMenuEvent"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSQGVSceneENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   56,    2, 0x06,    1 /* Public */,
       5,    1,   59,    2, 0x06,    3 /* Public */,
       6,    1,   62,    2, 0x06,    5 /* Public */,
       9,    1,   65,    2, 0x06,    7 /* Public */,
      10,    1,   68,    2, 0x06,    9 /* Public */,
      13,    1,   71,    2, 0x06,   11 /* Public */,
      14,    0,   74,    2, 0x06,   13 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject QGVScene::staticMetaObject = { {
    QMetaObject::SuperData::link<QGraphicsScene::staticMetaObject>(),
    qt_meta_stringdata_CLASSQGVSceneENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSQGVSceneENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSQGVSceneENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<QGVScene, std::true_type>,
        // method 'nodeContextMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QGVNode *, std::false_type>,
        // method 'nodeDoubleClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QGVNode *, std::false_type>,
        // method 'edgeContextMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QGVEdge *, std::false_type>,
        // method 'edgeDoubleClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QGVEdge *, std::false_type>,
        // method 'subGraphContextMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QGVSubGraph *, std::false_type>,
        // method 'subGraphDoubleClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QGVSubGraph *, std::false_type>,
        // method 'graphContextMenuEvent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void QGVScene::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QGVScene *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->nodeContextMenu((*reinterpret_cast< std::add_pointer_t<QGVNode*>>(_a[1]))); break;
        case 1: _t->nodeDoubleClick((*reinterpret_cast< std::add_pointer_t<QGVNode*>>(_a[1]))); break;
        case 2: _t->edgeContextMenu((*reinterpret_cast< std::add_pointer_t<QGVEdge*>>(_a[1]))); break;
        case 3: _t->edgeDoubleClick((*reinterpret_cast< std::add_pointer_t<QGVEdge*>>(_a[1]))); break;
        case 4: _t->subGraphContextMenu((*reinterpret_cast< std::add_pointer_t<QGVSubGraph*>>(_a[1]))); break;
        case 5: _t->subGraphDoubleClick((*reinterpret_cast< std::add_pointer_t<QGVSubGraph*>>(_a[1]))); break;
        case 6: _t->graphContextMenuEvent(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (QGVScene::*)(QGVNode * );
            if (_t _q_method = &QGVScene::nodeContextMenu; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (QGVScene::*)(QGVNode * );
            if (_t _q_method = &QGVScene::nodeDoubleClick; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (QGVScene::*)(QGVEdge * );
            if (_t _q_method = &QGVScene::edgeContextMenu; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (QGVScene::*)(QGVEdge * );
            if (_t _q_method = &QGVScene::edgeDoubleClick; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (QGVScene::*)(QGVSubGraph * );
            if (_t _q_method = &QGVScene::subGraphContextMenu; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (QGVScene::*)(QGVSubGraph * );
            if (_t _q_method = &QGVScene::subGraphDoubleClick; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (QGVScene::*)();
            if (_t _q_method = &QGVScene::graphContextMenuEvent; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject *QGVScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QGVScene::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSQGVSceneENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsScene::qt_metacast(_clname);
}

int QGVScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void QGVScene::nodeContextMenu(QGVNode * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QGVScene::nodeDoubleClick(QGVNode * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QGVScene::edgeContextMenu(QGVEdge * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QGVScene::edgeDoubleClick(QGVEdge * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QGVScene::subGraphContextMenu(QGVSubGraph * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QGVScene::subGraphDoubleClick(QGVSubGraph * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void QGVScene::graphContextMenuEvent()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
