// Markus Borris, 2015-17
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#include "uiwrap_build_config.h"
#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)
#include "impl_Qt/filesys_Qt.h"
#elif defined(UIW_LINKLIB_IMPL_CHOICE_WX)
#include "toolib/error.h"
static_assert(false, "not implemented");
#else
#include "impl_/filesys_.h"
#endif
#include "toolib/std/std_extensions.h"


namespace uiw
{
IFileSys* IFileSys::getInstance()
{
#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)

    static std::unique_ptr<implQt::CFileSys_Qt> instance(too::make_unique<implQt::CFileSys_Qt>());
    return instance.get();

#elif defined(UIW_LINKLIB_IMPL_CHOICE_WX)

    throw too::not_implemented{"IFileSys::GetInstance()"};

#else

    static std::unique_ptr<impl::CFileSys_> instance(too::make_unique<impl::CFileSys_>());
    return instance.get();

#endif
}
}
