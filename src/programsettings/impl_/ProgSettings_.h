// 2015-17

//! \file

#ifndef PROGSETTINGS_H_ieuhrngt783znt7238t87t3
#define PROGSETTINGS_H_ieuhrngt783znt7238t87t3

#include "uiwrap/programsettings/programsettings.h"
#include "ul/ul.h"
UL_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC
UL_WARNING_DISABLE_GCC(unused - local - typedefs)
#include "boost/property_tree/info_parser.hpp"
#include "boost/property_tree/ini_parser.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
UL_PRAGMA_WARNINGS_POP
#include <memory>


namespace mb::uiw::impl
{
namespace boost_pt = boost::property_tree;

class CProgSettings : public uiw::IProgSettings
{
public:
    CProgSettings(
        const std::string& fileNamePath, const std::string& fileExt,
        StorageFileFormat preferredStorageFileFormat = StorageFileFormat::JSON);
    ~CProgSettings() override;

    void Init(const std::string& OrganizationName, const std::string& ApplicationName) override;

    TVariant Value(
        const std::string& SectionName, const std::string& KeyName,
        const TVariant& Default) const override;
    void SetValue(
        const std::string& SectionName, const std::string& KeyName, const TVariant& Value) override;

    std::string ValueStr(
        const std::string& SectionName, const std::string& KeyName, const std::string& Default) const override;
    void SetValueStr(
        const std::string& SectionName, const std::string& KeyName, const std::string& Value) override;

    std::vector<TSectionKeyPair> GetAllKeys() const override;
    void Clear() override;
    void enable(bool enable) override;
    bool Contains(const std::string& SectionName, const std::string& KeyName) const override;
    void Remove(const std::string& SectionName, const std::string& KeyName) override;
    void Sync() override;

    EError GetError() const override;
    void ResetError() override;

private:
    mutable EError m_FirstOccurredError{EError::INIT_NOT_CALLED_OR_FAILED};
    StorageFileFormat m_StorageFileFormat{StorageFileFormat::JSON};
    std::string m_FileName;
    boost_pt::ptree m_PropTree;

    void SetError(EError e) const
    {
        if (m_FirstOccurredError == EError::E_NO_ERROR)
            m_FirstOccurredError = e;
    }
    //! first and second mustn't start or end with HIERARCHY_SEPARATOR
    static std::string concatenateWithHierarchySep(const std::string& first, const std::string& second);
};


//####################################################################################################################

CProgSettings::CProgSettings(
    const std::string& fileNamePath, const std::string& fileExt, StorageFileFormat preferredStorageFileFormat)
    : m_StorageFileFormat{preferredStorageFileFormat}
    , m_FileName{concatenateWithHierarchySep(fileNamePath, fileExt)}
{
}

CProgSettings::~CProgSettings()
{
    try
    {
        Sync();
    }
    catch (...)
    {
    }
}

std::string CProgSettings::concatenateWithHierarchySep(const std::string& first, const std::string& second)
{
    std::string ret{first};
    ret += HIERARCHY_SEPARATOR;
    ret += second;
    return ret;
}

void CProgSettings::Init(const std::string&, const std::string&)
{
    try
    {
        switch (m_StorageFileFormat)
        {
            case StorageFileFormat::INI:
                boost::property_tree::read_ini(m_FileName, m_PropTree);
                break;
            case StorageFileFormat::XML:
                boost::property_tree::read_xml(m_FileName, m_PropTree);
                break;
            case StorageFileFormat::JSON:
                boost::property_tree::read_json(m_FileName, m_PropTree);
                break;
            case StorageFileFormat::BOOST_INFO:
                boost::property_tree::read_info(m_FileName, m_PropTree);
                break;
            default:
                return;
        }
    }
    catch (...)
    {
        SetError(EError::ERROR_READING_SETTINGS);
        return;
    }
    m_FirstOccurredError = EError::E_NO_ERROR;
}

IProgSettings::TVariant CProgSettings::Value(
    const std::string& SectionName, const std::string& KeyName, const IProgSettings::TVariant& Default) const
{
    const std::string path{concatenateWithHierarchySep(SectionName, KeyName)};
    try
    {
        return m_PropTree.get<int>(path);
    }
    catch (const boost_pt::ptree_bad_path&)
    {
        return Default;
    }
    catch (const boost_pt::ptree_bad_data&)
    {
    }
    try
    {
        return m_PropTree.get<double>(path);
    }
    catch (const boost_pt::ptree_bad_data&)
    {
    }
    try
    {
        return m_PropTree.get<bool>(path);
    }
    catch (const boost_pt::ptree_bad_data&)
    {
    }
    try
    {
        return m_PropTree.get<std::string>(path);
    }
    catch (const boost_pt::ptree_bad_data&)
    {
        SetError(EError::INTERNAL_ERROR__VARIANT_CONVERSION);
    }
    return Default;
}

void CProgSettings::SetValue(
    const std::string& SectionName, const std::string& KeyName, const IProgSettings::TVariant& Value)
{
    const std::string path{concatenateWithHierarchySep(SectionName, KeyName)};
    try
    {
        m_PropTree.put(path, Value);
    }
    catch (const boost_pt::ptree_bad_data&)
    {
        SetError(EError::INTERNAL_ERROR__VARIANT_CONVERSION);
    }
}

std::string CProgSettings::ValueStr(const std::string&, const std::string&, const std::string&) const
{
    throw ul::not_implemented{"SetValueStr"};
}

void CProgSettings::SetValueStr(const std::string&, const std::string&, const std::string&)
{
    throw ul::not_implemented("SetValueStr");
}

std::vector<IProgSettings::TSectionKeyPair> CProgSettings::GetAllKeys() const
{
    std::vector<IProgSettings::TSectionKeyPair> ret;
    for (const auto& path : m_PropTree)
        ret.emplace_back(std::string(), path.first);
    return ret;
}

void CProgSettings::Clear()
{
    m_PropTree.clear();
}

void CProgSettings::enable(bool)
{
    throw ul::not_implemented("enable");
}

bool CProgSettings::Contains(const std::string& SectionName, const std::string& KeyName) const
{
    const std::string path{concatenateWithHierarchySep(SectionName, KeyName)};
    return m_PropTree.find(path) != m_PropTree.not_found();
}

void CProgSettings::Remove(const std::string& SectionName, const std::string& KeyName)
{
    const std::string path{concatenateWithHierarchySep(SectionName, KeyName)};
    m_PropTree.erase(path);
}

void CProgSettings::Sync()
{
    try
    {
        switch (m_StorageFileFormat)
        {
            case StorageFileFormat::INI:
                boost::property_tree::write_ini(m_FileName, m_PropTree);
                break;
            case StorageFileFormat::XML:
                boost::property_tree::write_xml(m_FileName, m_PropTree);
                break;
            case StorageFileFormat::JSON:
                boost::property_tree::write_json(m_FileName, m_PropTree);
                break;
            case StorageFileFormat::BOOST_INFO:
                boost::property_tree::write_info(m_FileName, m_PropTree);
                break;
            default:
                return;
        }
    }
    catch (...)
    {
        SetError(EError::ERROR_WRITING_SETTINGS);
        return;
    }
}

IProgSettings::EError CProgSettings::GetError() const
{
    return m_FirstOccurredError;
}

void CProgSettings::ResetError()
{
    m_FirstOccurredError = EError::E_NO_ERROR;
}
} // namespace mb::uiw::impl

#endif
