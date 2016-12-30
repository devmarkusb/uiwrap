// Markus Borris, 2016
// This file is part of my uiwrap library.

//!
/** *Important*
    This file implements a model collecting various library functions. The purpose of these functions
    is an extension of QML capabilities.
    In order to use these library functions from QML (which is what they are for), you have to
    take care of two things:
        (1) copy the file qmlextlib.qml, which sits right next to this current file, to your qml/dummydata
            folder in order for the library to be usable as a model even in GUI only mode,
        (2) make the usual call
            qml_engine.rootContext()->setContextProperty("qmlextlib", . );
            at the appropriate place in you cpp code to register an instance of the model implemented in this
            file here,
        (3) the ugliest step: add this file to your projects compilation list, such that it's getting moc'ed.
            So for CMake you might add
            sdks/uiwrap/include/uiwrap/GUI/impl_Qt/qmlextlib.h
            to the Qt specific sources that finally make up your target.

    Equally important impl. note: Of course qmlextlib.qml/h need to be kept in sync always.
    I'm pretty sure, this library approach is not the most beautiful one. But it came to mind easily and
    will hopefully be sufficient for some time and not grow into something increasingly ugly. But it is intended
    to grow, though perhaps not even needed very much to.
*/
//! \file

#ifndef QMLEXTLIB_H_eruigx4zgf783427g578sth378g
#define QMLEXTLIB_H_eruigx4zgf783427g578sth378g
//#ifdef UIW_LINKLIB_IMPL_CHOICE_QT

#include "uiwrap/string/impl_Qt/UrlString_util_Qt.h"
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_BEGIN"
#include <QObject>
#include <QString>
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_END"


namespace uiw
{
namespace implQt
{
//! The QML extension lib
class QmlExtLib : public QObject
{
    Q_OBJECT

public:
    //! Cf. the forwarded function in the implementation for documentation.
    Q_INVOKABLE QString prependFile_urlSchemePrefix(QString file_without_prefix) const
    {
        return ::uiw::implQt::prependFile_urlSchemePrefix(file_without_prefix);
    }

    //! Cf. the forwarded function in the implementation for documentation.
    Q_INVOKABLE QString removeFile_urlSchemePrefix(QString file_with_prefix) const
    {
        return ::uiw::implQt::removeFile_urlSchemePrefix(file_with_prefix);
    }
};
} // implQt
} // uiw

//#endif // UIW_LINKLIB_IMPL_CHOICE_QT
#endif