// Markus Borris, 2015-17
// This file is part of my uiwrap library.

//!
/**
 */
//! \file

#include "uiwrap_build_config.h"
#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)
#include "impl_Qt/library_Qt.h"
#elif defined(UIW_LINKLIB_IMPL_CHOICE_WX)
static_assert(false, "not implemented");
#else
#include "impl_/library_.h"
#endif
#include "ul/ul.h"

#include "ul/macros.h"


namespace mb::uiw
{
std::unique_ptr<ILibrary> ILibrary::make(const std::string& FilePathNameWithoutExtension, const std::string& Version)
{
#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)

    std::unique_ptr<ILibrary> p(ul::make_unique<implQt::CLibrary_Qt>(FilePathNameWithoutExtension, Version));
    if (!FilePathNameWithoutExtension.empty())
        p->Load();
    return p;

#elif defined(UIW_LINKLIB_IMPL_CHOICE_WX)

    ul::ignore_arg(FilePathNameWithoutExtension);
    ul::ignore_arg(Version);
    throw ul::not_implemented{"ILibrary::make"};

#else
    std::unique_ptr<ILibrary> p;
#if UL_OS_WINDOWS
    p = ul::make_unique<CLibrary_win>(FilePathNameWithoutExtension, Version);
#elif UL_OS_LINUX
    p = ul::make_unique<CLibrary_linux>(FilePathNameWithoutExtension, Version);
#endif
    if (!FilePathNameWithoutExtension.empty())
        p->Load();
    return p;

#endif
}
} // namespace mb::uiw
