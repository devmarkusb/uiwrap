// Markus Borris, 2015-16
// This file is part of my uiwrap library. Open source.

//!
/** Please note: class CProgSettings is not suitable to be put in a lib; we get linker warnings due to moc content.
*/
//! \file

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "ProgSettings_Qt.h"
#include "uiwrap/string/impl_Qt/StringConvert_Qt.h"
#include "Toolib/string/string_token.h"
#include "Toolib/std/std_extensions.h"
#include "Toolib/assert.h"


namespace uiw
{
namespace implQt
{

inline QSettings* CProgSettings::m_settings()
{
    TOO_ASSERT(m_settings_impl_doNotUseItDirectlyExceptOnInit);
    if (!m_settings_impl_doNotUseItDirectlyExceptOnInit)
        SetError(EError::INIT_NOT_CALLED_OR_FAILED);
    return m_settings_impl_doNotUseItDirectlyExceptOnInit.get();
}

inline const QSettings* CProgSettings::m_settings() const
{
    TOO_ASSERT(m_settings_impl_doNotUseItDirectlyExceptOnInit);
    if (!m_settings_impl_doNotUseItDirectlyExceptOnInit)
        SetError(EError::INIT_NOT_CALLED_OR_FAILED);
    return m_settings_impl_doNotUseItDirectlyExceptOnInit.get();
}

void CProgSettings::Init(const std::string& OrganizationName, const std::string& ApplicationName)
{
    m_settings_impl_doNotUseItDirectlyExceptOnInit = too::make_unique<QSettings>(
        QSettings::IniFormat, QSettings::UserScope, s2qs(OrganizationName), s2qs(ApplicationName));
    auto ok = m_settings_impl_doNotUseItDirectlyExceptOnInit->status();
    TOO_ASSERT(ok == QSettings::NoError);
    too::ignore_arg(ok);
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
    TOO_EXPECT_THROW(application_engine);
    if (!m_settings())
        return;
    QQmlApplicationEngine* ae = reinterpret_cast<QQmlApplicationEngine*>(application_engine);
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
        return QVariant();
    return m_settings()->value(SecAndKey, Default);
}

void CProgSettings::Sync()
{
    if (!m_settings())
        return;
    m_settings()->sync();
    GetError();
}

QString CProgSettings::CreateQtKeyName(const std::string& SectionName, const std::string& KeyName) const
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

std::vector<CProgSettings::TSectionKeyPair> CProgSettings::GetAllKeys() const
{
    std::vector<TSectionKeyPair> retkeys;
    if (!m_settings())
        return retkeys;
    QStringList keys(m_settings()->allKeys());
    retkeys.reserve(keys.size());
    for (auto i : keys)
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
    QVariant operator()(const CProgSettings::TInteger& v) const { return QVariant(v); }
    QVariant operator()(const double& v) const { return QVariant(v); }
    QVariant operator()(const std::string& v) const { return QVariant(s2qs(v)); }
    QVariant operator()(const bool& v) const { return QVariant(v); }
};

QVariant CProgSettings::var2qvar(const CProgSettings::TVariant& v) const
{
    return boost::apply_visitor(Convert_var2qvar_visitor(), v);
}

CProgSettings::TVariant CProgSettings::qvar2var(const QVariant& v) const
{
    bool isOK = true;
    TVariant ret;
    switch (v.type())
    {
    case QMetaType::Int:
        ret = v.toInt(&isOK);
        break;
    case QMetaType::Double:
        ret = v.toDouble(&isOK);
        break;
    case QMetaType::QString:
        ret = qs2s(v.toString());
        break;
    case QMetaType::Bool:
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
        return TVariant();
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
}
}
