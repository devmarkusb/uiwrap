// Markus Borris, 2015
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#include "Toolib/std/std_extensions.h"
#include "Toolib/ignore_arg.h"
#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)
#include "impl_Qt/library_Qt.h"
#elif defined(UIW_LINKLIB_IMPL_CHOICE_WX)
// todo
#else
#include "impl_/library_.h"
#endif


namespace uiw
{
std::unique_ptr<ILibrary> ILibrary::make(const std::string& FilePathNameWithoutExtension, const std::string& Version)
{
#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)

    std::unique_ptr<ILibrary> p(too::make_unique<implQt::CLibrary_Qt>(FilePathNameWithoutExtension, Version));
    if (!FilePathNameWithoutExtension.empty())
        p->Load();
    return p;

#elif defined(UIW_LINKLIB_IMPL_CHOICE_WX)

    // todo
    too::ignore_arg(FilePathNameWithoutExtension);
    too::ignore_arg(Version);
    return nullptr;

#else
    std::unique_ptr<ILibrary> p;
#if TOO_OS_WINDOWS == 1
    p = too::make_unique<CLibrary_win>(FilePathNameWithoutExtension, Version);
#elif TOO_OS_LINUX == 1
    p = too::make_unique<CLibrary_linux>(FilePathNameWithoutExtension, Version);
#endif
    if (!FilePathNameWithoutExtension.empty())
        p->Load();
    return p;

#endif
}
}
