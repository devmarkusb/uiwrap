// Markus Borris, 2015
// This file is part of my uiwrap library. Open source.

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
class CProgSettings : public QObject, public uiw::IProgSettings, private too::non_copyable
{
    Q_OBJECT

public:
    virtual ~CProgSettings() = default;

    virtual void Init(const too::string& OrganizationName, const too::string& ApplicationName);

    virtual void SetValue(const too::string& SectionName, const too::string& KeyName, const TVariant& Value);
    virtual TVariant Value(const too::string& SectionName, const too::string& KeyName, const TVariant& Default = TVariant()) const;

    virtual std::vector<TSectionKeyPair> GetAllKeys() const;
    virtual void Clear();
    virtual bool Contains(const too::string& SectionName, const too::string& KeyName) const;
    virtual void Remove(const too::string& SectionName, const too::string& KeyName);
    virtual void Sync();

    virtual EError GetError() const;
    virtual void ResetError() override;

    // ### QML access ###
    //! \param SecAndKey contains section and key like "sectionname/keyname" separated by "/"
    Q_INVOKABLE void setValue(const QString& SecAndKey, const QVariant& Value);
    Q_INVOKABLE QVariant value(const QString& SecAndKey, const QVariant& Default = QVariant()) const;

private:
    inline std::unique_ptr<QSettings>&  m_settings();
    inline const std::unique_ptr<QSettings>& m_settings() const;
    mutable EError                      m_FirstOccurredError{EError::INIT_NOT_CALLED_OR_FAILED};

    void SetError(EError e) const { if (m_FirstOccurredError == EError::E_NO_ERROR) m_FirstOccurredError = e; }
    QString CreateQtKeyName(const too::string& SectionName, const too::string& KeyName) const;
    QVariant var2qvar(const TVariant& v) const;
    TVariant qvar2var(const QVariant& v) const;

private:
    std::unique_ptr<QSettings>          m_settings_impl_doNotUseItDirectlyExceptOnInit{nullptr};
};

}
}

#endif // PROGSETTINGS_QT_H
