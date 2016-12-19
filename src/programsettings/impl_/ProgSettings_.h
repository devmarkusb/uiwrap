// Markus Borris, 2015
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef PROGSETTINGS__H_INCL_ieuhrngt783znt7238t87t3
#define PROGSETTINGS__H_INCL_ieuhrngt783znt7238t87t3

#include "uiwrap/programsettings/programsettings.h"
#include "Toolib/error.h"
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_BEGIN"
#include "Toolib/PPDefs/GCC/SUPPRESS_WARNING_unused-local-typedefs_BEGIN"
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "Toolib/PPDefs/GCC/SUPPRESS_WARNING_END"
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_END"
#include <memory>


namespace uiw
{
namespace impl
{
namespace boost_pt = boost::property_tree;

class CProgSettings : public uiw::IProgSettings
{
public:
    inline CProgSettings(const std::string& FileNamePath, const std::string& FileExt,
        StorageFileFormat PreferredStorageFileFormat = StorageFileFormat::JSON);
    virtual inline ~CProgSettings();

    virtual inline void Init(const std::string& OrganizationName, const std::string& ApplicationName) override;

    virtual inline TVariant Value(const std::string& SectionName, const std::string& KeyName,
        const TVariant& Default = TVariant()) const override;
    virtual inline void SetValue(
        const std::string& SectionName, const std::string& KeyName, const TVariant& Value) override;

    virtual std::string ValueStr(
            const std::string& SectionName, const std::string& KeyName, const std::string& Default = {}) const override;
    virtual void SetValueStr(
            const std::string& SectionName, const std::string& KeyName, const std::string& Value) override;

    virtual inline std::vector<TSectionKeyPair> GetAllKeys() const override;
    virtual inline void Clear() override;
    virtual inline bool Contains(const std::string& SectionName, const std::string& KeyName) const override;
    virtual inline void Remove(const std::string& SectionName, const std::string& KeyName) override;
    virtual inline void Sync() override;

    virtual inline EError GetError() const override;
    virtual inline void ResetError() override;

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
    //! \param first and \param second mustn't start or end with HIERARCHY_SEPARATOR
    static inline std::string concatenateWithHierarchySep(const std::string& first, const std::string& second);
};


//####################################################################################################################

inline CProgSettings::CProgSettings(
    const std::string& FileNamePath, const std::string& FileExt, StorageFileFormat PreferredStorageFileFormat)
    : m_StorageFileFormat(PreferredStorageFileFormat)
{
    m_FileName = concatenateWithHierarchySep(FileNamePath, FileExt);
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

inline std::string CProgSettings::concatenateWithHierarchySep(const std::string& first, const std::string& second)
{
    std::string ret{first};
    ret += HIERARCHY_SEPARATOR;
    ret += second;
    return ret;
}

inline void CProgSettings::Init(const std::string&, const std::string&)
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

inline IProgSettings::TVariant CProgSettings::Value(
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

inline void CProgSettings::SetValue(
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

inline std::string CProgSettings::ValueStr(
        const std::string&, const std::string&, const std::string&) const
{
    throw too::not_implemented("SetValueStr");
}

inline void CProgSettings::SetValueStr(
        const std::string&, const std::string&, const std::string&)
{
    throw too::not_implemented("SetValueStr");
}

inline std::vector<IProgSettings::TSectionKeyPair> CProgSettings::GetAllKeys() const
{
    std::vector<IProgSettings::TSectionKeyPair> ret;
    for (const auto& path : m_PropTree)
        ret.push_back(std::make_pair(std::string(), path.first));
    return ret;
}

inline void CProgSettings::Clear()
{
    m_PropTree.clear();
}

inline bool CProgSettings::Contains(const std::string& SectionName, const std::string& KeyName) const
{
    const std::string path{concatenateWithHierarchySep(SectionName, KeyName)};
    return m_PropTree.find(path) != m_PropTree.not_found();
}

inline void CProgSettings::Remove(const std::string& SectionName, const std::string& KeyName)
{
    const std::string path{concatenateWithHierarchySep(SectionName, KeyName)};
    m_PropTree.erase(path);
}

inline void CProgSettings::Sync()
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

inline IProgSettings::EError CProgSettings::GetError() const
{
    return m_FirstOccurredError;
}

inline void CProgSettings::ResetError()
{
    m_FirstOccurredError = EError::E_NO_ERROR;
}
}
}

#endif
