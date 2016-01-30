// Markus Borris, 2015
// This file is part of my uiwrap library.

//!
/** Please note: class CProgSettings is not suitable to be put in a lib; we get linker warnings due to moc content.
*/
//! \file

#ifndef PROGSETTINGS_QT_H_odzhf837t738tnx4387t34t3
#define PROGSETTINGS_QT_H_odzhf837t738tnx4387t34t3

#include "uiwrap/programsettings/programsettings.h"
#include <QObject>
#include <QSettings>
#include <memory>
#include "Toolib/class/non_copyable.h"


namespace uiw
{
namespace implQt
{
// do not put in a library, see comment at file start
class CProgSettings : public QObject, public uiw::IProgSettings
{
    Q_OBJECT

public:
    virtual ~CProgSettings() = default;

    virtual void Init(const std::string& OrganizationName, const std::string& ApplicationName);

    virtual void SetValue(const std::string& SectionName, const std::string& KeyName, const TVariant& Value);
    virtual TVariant Value(
        const std::string& SectionName, const std::string& KeyName, const TVariant& Default = TVariant()) const;

    virtual std::vector<TSectionKeyPair> GetAllKeys() const;
    virtual void Clear();
    virtual bool Contains(const std::string& SectionName, const std::string& KeyName) const;
    virtual void Remove(const std::string& SectionName, const std::string& KeyName);
    virtual void Sync();

    virtual EError GetError() const;
    virtual void ResetError() override;

    virtual void setAsRootContextProperty(void* application_engine, const std::string& name) const override;

    // ### QML access ###
    //! \param SecAndKey contains section and key like "sectionname/keyname" separated by "/"
    Q_INVOKABLE void setValue(const QString& SecAndKey, const QVariant& Value);
    Q_INVOKABLE QVariant value(const QString& SecAndKey, const QVariant& Default = QVariant()) const;

private:
    inline std::unique_ptr<QSettings>& m_settings();
    inline const std::unique_ptr<QSettings>& m_settings() const;
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
};
}
}

#endif // PROGSETTINGS_QT_H
