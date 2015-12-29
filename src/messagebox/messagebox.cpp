// Markus Borris, 2015
// This file is part of my uiwrap library. Open source.

//!
/**
*/
//! \file

#include "Toolib/std/std_extensions.h"
#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)
#include "impl_Qt/MessageBox_Qt.h"
#elif defined(UIW_LINKLIB_IMPL_CHOICE_WX)
//todo
#else
//todo
#endif


namespace uiw
{
	static std::unique_ptr<IMessageBox> IMessageBox::make()
	{
#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)

		return std::make_unique<implQt::CMessageBox_Qt>();

#elif defined(UIW_LINKLIB_IMPL_CHOICE_WX)

		//todo
		return nullptr;

#else

		//todo
		return nullptr;

#endif
	}
}