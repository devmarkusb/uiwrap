// Markus Borris, 2016
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef URLSTRING_UTIL_QT_H_sduigfxn3r7ngf27823f
#define URLSTRING_UTIL_QT_H_sduigfxn3r7ngf27823f

#include <QString>


namespace uiw
{
namespace implQt
{
//! \param abs_path needs to be an absolut path of a file or dir.
//! This function prepends something like `file:///`.
//! We don't make any suggestion at this place of whether you will need
//! native separators or not. There are appropriate conversion functions.
inline QString prependFileDir_urlPrefix(const QString& abs_path)
{
    QString ret{abs_path};
    ret.insert(0, QStringLiteral("file:///"));
    return ret;
}
}
}

#endif
