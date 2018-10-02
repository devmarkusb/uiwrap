// Markus Borris, 2015-17
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef PROGSETTINGS_QT_H_odzhf837t738tnx4387t34t3
#define PROGSETTINGS_QT_H_odzhf837t738tnx4387t34t3
#include "uiwrap_build_config.h"
#ifdef UIW_LINKLIB_IMPL_CHOICE_QT

#include "uiwrap/programsettings/programsettings.h"
#include "toolib/warnings.h"
TOO_PRAGMA_WARNINGS_PUSH_DISABLE_ALL
#include <QObject>
#include <QSettings>
#include <QString>
#include <QVariant>
TOO_PRAGMA_WARNINGS_POP
#include <memory>


namespace uiw
{
namespace implQt
{
// do not put in a library, see comment at file start
class CProgSettings : public QObject, public uiw::IProgSettings
{
    Q_OBJECT

public:
    CProgSettings() : QObject() {}
    ~CProgSettings() override = default;

    void Init(const std::string& OrganizationName, const std::string& ApplicationName) override;

    TVariant Value(const std::string& SectionName, const std::string& KeyName,
        const TVariant& Default = TVariant()) const override;
    void SetValue(const std::string& SectionName, const std::string& KeyName, const TVariant& Value) override;

    std::string ValueStr(
        const std::string& SectionName, const std::string& KeyName, const std::string& Default = {}) const override;
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

    void setAsRootContextProperty(void* application_engine, const std::string& name) override;

    // ### QML access ###
    //! \param SecAndKey contains section and key like "sectionname/keyname" separated by "/"
    Q_INVOKABLE void setValue(const QString& SecAndKey, const QVariant& Value);
    Q_INVOKABLE QVariant value(const QString& SecAndKey, const QVariant& Default = QVariant()) const;
    Q_INVOKABLE void flush();

private:
    inline QSettings* m_settings();
    inline const QSettings* m_settings() const;
    mutable EError m_FirstOccurredError{EError::INIT_NOT_CALLED_OR_FAILED};

    void SetError(EError e) const
    {
        if (m_FirstOccurredError == EError::E_NO_ERROR)
            m_FirstOccurredError = e;
    }
    QString CreateQtKeyName(const std::string& SectionName, const std::string& KeyName) const;
    QVariant var2qvar(const TVariant& v) const;
    TVariant qvar2var(const QVariant& v) const;

private:
    std::unique_ptr<QSettings> m_settings_impl_doNotUseItDirectlyExceptOnInit{nullptr};
    bool enabled{true};
};
}
}

#endif // UIW_LINKLIB_IMPL_CHOICE_QT
#endif
