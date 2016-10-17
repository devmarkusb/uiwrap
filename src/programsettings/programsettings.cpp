// Markus Borris, 2015
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)
#include "impl_Qt/ProgSettings_Qt.h"
#elif defined(UIW_LINKLIB_IMPL_CHOICE_WX)
// todo
#else
#include "impl_/ProgSettings_.h"
#endif
#include "Toolib/ignore_arg.h"
#include "Toolib/std/std_extensions.h"


namespace uiw
{
IProgSettings* IProgSettings::GetInstance(
    const std::string& FileNamePath, const std::string& FileExt, StorageFileFormat PreferredStorageFileFormat)
{
#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)

    too::ignore_arg(FileNamePath);
    too::ignore_arg(FileExt);
    too::ignore_arg(PreferredStorageFileFormat);
    static implQt::CProgSettings instance;
    return &instance;

#elif defined(UIW_LINKLIB_IMPL_CHOICE_WX)

    // todo
    return nullptr;

#else

    static impl::CProgSettings instance(FileNamePath, FileExt, PreferredStorageFileFormat);
    return &instance;

#endif
}
}
