// Markus Borris, 2015
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef PROGRAMSETTINGS_H_INCL_skzhfisxfuxskydfnixusgfiwz
#define PROGRAMSETTINGS_H_INCL_skzhfisxfuxskydfnixusgfiwz

#include <boost/variant.hpp>
#include <vector>
#include <string>
#include "Toolib/class/non_copyable.h"
#include "uiwrapDEF.h"


namespace uiw
{
static const std::string& HIERARCHY_SEPARATOR = ".";

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

    static IProgSettings* GetInstance(const std::string& FileNamePath = std::string(),
        const std::string& FileExt                   = std::string(),
        StorageFileFormat PreferredStorageFileFormat = StorageFileFormat::JSON);
    virtual void Init(const std::string& OrganizationName, const std::string& ApplicationName) = 0;

    using TInteger = int;
    using TVariant = boost::variant<TInteger, double, std::string, bool>;

    virtual void SetValue(const std::string& SectionName, const std::string& KeyName, const TVariant& Value) = 0;
    void SetValue(const std::string& KeyName, const TVariant& Value) { SetValue(std::string(), KeyName, Value); }
    virtual TVariant Value(
        const std::string& SectionName, const std::string& KeyName, const TVariant& Default = TVariant()) const = 0;
    TVariant Value(const std::string& KeyName, const TVariant& Default = TVariant()) const
    {
        return Value(std::string(), KeyName, Default);
    }

    using TSectionKeyPair = std::pair<std::string, std::string>;
    virtual std::vector<TSectionKeyPair> GetAllKeys() const = 0;
    virtual void Clear() = 0;
    virtual bool Contains(const std::string& SectionName, const std::string& KeyName) const = 0;
    bool Contains(const std::string& KeyName) { return Contains(std::string(), KeyName); }
    virtual void Remove(const std::string& SectionName, const std::string& KeyName) = 0;
    void Remove(const std::string& KeyName) { Remove(std::string(), KeyName); }
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
