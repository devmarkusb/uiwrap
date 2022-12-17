#include "uiwrap_build_config.h"
#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)
#include "impl_Qt/MessageBox_Qt.h"
#elif defined(UIW_LINKLIB_IMPL_CHOICE_WX)
static_assert(false, "not implemented");
#else
#include "impl_/messagebox_.h"
#endif
#include "ul/ul.h"


namespace mb::uiw::gui
{
std::unique_ptr<IMessageBox> IMessageBox::make()
{
#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)

    return ul::make_unique<uiw::implQt::CMessageBox_Qt>();

#elif defined(UIW_LINKLIB_IMPL_CHOICE_WX)

    throw ul::not_implemented{"IMessageBox::make"};

#else

    return ul::make_unique<uiw::impl::MessageBox>();

#endif
}
} // namespace mb::uiw::gui
