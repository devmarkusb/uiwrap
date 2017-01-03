// Markus Borris, 2016
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef LANGUAGE_ID_CONVERT_H_idurghfzexnn24789sgh45
#define LANGUAGE_ID_CONVERT_H_idurghfzexnn24789sgh45
//#ifdef UIW_LINKLIB_IMPL_CHOICE_QT

#include "Toolib/assert.h"
#include "Toolib/error.h"
#include "Toolib/string/languages.h"
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_BEGIN"
#include <QLocale>
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_END"
#include <string>


namespace uiw
{
namespace implQt
{
inline QLocale::Language fromLanguageID(const too::str::LangID_iso639_1& id)
{
    if (id == too::str::auto_systems_language)
    {
        // on library-level we can't be sure that every GUI framework besides Qt understands
        // auto/systems language as a language itself (it really isn't), so please handle it separately
        // (even for Qt I'm unsure whether QLocale::C or QLocale::AnyLanguage are correct choices,
        // the first one certainly not - doc says something about being English always)
        TOO_ASSERT_THROW(false);
    }
    else if (id == "ar")
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
    else if (id == "jp")
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
        throw too::not_implemented{"unsupported or unknonwn ISO 639-1 language code"};
}
} // implQt
} // uiw

//#endif //UIW_LINKLIB_IMPL_CHOICE_QT
#endif