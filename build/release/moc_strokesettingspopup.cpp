/****************************************************************************
** Meta object code from reading C++ file 'strokesettingspopup.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Ihm_Trabajo/widgets/strokesettingspopup.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'strokesettingspopup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.1. It"
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
struct qt_meta_tag_ZN19StrokeSettingsPopupE_t {};
} // unnamed namespace

template <> constexpr inline auto StrokeSettingsPopup::qt_create_metaobjectdata<qt_meta_tag_ZN19StrokeSettingsPopupE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "StrokeSettingsPopup",
        "colorChanged",
        "",
        "QColor",
        "color",
        "strokeWidthChanged",
        "width",
        "fontChanged",
        "QFont",
        "font",
        "toggleProjectionsRequested",
        "onFontStyleChanged",
        "onColorButtonClicked",
        "onWidthSliderChanged",
        "value",
        "onRgbaSliderChanged",
        "onHexEditChanged",
        "onApplyClicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'colorChanged'
        QtMocHelpers::SignalData<void(const QColor &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'strokeWidthChanged'
        QtMocHelpers::SignalData<void(int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Signal 'fontChanged'
        QtMocHelpers::SignalData<void(const QFont &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
        // Signal 'toggleProjectionsRequested'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onFontStyleChanged'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'onColorButtonClicked'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onWidthSliderChanged'
        QtMocHelpers::SlotData<void(int)>(13, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 14 },
        }}),
        // Slot 'onRgbaSliderChanged'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onHexEditChanged'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onApplyClicked'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<StrokeSettingsPopup, qt_meta_tag_ZN19StrokeSettingsPopupE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject StrokeSettingsPopup::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19StrokeSettingsPopupE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19StrokeSettingsPopupE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN19StrokeSettingsPopupE_t>.metaTypes,
    nullptr
} };

void StrokeSettingsPopup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<StrokeSettingsPopup *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->colorChanged((*reinterpret_cast<std::add_pointer_t<QColor>>(_a[1]))); break;
        case 1: _t->strokeWidthChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->fontChanged((*reinterpret_cast<std::add_pointer_t<QFont>>(_a[1]))); break;
        case 3: _t->toggleProjectionsRequested(); break;
        case 4: _t->onFontStyleChanged(); break;
        case 5: _t->onColorButtonClicked(); break;
        case 6: _t->onWidthSliderChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->onRgbaSliderChanged(); break;
        case 8: _t->onHexEditChanged(); break;
        case 9: _t->onApplyClicked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (StrokeSettingsPopup::*)(const QColor & )>(_a, &StrokeSettingsPopup::colorChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (StrokeSettingsPopup::*)(int )>(_a, &StrokeSettingsPopup::strokeWidthChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (StrokeSettingsPopup::*)(const QFont & )>(_a, &StrokeSettingsPopup::fontChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (StrokeSettingsPopup::*)()>(_a, &StrokeSettingsPopup::toggleProjectionsRequested, 3))
            return;
    }
}

const QMetaObject *StrokeSettingsPopup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *StrokeSettingsPopup::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19StrokeSettingsPopupE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int StrokeSettingsPopup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void StrokeSettingsPopup::colorChanged(const QColor & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void StrokeSettingsPopup::strokeWidthChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void StrokeSettingsPopup::fontChanged(const QFont & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void StrokeSettingsPopup::toggleProjectionsRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
