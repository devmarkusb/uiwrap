//! \file

#ifndef LANGUAGE_ID_CONVERT_H_idurghfzexnn24789sgh45
#define LANGUAGE_ID_CONVERT_H_idurghfzexnn24789sgh45

#include "uiwrap_build_config.h"
#include "../../config.h"

#ifdef UIW_LINKLIB_IMPL_CHOICE_QT
#include "ul/ul.h"
UL_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC
#include "QLocale"
UL_PRAGMA_WARNINGS_POP
#include <string>

namespace mb::uiw::implQt {
//! Throws ul::not_implemented for unsupported or unknown id, expects id not to be ul::str::auto_systems_language.
inline QLocale::Language fromLanguageID(const ul::str::LangID_iso639_1& id) {
    if (id == ul::str::auto_systems_language) {
        // on library-level we can't be sure that every GUI framework besides Qt understands
        // auto/systems language as a language itself (it really isn't), so please handle it separately
        // (even for Qt I'm unsure whether QLocale::C or QLocale::AnyLanguage are correct choices,
        // the first one certainly not - doc says something about being English always)
        UL_ASSERT_THROW(false);
    } else if (id == "ar")
        return QLocale::Arabic;
    else if (id == "bn")
        return QLocale::Bengali;
    else if (id == "de")
        return QLocale::German;
    else if (id == "en")
        return QLocale::English;
    else if (id == "es")
        return QLocale::Spanish;
    else if (id == "fr")
        return QLocale::French;
    else if (id == "hi")
        return QLocale::Hindi;
    else if (id == "it")
        return QLocale::Italian;
    else if (id == "ja")
        return QLocale::Japanese;
    else if (id == "ko")
        return QLocale::Korean;
    else if (id == "pt")
        return QLocale::Portuguese;
    else if (id == "ru")
        return QLocale::Russian;
    else if (id == "zh")
        return QLocale::Chinese;
    else
        throw ul::not_implemented{"unsupported or unknonwn ISO 639-1 language code"};
}
} // namespace mb::uiw::implQt

#endif // UIW_LINKLIB_IMPL_CHOICE_QT

UIW_HEADER_END

#endif
