// Markus Borris, 2015-17
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#include "uiwrap_build_config.h"
#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)
#include "impl_Qt/ProgSettings_Qt.h"
#elif defined(UIW_LINKLIB_IMPL_CHOICE_WX)
static_assert(false, "not implemented");
#else
#include "impl_/ProgSettings_.h"
#endif
#include "ul/ul.h"


namespace mb::uiw
{
IProgSettings* IProgSettings::GetInstance(
    const std::string& FileNamePath, const std::string& FileExt, StorageFileFormat PreferredStorageFileFormat)
{
#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)

    ul::ignore_arg(FileNamePath);
    ul::ignore_arg(FileExt);
    ul::ignore_arg(PreferredStorageFileFormat);
    static implQt::CProgSettings instance;
    return &instance;

#elif defined(UIW_LINKLIB_IMPL_CHOICE_WX)

    throw ul::not_implemented{"IProgSettings::GetInstance"};

#else

    static impl::CProgSettings instance(FileNamePath, FileExt, PreferredStorageFileFormat);
    return &instance;

#endif
}
}
