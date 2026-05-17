// 2015-17

//! \file

#ifndef PROGSETTINGS_H_ieuhrngt783znt7238t87t3
#define PROGSETTINGS_H_ieuhrngt783znt7238t87t3

#include "mb/ul/ul.hpp"
#include "uiwrap/programsettings/programsettings.h"
UL_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC
UL_WARNING_DISABLE_GCC(unused-local-typedefs)
#include "boost/property_tree/info_parser.hpp"
#include "boost/property_tree/ini_parser.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
UL_PRAGMA_WARNINGS_POP
#include <algorithm>
#include <iterator>
#include <memory>
#include <utility>

namespace mb::uiw::impl {
namespace boost_pt = boost::property_tree;

class CProgSettings : public uiw::IProgSettings {
public:
    CProgSettings(
        const std::string& fileNamePath,
        const std::string& fileExt,
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

    void persistToFile();

    EError getError() const override;
    void resetError() override;

private:
    mutable EError m_FirstOccurredError{EError::INIT_NOT_CALLED_OR_FAILED};
    StorageFileFormat m_StorageFileFormat{StorageFileFormat::JSON};
    std::string m_FileName;
    boost_pt::ptree m_PropTree;

    void SetError(EError e) const {
        if (m_FirstOccurredError == EError::E_NO_ERROR)
            m_FirstOccurredError = e;
    }

    //! first and second mustn't start or end with HIERARCHY_SEPARATOR
    static std::string concatenateWithHierarchySep(const std::string& first, const std::string& second);

    template <typename ReadFn>
    void readStorageFile(ReadFn&& readFn) {
        std::forward<ReadFn>(readFn)(m_FileName, m_PropTree);
    }

    template <typename WriteFn>
    void writeStorageFile(WriteFn&& writeFn) {
        std::forward<WriteFn>(writeFn)(m_FileName, m_PropTree);
    }
};

//####################################################################################################################

CProgSettings::CProgSettings(
    const std::string& fileNamePath, const std::string& fileExt, StorageFileFormat preferredStorageFileFormat)
    : m_StorageFileFormat{preferredStorageFileFormat}
    , m_FileName{concatenateWithHierarchySep(fileNamePath, fileExt)} {
}

CProgSettings::~CProgSettings() {
    try {
        persistToFile();
    } catch (...) {
    }
}

std::string CProgSettings::concatenateWithHierarchySep(const std::string& first, const std::string& second) {
    std::string ret{first};
    ret.append(HIERARCHY_SEPARATOR);
    ret += second;
    return ret;
}

void CProgSettings::init(const std::string&, const std::string&) {
    try {
        switch (m_StorageFileFormat) {
            case StorageFileFormat::INI:
                readStorageFile(boost::property_tree::read_ini);
                break;
            case StorageFileFormat::XML:
                readStorageFile(boost::property_tree::read_xml);
                break;
            case StorageFileFormat::JSON:
                readStorageFile(boost::property_tree::read_json);
                break;
            case StorageFileFormat::BOOST_INFO:
                readStorageFile(boost::property_tree::read_info);
                break;
            default:
                return;
        }
    } catch (...) {
        SetError(EError::ERROR_READING_SETTINGS);
        return;
    }
    m_FirstOccurredError = EError::E_NO_ERROR;
}

std::string CProgSettings::valueStr(const std::string&, const std::string&, const std::string&) const {
    throw ul::NotImplemented{"valueStr"};
}

void CProgSettings::setValueStr(const std::string&, const std::string&, const std::string&) {
    throw ul::NotImplemented("setValueStr");
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static) -- uses m_PropTree
std::vector<IProgSettings::TSectionKeyPair> CProgSettings::getAllKeys() const {
    std::vector<IProgSettings::TSectionKeyPair> ret;
    ret.reserve(m_PropTree.size());
    std::transform(
        m_PropTree.begin(), m_PropTree.end(), std::back_inserter(ret), [](const boost_pt::ptree::value_type& path) {
            return IProgSettings::TSectionKeyPair{std::string(), path.first};
        });
    return ret;
}

void CProgSettings::clear() {
    m_PropTree.clear();
}

void CProgSettings::enable(bool) {
    throw ul::NotImplemented("enable");
}

bool CProgSettings::contains(const std::string& sectionName, const std::string& keyName) const {
    const std::string path{concatenateWithHierarchySep(sectionName, keyName)};
    return m_PropTree.find(path) != m_PropTree.not_found();
}

void CProgSettings::remove(const std::string& sectionName, const std::string& keyName) {
    const std::string path{concatenateWithHierarchySep(sectionName, keyName)};
    m_PropTree.erase(path);
}

void CProgSettings::sync() {
    persistToFile();
}

void CProgSettings::persistToFile() {
    try {
        switch (m_StorageFileFormat) {
            case StorageFileFormat::INI:
                writeStorageFile(boost::property_tree::write_ini);
                break;
            case StorageFileFormat::XML:
                writeStorageFile(boost::property_tree::write_xml);
                break;
            case StorageFileFormat::JSON:
                writeStorageFile(boost::property_tree::write_json);
                break;
            case StorageFileFormat::BOOST_INFO:
                writeStorageFile(boost::property_tree::write_info);
                break;
            default:
                return;
        }
    } catch (...) {
        SetError(EError::ERROR_WRITING_SETTINGS);
        return;
    }
}

IProgSettings::EError CProgSettings::getError() const {
    return m_FirstOccurredError;
}

void CProgSettings::resetError() {
    m_FirstOccurredError = EError::E_NO_ERROR;
}
} // namespace mb::uiw::impl

#endif
