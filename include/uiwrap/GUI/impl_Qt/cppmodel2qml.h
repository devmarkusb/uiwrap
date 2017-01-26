#ifndef CPPMODEL2QML_H_dfljkghx4782tgh378g1
#define CPPMODEL2QML_H_dfljkghx4782tgh378g1
//#ifdef UIW_LINKLIB_IMPL_CHOICE_QT

#include "Toolib/std/std_extensions.h"
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_BEGIN"
#include <QQmlContext>
#include <QString>
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_END"
#include <memory>


namespace uiw
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
    auto model = too::make_unique<CppModel>(params...);
    context.setContextProperty(qmlModelName, model.get());
    return model;
}
} // implQt
} // uiw

//#endif // UIW_LINKLIB_IMPL_CHOICE_QT
#endif