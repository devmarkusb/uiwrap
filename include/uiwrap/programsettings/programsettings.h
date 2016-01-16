// Markus Borris, 2015
// This file is part of my uiwrap library. Open source.

//!
/**
*/
//! \file

#ifndef PROGRAMSETTINGS_H_INCL_skzhfisxfuxskydfnixusgfiwz
#define PROGRAMSETTINGS_H_INCL_skzhfisxfuxskydfnixusgfiwz

#include <boost/variant.hpp>
#include <vector>
#include "Toolib/string/tooString.h"
#include "Toolib/class/non_copyable.h"
#include "uiwrapDEF.h"


namespace uiw
{
static const too::string& HIERARCHY_SEPARATOR = _TOOSTR(".");

class UIWRAPSHARED_EXPORT IProgSettings : private too::non_copyable
{
public:
    enum class StorageFileFormat
    {
        INI,
        XML,
        JSON,
        BOOST_INFO,
    };

    virtual ~IProgSettings() = default;

    static IProgSettings* GetInstance(const too::string& FileNamePath = too::string(),
        const too::string& FileExt                   = too::string(),
        StorageFileFormat PreferredStorageFileFormat = StorageFileFormat::JSON);
    virtual void Init(const too::string& OrganizationName, const too::string& ApplicationName) = 0;

    using TInteger = int;
    using TVariant = boost::variant<TInteger, double, too::string, bool>;

    virtual void SetValue(const too::string& SectionName, const too::string& KeyName, const TVariant& Value) = 0;
    void SetValue(const too::string& KeyName, const TVariant& Value) { SetValue(too::string(), KeyName, Value); }
    virtual TVariant Value(
        const too::string& SectionName, const too::string& KeyName, const TVariant& Default = TVariant()) const = 0;
    TVariant Value(const too::string& KeyName, const TVariant& Default = TVariant()) const
    {
        return Value(too::string(), KeyName, Default);
    }

    using TSectionKeyPair = std::pair<too::string, too::string>;
    virtual std::vector<TSectionKeyPair> GetAllKeys() const = 0;
    virtual void Clear() = 0;
    virtual bool Contains(const too::string& SectionName, const too::string& KeyName) const = 0;
    bool Contains(const too::string& KeyName) { return Contains(too::string(), KeyName); }
    virtual void Remove(const too::string& SectionName, const too::string& KeyName) = 0;
    void Remove(const too::string& KeyName) { Remove(too::string(), KeyName); }
    //! You need to call this only, if you want to know whether there occurred an error by GetError().
    /** It initiates a persistent writing to data store, which would nevertheless happen asynchronously.
        Perhaps most useful when writing a bunch of program data immediately before program exit.*/
    virtual void Sync() = 0;

    enum class EError
    {
        E_NO_ERROR = 0, // scary: NO_ERROR seems to be defined somewhere in system headers!?
        INIT_NOT_CALLED_OR_FAILED,
        ERROR_WRITING_SETTINGS,
        ERROR_READING_SETTINGS,
        INTERNAL_ERROR__VARIANT_CONVERSION,
    };
    virtual EError GetError() const = 0;
    virtual void ResetError() = 0;
};
}

#endif
