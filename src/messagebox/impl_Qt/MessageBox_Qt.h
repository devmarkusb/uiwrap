// 2015-17

//! \file

#ifndef MESSAGEBOX_QT_H_sidxgezwgfw76rn3x16
#define MESSAGEBOX_QT_H_sidxgezwgfw76rn3x16
#include "uiwrap_build_config.h"
#ifdef UIW_LINKLIB_IMPL_CHOICE_QT

#include "uiwrap/messagebox/messagebox.h"
#include "ul/ul.h"
UL_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC
#include <QMessageBox>
UL_PRAGMA_WARNINGS_POP


namespace mb::uiw::implQt
{
class CMessageBox_Qt : public uiw::gui::IMessageBox
{
public:
    ~CMessageBox_Qt() override = default;

    void addButton(EButton b) override;
    void setDefaultButton(EButton b) override;
    void setEscapeButton(EButton b) override;
    void setWindowTitle(const std::string& s) override;
    void setCaption(const std::string& s) override;
    void setText(const std::string& s) override;
    void setDetailedText(const std::string& s) override;
    void setIcon(EIcon i) override;

    void runModal() override;

    EButton getRunResult() override;

private:
    QMessageBox m_MsgBox{};
    QMessageBox::StandardButton m_RetVal{QMessageBox::NoButton};

    static QMessageBox::StandardButton EButton2StandardButton(EButton b);
    EButton StandardButton2EButton(QMessageBox::StandardButton qb);
};
} // namespace mb::uiw::implQt

#endif // UIW_LINKLIB_IMPL_CHOICE_QT
#endif
