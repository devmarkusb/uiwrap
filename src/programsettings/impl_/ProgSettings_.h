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

    void init(const std::string& organizationName, const std::string& applicationName) override;

    std::string valueStr(
        const std::string& sectionName, const std::string& keyName, const std::string& def) const override;
    void setValueStr(const std::string& sectionName, const std::string& keyName, const std::string& value) override;

    std::vector<TSectionKeyPair> getAllKeys() const override;
    void clear() override;
    void enable(bool enable) override;
    bool contains(const std::string& sectionName, const std::string& keyName) const override;
    void remove(const std::string& sectionName, const std::string& keyName) override;
    void sync() override;

    EError getError() const override;
    void resetError() override;

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
        sync();
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

void CProgSettings::init(const std::string&, const std::string&)
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

std::string CProgSettings::valueStr(const std::string&, const std::string&, const std::string&) const
{
    throw ul::not_implemented{"valueStr"};
}

void CProgSettings::setValueStr(const std::string&, const std::string&, const std::string&)
{
    throw ul::not_implemented("setValueStr");
}

std::vector<IProgSettings::TSectionKeyPair> CProgSettings::getAllKeys() const
{
    std::vector<IProgSettings::TSectionKeyPair> ret;
    for (const auto& path : m_PropTree)
        ret.emplace_back(std::string(), path.first);
    return ret;
}

void CProgSettings::clear()
{
    m_PropTree.clear();
}

void CProgSettings::enable(bool)
{
    throw ul::not_implemented("enable");
}

bool CProgSettings::contains(const std::string& sectionName, const std::string& keyName) const
{
    const std::string path{concatenateWithHierarchySep(sectionName, keyName)};
    return m_PropTree.find(path) != m_PropTree.not_found();
}

void CProgSettings::remove(const std::string& sectionName, const std::string& keyName)
{
    const std::string path{concatenateWithHierarchySep(sectionName, keyName)};
    m_PropTree.erase(path);
}

void CProgSettings::sync()
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

IProgSettings::EError CProgSettings::getError() const
{
    return m_FirstOccurredError;
}

void CProgSettings::resetError()
{
    m_FirstOccurredError = EError::E_NO_ERROR;
}
} // namespace mb::uiw::impl

#endif
