// Markus Borris, 2015
// This file is part of my uiwrap library. Open source.

//!
/**
*/
//! \file

#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)
#include "impl_Qt/MessageBox_Qt.h"
#elif defined(UIW_LINKLIB_IMPL_CHOICE_WX)
static_assert(false, "not implemented");
#else
#include "impl/messagebox_.h"
#endif
#include "Toolib/std/std_extensions.h"


namespace uiw
{
namespace gui
{

std::unique_ptr<IMessageBox> IMessageBox::make()
{
#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)

    return too::make_unique<uiw::implQt::CMessageBox_Qt>();

#elif defined(UIW_LINKLIB_IMPL_CHOICE_WX)

    throw too::not_implemented{"IMessageBox::make"};

#else

    return too::make_unique<uiw::impl::MessageBox>();

#endif
}
}
}
