// Markus Borris, 2016-17
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef URLSTRING_UTIL_QT_H_sduigfxn3r7ngf27823f
#define URLSTRING_UTIL_QT_H_sduigfxn3r7ngf27823f
#include "uiwrap_build_config.h"
#ifdef UIW_LINKLIB_IMPL_CHOICE_QT

#include "toolib/warnings.h"
TOO_PRAGMA_WARNINGS_PUSH_DISABLE_ALL
#include <QString>
#include <QUrl>
TOO_PRAGMA_WARNINGS_POP


namespace uiw
{
namespace implQt
{
//! \param abs_path needs to be an absolut path of a file or dir.
//! This function prepends something like `file:///`. This is called the
//! scheme and can be 'ftp' or 'http' and such, all appended with '://'.
//! Probably the third slash '/' belongs to the path itself for the
//! 'file' case.
//! We don't make any suggestion at this place of whether you will need
//! native separators or not. There are appropriate conversion functions.
inline QString prependFile_urlSchemePrefix(const QString& abs_path)
{
    QUrl u{QUrl::fromLocalFile(abs_path)};
    return u.toString();
}

//! Inverse to prependFile_urlSchemePrefix, this removes the prefixed scheme.
inline QString removeFile_urlSchemePrefix(const QUrl& abs_path_with_scheme_prefix)
{
    return abs_path_with_scheme_prefix.toLocalFile();
}
inline QString removeFile_urlSchemePrefix(const QString& abs_path_with_scheme_prefix)
{
    const QUrl u{abs_path_with_scheme_prefix};
    return removeFile_urlSchemePrefix(u);
}
} // implQt
} // uiw

#endif // UIW_LINKLIB_IMPL_CHOICE_QT
#endif
