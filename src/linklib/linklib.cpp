#include "uiwrap_build_config.h"
#include "uiwrap/linklib/library_interface.h"

#ifdef UIW_LINKLIB_IMPL_CHOICE_QT
#include "impl_Qt/library_Qt.h"
#elifdef UIW_LINKLIB_IMPL_CHOICE_WX
static_assert(false, "not implemented");
#else
#include "impl_/library_.h"
#endif

#include "mb/ul/ul.hpp"

namespace mb::uiw {
std::unique_ptr<ILibrary> ILibrary::make(
    const std::string& filePathNameWithoutExtension, [[maybe_unused]] const std::string& version) {
#ifdef UIW_LINKLIB_IMPL_CHOICE_QT

    std::unique_ptr<ILibrary> p(ul::make_unique<implQt::CLibrary_Qt>(filePathNameWithoutExtension, version));
    if (!filePathNameWithoutExtension.empty())
        p->Load();
    return p;

#elifdef UIW_LINKLIB_IMPL_CHOICE_WX

    ul::ignore_unused(filePathNameWithoutExtension);
    ul::ignore_unused(version);
    throw ul::NotImplemented{"ILibrary::make"};

#else
    std::unique_ptr<ILibrary> p;
#if UL_OS_WINDOWS
    p = ul::make_unique<CLibrary_win>(filePathNameWithoutExtension, version);
#elif UL_OS_LINUX || UL_OS_MAC
    p = ul::make_unique<CLibrary_linux>(filePathNameWithoutExtension, version);
#endif
    // cppcheck-suppress knownConditionTrueFalse
    if (!p)
        throw ul::NotImplemented{"ILibrary::make"};
    if (!filePathNameWithoutExtension.empty())
        p->Load();
    return p;

#endif
}
} // namespace mb::uiw
