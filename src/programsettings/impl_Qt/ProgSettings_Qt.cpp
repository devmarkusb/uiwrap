#include "ProgSettings_Qt.h"
#include "toolib/string/string_token.h"
#include "uiwrap/string/impl_Qt/stringconvert_Qt.h"
#include "ul/ul.h"
UL_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC
#include <QQmlApplicationEngine>
#include <QQmlContext>
UL_PRAGMA_WARNINGS_POP


namespace mb::uiw::implQt
{
QSettings* CProgSettings::m_settings()
{
    if (!this->enabled)
        return {};
    UL_ASSERT(m_settings_impl_doNotUseItDirectlyExceptOnInit);
    if (!m_settings_impl_doNotUseItDirectlyExceptOnInit)
        SetError(EError::INIT_NOT_CALLED_OR_FAILED);
    return m_settings_impl_doNotUseItDirectlyExceptOnInit.get();
}

const QSettings* CProgSettings::m_settings() const
{
    UL_ASSERT(m_settings_impl_doNotUseItDirectlyExceptOnInit);
    if (!m_settings_impl_doNotUseItDirectlyExceptOnInit)
        SetError(EError::INIT_NOT_CALLED_OR_FAILED);
    return m_settings_impl_doNotUseItDirectlyExceptOnInit.get();
}

void CProgSettings::Init(const std::string& OrganizationName, const std::string& ApplicationName)
{
    m_settings_impl_doNotUseItDirectlyExceptOnInit = ul::make_unique<QSettings>(
        QSettings::IniFormat, QSettings::UserScope, s2qs(OrganizationName), s2qs(ApplicationName));
    auto ok = m_settings_impl_doNotUseItDirectlyExceptOnInit->status();
    UL_ASSERT(ok == QSettings::NoError);
    ul::ignore_arg(ok);
    m_FirstOccurredError = EError::E_NO_ERROR;
    GetError();
}

CProgSettings::EError CProgSettings::GetError() const
{
    if (m_FirstOccurredError == EError::E_NO_ERROR)
    {
        if (m_settings())
        {
            if (m_settings()->status() == QSettings::FormatError)
                SetError(EError::ERROR_READING_SETTINGS);
            else if (m_settings()->status() == QSettings::AccessError)
                SetError(EError::ERROR_WRITING_SETTINGS);
        }
    }
    return m_FirstOccurredError;
}

void CProgSettings::ResetError()
{
    m_FirstOccurredError = EError::E_NO_ERROR;
}

void CProgSettings::setAsRootContextProperty(void* application_engine, const std::string& name)
{
    UL_EXPECT_THROW(application_engine);
    if (!m_settings())
        return;
    auto* ae = reinterpret_cast<QQmlApplicationEngine*>(application_engine);
    ae->rootContext()->setContextProperty(s2qs(name), this);
}

void CProgSettings::setValue(const QString& SecAndKey, const QVariant& Value)
{
    if (!m_settings())
        return;
    m_settings()->setValue(SecAndKey, Value);
}

QVariant CProgSettings::value(const QString& SecAndKey, const QVariant& Default) const
{
    if (!m_settings())
        return {};
    return m_settings()->value(SecAndKey, Default);
}

void CProgSettings::flush()
{
    Sync();
}

void CProgSettings::Sync()
{
    if (!m_settings())
        return;
    m_settings()->sync();
    GetError();
}

QString CProgSettings::CreateQtKeyName(const std::string& SectionName, const std::string& KeyName)
{
    return s2qs(SectionName + "/" + KeyName);
}

void CProgSettings::Remove(const std::string& SectionName, const std::string& KeyName)
{
    if (!m_settings())
        return;
    m_settings()->remove(CreateQtKeyName(SectionName, KeyName));
    // here writing can occur asynchronously, so GetError() doesn't make sense before a call to Sync()
}

bool CProgSettings::Contains(const std::string& SectionName, const std::string& KeyName) const
{
    if (!m_settings())
        return false;
    bool ret = m_settings()->contains(CreateQtKeyName(SectionName, KeyName));
    GetError();
    return ret;
}

void CProgSettings::Clear()
{
    if (!m_settings())
        return;
    m_settings()->clear();
    // here writing can occur asynchronously, so GetError() doesn't make sense before a call to Sync()
}

void CProgSettings::enable(bool enable)
{
    this->enabled = enable;
}

std::vector<CProgSettings::TSectionKeyPair> CProgSettings::GetAllKeys() const
{
    std::vector<TSectionKeyPair> retkeys;
    if (!m_settings())
        return retkeys;
    QStringList keys(m_settings()->allKeys());
    retkeys.reserve(static_cast<size_t>(keys.size()));
    for (const auto& i : keys)
    {
        std::vector<std::string> keypath;
        too::str::tokenizeString(qs2s(i), "/", keypath);
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

class Convert_var2qvar_visitor : public boost::static_visitor<QVariant>
{
public:
    // template version leads to a bunch of compile errors; didn't think about it much, since the variant below worked
    //    template<typename T>
    //    QVariant operator()(const T& v) const
    //    {
    //        if (std::is_same<T,std::string>::value)
    //            return QVariant(s2qs(v));
    //        else
    //            return QVariant(v);
    //    }
    QVariant operator()(const CProgSettings::TInteger& v) const
    {
        return {v};
    }
    QVariant operator()(const double& v) const
    {
        return {v};
    }
    QVariant operator()(const std::string& v) const
    {
        return QVariant(s2qs(v));
    }
    QVariant operator()(const bool& v) const
    {
        return {v};
    }
};

QVariant CProgSettings::var2qvar(const CProgSettings::TVariant& v)
{
    return boost::apply_visitor(Convert_var2qvar_visitor(), v);
}

CProgSettings::TVariant CProgSettings::qvar2var(const QVariant& v) const
{
    bool isOK = true;
    TVariant ret;
    switch (v.type())
    {
        case QVariant::Int:
            ret = v.toInt(&isOK);
            break;
        case QVariant::Double:
            ret = v.toDouble(&isOK);
            break;
        case QVariant::String:
            ret = qs2s(v.toString());
            break;
        case QVariant::Bool:
            ret = v.toBool();
            break;
        default:
            isOK = false;
            break;
    }
    if (!isOK)
        SetError(EError::INTERNAL_ERROR__VARIANT_CONVERSION);
    return ret;
}

CProgSettings::TVariant CProgSettings::Value(
    const std::string& SectionName, const std::string& KeyName, const CProgSettings::TVariant& Default) const
{
    if (!m_settings())
        return {};
    QVariant val(m_settings()->value(CreateQtKeyName(SectionName, KeyName), var2qvar(Default)));
    GetError();
    if (!val.isValid() || val.isNull())
        SetError(EError::ERROR_READING_SETTINGS);
    return qvar2var(val);
}

void CProgSettings::SetValue(
    const std::string& SectionName, const std::string& KeyName, const CProgSettings::TVariant& Value)
{
    if (!m_settings())
        return;
    m_settings()->setValue(CreateQtKeyName(SectionName, KeyName), var2qvar(Value));
    // here writing can occur asynchronously, so GetError() doesn't make sense before a call to Sync()
}

std::string CProgSettings::ValueStr(
    const std::string& SectionName, const std::string& KeyName, const std::string& Default) const
{
    if (!m_settings())
        return {};
    QString val{m_settings()->value(CreateQtKeyName(SectionName, KeyName), QVariant{s2qs(Default)}).toString()};
    GetError();
    return qs2s(val);
}

void CProgSettings::SetValueStr(const std::string& SectionName, const std::string& KeyName, const std::string& Value)
{
    if (!m_settings())
        return;
    m_settings()->setValue(CreateQtKeyName(SectionName, KeyName), s2qs(Value));
}
} // namespace mb::uiw
