#include "uiwrap_build_config.h"
#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)
#include "impl_Qt/library_Qt.h"
#elif defined(UIW_LINKLIB_IMPL_CHOICE_WX)
static_assert(false, "not implemented");
#else
#include "impl_/library_.h"
#endif
#include "ul/ul.h"

namespace mb::uiw {
std::unique_ptr<ILibrary> ILibrary::make(const std::string& filePathNameWithoutExtension, const std::string& version) {
#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)

    std::unique_ptr<ILibrary> p(ul::make_unique<implQt::CLibrary_Qt>(filePathNameWithoutExtension, version));
    if (!filePathNameWithoutExtension.empty())
        p->Load();
    return p;

#elif defined(UIW_LINKLIB_IMPL_CHOICE_WX)

    ul::ignore_unused(filePathNameWithoutExtension);
    ul::ignore_unused(version);
    throw ul::not_implemented{"ILibrary::make"};

#else
    std::unique_ptr<ILibrary> p;
#if UL_OS_WINDOWS
    p = ul::make_unique<CLibrary_win>(FilePathNameWithoutExtension, Version);
#elif UL_OS_LINUX
    p = ul::make_unique<CLibrary_linux>(filePathNameWithoutExtension, version);
#endif
    if (!filePathNameWithoutExtension.empty())
        p->Load();
    return p;

#endif
}
} // namespace mb::uiw
