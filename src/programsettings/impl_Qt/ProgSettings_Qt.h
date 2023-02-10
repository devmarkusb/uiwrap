// 2015-17

/** \file
    Please note: class CProgSettings is not suitable to be put in a lib; we get linker warnings due to moc content.*/

#ifndef PROGSETTINGS_QT_H_odzhf837t738tnx4387t34t3
#define PROGSETTINGS_QT_H_odzhf837t738tnx4387t34t3

#include "uiwrap_build_config.h"

#ifdef UIW_LINKLIB_IMPL_CHOICE_QT
#include "uiwrap/programsettings/programsettings.h"
#include "ul/ul.h"
UL_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC
#include "QObject"
#include "QSettings"
#include "QString"
#include "QVariant"
UL_PRAGMA_WARNINGS_POP
#include <memory>

namespace mb::uiw::implQt
{
// do not put in a library, see comment at file start
class CProgSettings
    : public QObject
    , public uiw::IProgSettings
{
    Q_OBJECT

public:
    CProgSettings()
        : QObject()
    {
    }

    ~CProgSettings() override = default;

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

    void setAsRootContextProperty(void* application_engine, const std::string& name) override;

    // ### QML access ###
    //! \param secAndKey contains section and key like "sectionname/keyname" separated by "/"
    Q_INVOKABLE void setValue(const QString& secAndKey, const QVariant& value);
    Q_INVOKABLE QVariant value(const QString& secAndKey, const QVariant& def = QVariant()) const;
    Q_INVOKABLE void flush();

private:
    QSettings* m_settings();
    const QSettings* m_settings() const;
    mutable EError m_FirstOccurredError{EError::INIT_NOT_CALLED_OR_FAILED};

    void SetError(EError e) const
    {
        if (m_FirstOccurredError == EError::E_NO_ERROR)
            m_FirstOccurredError = e;
    }

    static QString CreateQtKeyName(const std::string& sectionName, const std::string& keyName);

private:
    std::unique_ptr<QSettings> m_settings_impl_doNotUseItDirectlyExceptOnInit{nullptr};
    bool enabled{true};
};
} // namespace mb::uiw::implQt

#endif // UIW_LINKLIB_IMPL_CHOICE_QT
#endif
