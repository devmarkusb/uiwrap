// 2016-17

//!
/**
 */
//! \file
#ifndef CPPMODEL2QML_H_dfljkghx4782tgh378g1
#define CPPMODEL2QML_H_dfljkghx4782tgh378g1
#include "uiwrap_build_config.h"
#ifdef UIW_LINKLIB_IMPL_CHOICE_QT

#include "ul/ul.h"
UL_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC
#include <QQmlContext>
#include <QString>
UL_PRAGMA_WARNINGS_POP
#include <memory>


namespace mb::uiw
{
namespace implQt
{
//! Usage: pass sth. like qml_engine.rootContext() to \param context, decide for a name by which
//! the property is accessed in qml, pass it to \param qmlModelName, then pass arbitrary constructor
//! arguments for your cpp model.
template <class CppModel, class... ConstrParamTypes>
// CppModel expected to be a proper Q_OBJECT inherited type and ConstrParamTypes its constructor parameters
std::unique_ptr<CppModel> createCppModelAndSetAsQmlProp(
    QQmlContext& context, QString qmlModelName, ConstrParamTypes&&... params)
{
    auto model = ul::make_unique<CppModel>(params...);
    context.setContextProperty(qmlModelName, model.get());
    return model;
}
} // namespace implQt
} // namespace mb::uiw

#endif // UIW_LINKLIB_IMPL_CHOICE_QT
#endif
