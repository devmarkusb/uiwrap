#include "uiwrap_build_config.h"
#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)
#include "impl_Qt/ProgSettings_Qt.h"
#elif defined(UIW_LINKLIB_IMPL_CHOICE_WX)
static_assert(false, "not implemented");
#else
#include "impl_/ProgSettings_.h"
#endif
#include "ul/ul.h"


namespace mb::uiw
{
IProgSettings* IProgSettings::getInstance(
    const std::string& fileNamePath, const std::string& fileExt, StorageFileFormat preferredStorageFileFormat)
{
#if defined(UIW_LINKLIB_IMPL_CHOICE_QT)

    ul::ignore_arg(fileNamePath);
    ul::ignore_arg(fileExt);
    ul::ignore_arg(preferredStorageFileFormat);
    static implQt::CProgSettings instance;
    return &instance;

#elif defined(UIW_LINKLIB_IMPL_CHOICE_WX)

    throw ul::not_implemented{"IProgSettings::getInstance"};

#else

    static impl::CProgSettings instance(fileNamePath, fileExt, preferredStorageFileFormat);
    return &instance;

#endif
}
} // namespace mb::uiw
