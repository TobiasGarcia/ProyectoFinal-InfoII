/****************************************************************************
** Meta object code from reading C++ file 'level.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ProyectoFinal/level/level.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'level.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Level_t {
    QByteArrayData data[17];
    char stringdata0[206];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Level_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Level_t qt_meta_stringdata_Level = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Level"
QT_MOC_LITERAL(1, 6, 12), // "update_index"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 13), // "removed_index"
QT_MOC_LITERAL(4, 34, 21), // "update_level_progress"
QT_MOC_LITERAL(5, 56, 13), // "progress_type"
QT_MOC_LITERAL(6, 70, 15), // "remove_power_up"
QT_MOC_LITERAL(7, 86, 10), // "give_power"
QT_MOC_LITERAL(8, 97, 10), // "power_type"
QT_MOC_LITERAL(9, 108, 12), // "remove_enemy"
QT_MOC_LITERAL(10, 121, 10), // "list_index"
QT_MOC_LITERAL(11, 132, 12), // "finish_delay"
QT_MOC_LITERAL(12, 145, 7), // "defrost"
QT_MOC_LITERAL(13, 153, 9), // "no_health"
QT_MOC_LITERAL(14, 163, 12), // "finish_level"
QT_MOC_LITERAL(15, 176, 16), // "next_instruction"
QT_MOC_LITERAL(16, 193, 12) // "check_action"

    },
    "Level\0update_index\0\0removed_index\0"
    "update_level_progress\0progress_type\0"
    "remove_power_up\0give_power\0power_type\0"
    "remove_enemy\0list_index\0finish_delay\0"
    "defrost\0no_health\0finish_level\0"
    "next_instruction\0check_action"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Level[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       4,    1,   72,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   75,    2, 0x0a /* Public */,
       7,    1,   76,    2, 0x0a /* Public */,
       9,    1,   79,    2, 0x0a /* Public */,
      11,    0,   82,    2, 0x0a /* Public */,
      12,    0,   83,    2, 0x0a /* Public */,
      13,    0,   84,    2, 0x0a /* Public */,
      14,    0,   85,    2, 0x0a /* Public */,
      15,    0,   86,    2, 0x0a /* Public */,
      16,    0,   87,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Short,    3,
    QMetaType::Void, QMetaType::Short,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Short,    8,
    QMetaType::Void, QMetaType::Short,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Level::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Level *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->update_index((*reinterpret_cast< short(*)>(_a[1]))); break;
        case 1: _t->update_level_progress((*reinterpret_cast< short(*)>(_a[1]))); break;
        case 2: _t->remove_power_up(); break;
        case 3: _t->give_power((*reinterpret_cast< short(*)>(_a[1]))); break;
        case 4: _t->remove_enemy((*reinterpret_cast< short(*)>(_a[1]))); break;
        case 5: _t->finish_delay(); break;
        case 6: _t->defrost(); break;
        case 7: _t->no_health(); break;
        case 8: _t->finish_level(); break;
        case 9: _t->next_instruction(); break;
        case 10: _t->check_action(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Level::*)(short );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Level::update_index)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Level::*)(short );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Level::update_level_progress)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Level::staticMetaObject = { {
    &QGraphicsScene::staticMetaObject,
    qt_meta_stringdata_Level.data,
    qt_meta_data_Level,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Level::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Level::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Level.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsScene::qt_metacast(_clname);
}

int Level::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void Level::update_index(short _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Level::update_level_progress(short _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
