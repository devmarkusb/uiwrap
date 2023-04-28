#include "ProgSettings_Qt.h"
#include "uiwrap/string/impl_Qt/stringconvert_Qt.h"
#include "ul/ul.h"
UL_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC
#include "QQmlApplicationEngine"
#include "QQmlContext"
UL_PRAGMA_WARNINGS_POP

namespace mb::uiw::implQt {
QSettings* CProgSettings::m_settings() {
    if (!this->enabled)
        return {};
    UL_ASSERT(m_settings_impl_doNotUseItDirectlyExceptOnInit);
    if (!m_settings_impl_doNotUseItDirectlyExceptOnInit)
        SetError(EError::INIT_NOT_CALLED_OR_FAILED);
    return m_settings_impl_doNotUseItDirectlyExceptOnInit.get();
}

const QSettings* CProgSettings::m_settings() const {
    UL_ASSERT(m_settings_impl_doNotUseItDirectlyExceptOnInit);
    if (!m_settings_impl_doNotUseItDirectlyExceptOnInit)
        SetError(EError::INIT_NOT_CALLED_OR_FAILED);
    return m_settings_impl_doNotUseItDirectlyExceptOnInit.get();
}

void CProgSettings::init(const std::string& organizationName, const std::string& applicationName) {
    m_settings_impl_doNotUseItDirectlyExceptOnInit = ul::make_unique<QSettings>(
        QSettings::IniFormat, QSettings::UserScope, s2qs(organizationName), s2qs(applicationName));
    auto ok = m_settings_impl_doNotUseItDirectlyExceptOnInit->status();
    UL_ASSERT(ok == QSettings::NoError);
    ul::ignore_unused(ok);
    m_FirstOccurredError = EError::E_NO_ERROR;
    getError();
}

CProgSettings::EError CProgSettings::getError() const {
    if (m_FirstOccurredError == EError::E_NO_ERROR) {
        if (m_settings()) {
            if (m_settings()->status() == QSettings::FormatError)
                SetError(EError::ERROR_READING_SETTINGS);
            else if (m_settings()->status() == QSettings::AccessError)
                SetError(EError::ERROR_WRITING_SETTINGS);
        }
    }
    return m_FirstOccurredError;
}

void CProgSettings::resetError() {
    m_FirstOccurredError = EError::E_NO_ERROR;
}

void CProgSettings::setAsRootContextProperty(void* application_engine, const std::string& name) {
    UL_EXPECT_THROW(application_engine);
    if (!m_settings())
        return;
    auto* ae = reinterpret_cast<QQmlApplicationEngine*>(application_engine);
    ae->rootContext()->setContextProperty(s2qs(name), this);
}

void CProgSettings::setValue(const QString& secAndKey, const QVariant& value) {
    if (!m_settings())
        return;
    m_settings()->setValue(secAndKey, value);
}

QVariant CProgSettings::value(const QString& secAndKey, const QVariant& def) const {
    if (!m_settings())
        return {};
    return m_settings()->value(secAndKey, def);
}

void CProgSettings::flush() {
    sync();
}

void CProgSettings::sync() {
    if (!m_settings())
        return;
    m_settings()->sync();
    getError();
}

QString CProgSettings::CreateQtKeyName(const std::string& sectionName, const std::string& keyName) {
    return s2qs(sectionName + "/" + keyName);
}

void CProgSettings::remove(const std::string& sectionName, const std::string& keyName) {
    if (!m_settings())
        return;
    m_settings()->remove(CreateQtKeyName(sectionName, keyName));
    // here writing can occur asynchronously, so GetError() doesn't make sense before a call to Sync()
}

bool CProgSettings::contains(const std::string& sectionName, const std::string& keyName) const {
    if (!m_settings())
        return false;
    bool ret = m_settings()->contains(CreateQtKeyName(sectionName, keyName));
    getError();
    return ret;
}

void CProgSettings::clear() {
    if (!m_settings())
        return;
    m_settings()->clear();
    // here writing can occur asynchronously, so GetError() doesn't make sense before a call to Sync()
}

void CProgSettings::enable(bool enable) {
    this->enabled = enable;
}

std::vector<CProgSettings::TSectionKeyPair> CProgSettings::getAllKeys() const {
    std::vector<TSectionKeyPair> retkeys;
    if (!m_settings())
        return retkeys;
    QStringList keys(m_settings()->allKeys());
    retkeys.reserve(static_cast<size_t>(keys.size()));
    for (const auto& i : keys) {
        std::vector<std::string> keypath;
        ul::str::tokenizeString(qs2s(i), "/", keypath);
        // only support one level of sections, so take the last two entries
        if (keypath.empty())
            continue;
        TSectionKeyPair SecKey;
        SecKey.second = keypath.back();
        if (keypath.size() >= 2)
            SecKey.first = keypath[keypath.size() - 2];
        retkeys.push_back(SecKey);
    }
    return retkeys;
}

std::string CProgSettings::valueStr(
    const std::string& sectionName, const std::string& keyName, const std::string& def) const {
    if (!m_settings())
        return {};
    QString val{m_settings()->value(CreateQtKeyName(sectionName, keyName), QVariant{s2qs(def)}).toString()};
    getError();
    return qs2s(val);
}

void CProgSettings::setValueStr(const std::string& sectionName, const std::string& keyName, const std::string& value) {
    if (!m_settings())
        return;
    m_settings()->setValue(CreateQtKeyName(sectionName, keyName), s2qs(value));
}
} // namespace mb::uiw::implQt
