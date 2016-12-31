// Markus Borris, 2015-16
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef STRINGCONVERT_QT_H_isadfnyiwungsfuzwigqf3
#define STRINGCONVERT_QT_H_isadfnyiwungsfuzwigqf3
//#ifdef UIW_LINKLIB_IMPL_CHOICE_QT

#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_BEGIN"
#include <QString>
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_END"


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
inline QString s2qs(const std::string& s)
{
	return QString::fromUtf8(s.c_str());
}
}
}

//#endif //UIW_LINKLIB_IMPL_CHOICE_QT
#endif
