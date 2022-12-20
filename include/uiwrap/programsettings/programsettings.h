// 2015-17

//! \file

#ifndef PROGRAMSETTINGS_H_skzhfisxfuxskydfnixusgfiwz
#define PROGRAMSETTINGS_H_skzhfisxfuxskydfnixusgfiwz

#include "uiwrapDEF.h"
#include "uiwrap_build_config.h"
#include "../config.h"
#include "ul/ul.h"

#include "boost/variant.hpp"
#include <string>
#include <vector>


namespace mb::uiw
{
#if !defined(UIW_LINKLIB_IMPL_CHOICE_QT)
static const std::string& HIERARCHY_SEPARATOR = ".";
#endif

class UIWRAPSHARED_EXPORT IProgSettings : private ul::NonCopyable
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

    /** *Important* Before first usage, that is, with you first call to getInstance,
        you have to call immediately Init and for Qt afterwards also setAsRootContextProperty.
        Only after that a proper functionality of this class is possible.*/
    static IProgSettings* getInstance(
        const std::string& FileNamePath = std::string(), const std::string& FileExt = std::string(),
        StorageFileFormat PreferredStorageFileFormat = StorageFileFormat::JSON);
    virtual void init(const std::string& OrganizationName, const std::string& ApplicationName) = 0;

    using TInteger = int;
    using TVariant = boost::variant<TInteger, double, std::string, bool>;

    //! Not working?!
    class TVariantGetter : public boost::static_visitor<>
    {
    public:
        int operator()(const IProgSettings::TInteger& v) const
        {
            return v;
        }
        double operator()(const double& v) const
        {
            return v;
        }
        std::string operator()(const std::string& v) const
        {
            return v;
        }
        bool operator()(const bool& v) const
        {
            return v;
        }
    };

    //! Not working?! At least two issues:
    /** (1) boost variant is just difficult to use
        (2) Qt implementation yields string always, which doesn't fit when converting to other variant type

        Until any fix, you are unfortunately stuck with the recommendation of using valueStr/setValueStr.*/
    [[nodiscard]] virtual TVariant value(
        const std::string& SectionName, const std::string& KeyName, const TVariant& Default) const = 0;
    //! Not working?!
    virtual void setValue(const std::string& SectionName, const std::string& KeyName, const TVariant& Value) = 0;

    [[nodiscard]] virtual std::string valueStr(
        const std::string& SectionName, const std::string& KeyName, const std::string& Default) const = 0;
    virtual void setValueStr(const std::string& SectionName, const std::string& KeyName, const std::string& Value) = 0;

    using TSectionKeyPair = std::pair<std::string, std::string>;
    [[nodiscard]] virtual std::vector<TSectionKeyPair> getAllKeys() const = 0;
    virtual void clear() = 0;
    //! Enables/disables the class' whole functionality. Default is enabled.
    virtual void enable(bool enable) = 0;
    [[nodiscard]] virtual bool contains(const std::string& SectionName, const std::string& KeyName) const = 0;
    [[nodiscard]] bool contains(const std::string& KeyName) const
    {
        return contains(std::string(), KeyName);
    }
    virtual void remove(const std::string& SectionName, const std::string& KeyName) = 0;
    void remove(const std::string& KeyName)
    {
        remove(std::string(), KeyName);
    }
    //! You need to call this only, if you want to know whether there occurred an error by getError().
    /** It initiates a persistent writing to data store, which would nevertheless happen asynchronously.
        Perhaps most useful when writing a bunch of program data immediately before program exit.*/
    virtual void sync() = 0;

    enum class EError
    {
        E_NO_ERROR = 0, // scary: NO_ERROR seems to be defined somewhere in system headers!?
        INIT_NOT_CALLED_OR_FAILED,
        ERROR_WRITING_SETTINGS,
        ERROR_READING_SETTINGS,
        INTERNAL_ERROR__VARIANT_CONVERSION,
    };
    [[nodiscard]] virtual EError getError() const = 0;
    virtual void resetError() = 0;

    virtual void setAsRootContextProperty(void* application_engine, const std::string& name)
    {
        ul::ignore_arg(application_engine);
        ul::ignore_arg(name);
    }
};
} // namespace mb::uiw

UIW_HEADER_END

#endif
