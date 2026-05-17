#include "uiwrap/programsettings/programsettings.h"

#ifdef UIW_LINKLIB_IMPL_CHOICE_QT
#include "impl_Qt/ProgSettings_Qt.h"
#elifdef UIW_LINKLIB_IMPL_CHOICE_WX
static_assert(false, "not implemented");
#else
#include "impl_/ProgSettings_.h"
#endif

#include "mb/ul/ul.hpp"

namespace mb::uiw {
IProgSettings* IProgSettings::getInstance(
    const std::string& fileNamePath, const std::string& fileExt, StorageFileFormat preferredStorageFileFormat) {
#ifdef UIW_LINKLIB_IMPL_CHOICE_QT

    ul::ignore_unused(fileNamePath);
    ul::ignore_unused(fileExt);
    ul::ignore_unused(preferredStorageFileFormat);
    static implQt::CProgSettings instance;
    return &instance;

#elifdef UIW_LINKLIB_IMPL_CHOICE_WX

    throw ul::NotImplemented{"IProgSettings::getInstance"};

#else

    static impl::CProgSettings instance(fileNamePath, fileExt, preferredStorageFileFormat);
    return &instance;

#endif
}
} // namespace mb::uiw
