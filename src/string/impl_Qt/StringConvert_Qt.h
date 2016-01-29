// Markus Borris, 2015-16
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef STRINGCONVERT_QT_H_isadfnyiwungsfuzwigqf3
#define STRINGCONVERT_QT_H_isadfnyiwungsfuzwigqf3

#include <QString>


namespace uiw
{
namespace implQt
{

//! QString to std::string
inline std::string qs2s(QString s)
{
    return std::string(s.toUtf8().constData());
}

//! std::string to QString
inline QString s2qs(std::string s)
{
    return QString::fromUtf8(s.c_str());
}

}
}

#endif
