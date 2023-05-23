#include "uiwrap_build_config.h"
#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)
#include "impl_Qt/filesys_Qt.h"
#elif defined(UIW_LINKLIB_IMPL_CHOICE_WX)
static_assert(false, "not implemented");
#else
#include "impl_/filesys_.h"
#endif
#include "ul/ul.h"

namespace mb::uiw::file {
IFileSys* IFileSys::getInstance() {
#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)

    static std::unique_ptr<implQt::CFileSys_Qt> instance(ul::make_unique<implQt::CFileSys_Qt>());
    return instance.get();

#elif defined(UIW_LINKLIB_IMPL_CHOICE_WX)

    throw ul::NotImplemented{"IFileSys::GetInstance()"};

#else

    static std::unique_ptr<impl::CFileSys_> instance(ul::make_unique<impl::CFileSys_>());
    return instance.get();

#endif
}
} // namespace mb::uiw::file
