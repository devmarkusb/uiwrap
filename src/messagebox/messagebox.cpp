#include "mb/uiwrap/messagebox/messagebox.hpp"
#include "mb/uiwrap/uiwrap_build_config.hpp"

#ifdef UIW_LINKLIB_IMPL_CHOICE_QT
#include "impl_Qt/MessageBox_Qt.hpp"
#elifdef UIW_LINKLIB_IMPL_CHOICE_WX
static_assert(false, "not implemented");
#else
#include "impl_/messagebox_.hpp"
#endif

#include "mb/ul/ul.hpp"

namespace mb::uiw::gui {
std::unique_ptr<IMessageBox> IMessageBox::make() {
#ifdef UIW_LINKLIB_IMPL_CHOICE_QT

    return ul::make_unique<uiw::implQt::CMessageBox_Qt>();

#elifdef UIW_LINKLIB_IMPL_CHOICE_WX

    throw ul::NotImplemented{"IMessageBox::make"};

#else

    return ul::make_unique<uiw::impl::MessageBox>();

#endif
}
} // namespace mb::uiw::gui
