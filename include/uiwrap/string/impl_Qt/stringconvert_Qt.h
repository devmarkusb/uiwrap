//! \file

#ifndef STRINGCONVERT_QT_H_isadfnyiwungsfuzwigqf3
#define STRINGCONVERT_QT_H_isadfnyiwungsfuzwigqf3

#include "uiwrap_build_config.h"
#include "../../config.h"

#ifdef UIW_LINKLIB_IMPL_CHOICE_QT
#include "ul/ul.h"
UL_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC
#include "QString"
UL_PRAGMA_WARNINGS_POP

namespace mb::uiw::implQt
{
//! QString to std::string
inline std::string qs2s(const QString& s)
{
    return std::string(s.toUtf8().constData());
}

//! std::string to QString
inline QString s2qs(const std::string& s)
{
    return QString::fromUtf8(s.c_str());
}
} // namespace mb::uiw::implQt

#endif // UIW_LINKLIB_IMPL_CHOICE_QT

UIW_HEADER_END

#endif
