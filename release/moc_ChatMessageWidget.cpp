/****************************************************************************
** Meta object code from reading C++ file 'ChatMessageWidget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/widgets/ChatMessageWidget.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ChatMessageWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.2. It"
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
struct qt_meta_tag_ZN17ChatMessageWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto ChatMessageWidget::qt_create_metaobjectdata<qt_meta_tag_ZN17ChatMessageWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ChatMessageWidget",
        "copyRequested",
        "",
        "text",
        "forwardRequested",
        "Message",
        "message",
        "favoriteRequested",
        "deleteRequested",
        "recallRequested",
        "multiSelectRequested"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'copyRequested'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'forwardRequested'
        QtMocHelpers::SignalData<void(const Message &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 6 },
        }}),
        // Signal 'favoriteRequested'
        QtMocHelpers::SignalData<void(const Message &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 6 },
        }}),
        // Signal 'deleteRequested'
        QtMocHelpers::SignalData<void(const Message &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 6 },
        }}),
        // Signal 'recallRequested'
        QtMocHelpers::SignalData<void(const Message &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 6 },
        }}),
        // Signal 'multiSelectRequested'
        QtMocHelpers::SignalData<void(const Message &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 6 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ChatMessageWidget, qt_meta_tag_ZN17ChatMessageWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ChatMessageWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17ChatMessageWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17ChatMessageWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN17ChatMessageWidgetE_t>.metaTypes,
    nullptr
} };

void ChatMessageWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ChatMessageWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->copyRequested((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->forwardRequested((*reinterpret_cast<std::add_pointer_t<Message>>(_a[1]))); break;
        case 2: _t->favoriteRequested((*reinterpret_cast<std::add_pointer_t<Message>>(_a[1]))); break;
        case 3: _t->deleteRequested((*reinterpret_cast<std::add_pointer_t<Message>>(_a[1]))); break;
        case 4: _t->recallRequested((*reinterpret_cast<std::add_pointer_t<Message>>(_a[1]))); break;
        case 5: _t->multiSelectRequested((*reinterpret_cast<std::add_pointer_t<Message>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ChatMessageWidget::*)(const QString & )>(_a, &ChatMessageWidget::copyRequested, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ChatMessageWidget::*)(const Message & )>(_a, &ChatMessageWidget::forwardRequested, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ChatMessageWidget::*)(const Message & )>(_a, &ChatMessageWidget::favoriteRequested, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (ChatMessageWidget::*)(const Message & )>(_a, &ChatMessageWidget::deleteRequested, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (ChatMessageWidget::*)(const Message & )>(_a, &ChatMessageWidget::recallRequested, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (ChatMessageWidget::*)(const Message & )>(_a, &ChatMessageWidget::multiSelectRequested, 5))
            return;
    }
}

const QMetaObject *ChatMessageWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChatMessageWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17ChatMessageWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ChatMessageWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ChatMessageWidget::copyRequested(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void ChatMessageWidget::forwardRequested(const Message & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void ChatMessageWidget::favoriteRequested(const Message & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void ChatMessageWidget::deleteRequested(const Message & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void ChatMessageWidget::recallRequested(const Message & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void ChatMessageWidget::multiSelectRequested(const Message & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}
QT_WARNING_POP
