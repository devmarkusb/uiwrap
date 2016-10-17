// Markus Borris, 2015
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)
#include "impl_Qt/filesys_Qt.h"
#elif defined(UIW_LINKLIB_IMPL_CHOICE_WX)
// todo
#else
#include "impl_/filesys_.h"
#endif
#include "Toolib/std/std_extensions.h"


namespace uiw
{
IFileSys* IFileSys::GetInstance()
{
#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)

    static std::unique_ptr<implQt::CFileSys_Qt> instance(too::make_unique<implQt::CFileSys_Qt>());
    return instance.get();

#elif defined(UIW_LINKLIB_IMPL_CHOICE_WX)

    // todo
    return nullptr;

#else

    static std::unique_ptr<impl::CFileSys_> instance(too::make_unique<impl::CFileSys_>());
    return instance.get();

#endif
}
}
