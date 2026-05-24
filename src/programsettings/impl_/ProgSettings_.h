// 2015-17

//! \file

#ifndef PROGSETTINGS_H_ieuhrngt783znt7238t87t3
#define PROGSETTINGS_H_ieuhrngt783znt7238t87t3

#include "uiwrap/programsettings/programsettings.h"
UL_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC
UL_WARNING_DISABLE_CLANG_ALL
UL_WARNING_DISABLE_GCC(unused-local-typedefs)
#include "boost/property_tree/ptree.hpp"
// cppcheck-suppress unknownMacro
UL_PRAGMA_WARNINGS_POP
#include <string>

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
};
} // namespace mb::uiw::impl

#endif
