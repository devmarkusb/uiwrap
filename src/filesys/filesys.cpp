#include "uiwrap/filesys/filesys.h"
#include "uiwrap_build_config.h"

#ifdef UIW_LINKLIB_IMPL_CHOICE_QT
#include "impl_Qt/filesys_Qt.h"
#elifdef UIW_LINKLIB_IMPL_CHOICE_WX
static_assert(false, "not implemented");
#else
#include "impl_/filesys_.h"
#endif

#include "mb/ul/ul.hpp"

namespace mb::uiw::file {
IFileSys* IFileSys::getInstance() {
#ifdef UIW_LINKLIB_IMPL_CHOICE_QT

    static const std::unique_ptr<implQt::CFileSys_Qt> instance(ul::make_unique<implQt::CFileSys_Qt>());
    return instance.get();

#elifdef UIW_LINKLIB_IMPL_CHOICE_WX

    throw ul::NotImplemented{"IFileSys::GetInstance()"};

#else

    static const std::unique_ptr<impl::CFileSys_> instance(ul::make_unique<impl::CFileSys_>());
    return instance.get();

#endif
}
} // namespace mb::uiw::file
