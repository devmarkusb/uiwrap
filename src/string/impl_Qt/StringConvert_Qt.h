// Markus Borris, 2015
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef STRINGCONVERT_QT_H_isadfnyiwungsfuzwigqf3
#define STRINGCONVERT_QT_H_isadfnyiwungsfuzwigqf3

#include <QString>
#include "Toolib/types.h"


namespace uiw
{
namespace implQt
{

//! QString to std::string
inline std::string qs2toos(QString s)
{
    too::static_assert_string_BitsPerChar_and_Encoding<std::string, 8>();
    return std::string(s.toUtf8().constData());
}

//! std::string to QString
inline QString toos2qs(std::string s)
{
    too::static_assert_string_BitsPerChar_and_Encoding<std::string, 8>();
    return QString::fromUtf8(s.c_str());
}
}
}

#endif // STRINGCONVERT_QT_H
