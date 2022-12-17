// 2016-17

/** \file
    *Important*
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

#ifndef QMLEXTLIB_H_eruigx4zgf783427g578sth378g
#define QMLEXTLIB_H_eruigx4zgf783427g578sth378g
#include "uiwrap_build_config.h"
#ifdef UIW_LINKLIB_IMPL_CHOICE_QT

#include "uiwrap/filesys/filesys.h"
#include "uiwrap/string/impl_Qt/stringconvert_Qt.h"
#include "uiwrap/string/impl_Qt/urlstring_util_Qt.h"
#include "ul/ul.h"
UL_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC
#include <QDir>
#include <QObject>
#include <QString>
#include <utility>
UL_PRAGMA_WARNINGS_POP


namespace mb::uiw::implQt
{
class DynamicTranslator
{
public:
    virtual ~DynamicTranslator() = default;

    virtual void updateTranslations() const = 0;
};

//! The QML extension lib
class QmlExtLib
    : public QObject
    , public DynamicTranslator
{
    Q_OBJECT

    //! Cf. qml for doc.
    Q_PROPERTY(QString dynTr MEMBER dynTr NOTIFY dynTrChanged FINAL)

public:
    //! Cf. the forwarded function in the implementation for documentation.
    Q_INVOKABLE [[nodiscard]] static QString prependFile_urlSchemePrefix(const QString& file_without_prefix)
    {
        return uiw::implQt::prependFile_urlSchemePrefix(file_without_prefix);
    }

    //! Cf. the forwarded function in the implementation for documentation.
    Q_INVOKABLE [[nodiscard]] static QString removeFile_urlSchemePrefix(const QString& file_with_prefix)
    {
        return uiw::implQt::removeFile_urlSchemePrefix(file_with_prefix);
    }

    //! For Windows returns path with backlashes instead of slashes.
    Q_INVOKABLE [[nodiscard]] static QString toNativeSeparators(const QString& file_with_mixed_separators)
    {
        return QDir::toNativeSeparators(file_with_mixed_separators);
    }

    //! Always returns path with slashes.
    Q_INVOKABLE [[nodiscard]] static QString fromNativeSeparators(const QString& file_with_mixed_separators)
    {
        return QDir::fromNativeSeparators(file_with_mixed_separators);
    }

    //! Expects fullpath with slashes only.
    Q_INVOKABLE [[nodiscard]] static bool fileExists(QString fullpath)
    {
        const auto fs = uiw::file::IFileSys::getInstance();
        return fs->fileExists(uiw::implQt::qs2s(std::move(fullpath)));
    }

    //! Expects fullpath with slashes only.
    Q_INVOKABLE [[nodiscard]] static bool isFile(QString fullpath)
    {
        const auto fs = uiw::file::IFileSys::getInstance();
        return fs->isFile(uiw::implQt::qs2s(std::move(fullpath)));
    }

    //! Call this after installing a new translator. Cf. .qml file dynTr for remaining part of doc.
    void updateTranslations() const override
    {
        emit dynTrChanged();
    }

signals:
    void dynTrChanged() const;

private:
    QString dynTr;
};
} // namespace mb::uiw

#endif // UIW_LINKLIB_IMPL_CHOICE_QT
#endif
