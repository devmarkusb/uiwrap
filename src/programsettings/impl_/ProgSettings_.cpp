#include "ProgSettings_.hpp"

#include "mb/ul/ul.hpp"
UL_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC
UL_WARNING_DISABLE_CLANG_ALL
UL_WARNING_DISABLE_GCC(unused-local-typedefs)
#include "boost/property_tree/info_parser.hpp"
#include "boost/property_tree/ini_parser.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "boost/property_tree/xml_parser.hpp"
UL_PRAGMA_WARNINGS_POP

#include <algorithm>
#include <iterator>

namespace mb::uiw::impl {
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
        // NOLINTBEGIN(bugprone-branch-clone) -- distinct boost::property_tree readers per format
        switch (m_StorageFileFormat) {
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
        // NOLINTEND(bugprone-branch-clone)
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

std::vector<IProgSettings::TSectionKeyPair> CProgSettings::getAllKeys() const {
    std::vector<IProgSettings::TSectionKeyPair> ret;
    ret.reserve(m_PropTree.size());
    std::ranges::transform(m_PropTree, std::back_inserter(ret), [](const auto& path) {
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
        // NOLINTBEGIN(bugprone-branch-clone) -- distinct boost::property_tree writers per format
        switch (m_StorageFileFormat) {
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
        // NOLINTEND(bugprone-branch-clone)
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
