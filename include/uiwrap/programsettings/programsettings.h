// Markus Borris, 2015-16
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef PROGRAMSETTINGS_H_INCL_skzhfisxfuxskydfnixusgfiwz
#define PROGRAMSETTINGS_H_INCL_skzhfisxfuxskydfnixusgfiwz

#include "uiwrapDEF.h"
#include "Toolib/class/non_copyable.h"
#include "Toolib/ignore_arg.h"
#include <boost/variant.hpp>
#include <string>
#include <vector>


namespace uiw
{
#if !defined(UIW_LINKLIB_IMPL_CHOICE_QT)
static const std::string& HIERARCHY_SEPARATOR = ".";
#endif

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

    //! Not working?!
    class TVariantGetter : public boost::static_visitor<>
    {
    public:
        int operator()(const IProgSettings::TInteger& v) const { return v; }
        double operator()(const double& v) const { return v; }
        std::string operator()(const std::string& v) const { return v; }
        bool operator()(const bool& v) const { return v; }
    };

    //! Not working?! At least two issues:
    /** (1) boost variant is just difficult to use
        (2) Qt implementation yields string always, which doesn't fit when converting to other variant type

        Until any fix, you are unfortunately stuck with the recommendation of using ValueStr/SetValueStr.*/
    virtual TVariant Value(
        const std::string& SectionName, const std::string& KeyName, const TVariant& Default = TVariant()) const = 0;
    //! Not working?!
    virtual void SetValue(const std::string& SectionName, const std::string& KeyName, const TVariant& Value) = 0;

    virtual std::string ValueStr(
            const std::string& SectionName, const std::string& KeyName, const std::string& Default = {}) const = 0;
    virtual void SetValueStr(
            const std::string& SectionName, const std::string& KeyName, const std::string& Value) = 0;

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

    virtual void setAsRootContextProperty(void* application_engine, const std::string& name)
    { too::ignore_arg(application_engine); too::ignore_arg(name); }
};

}

#endif
