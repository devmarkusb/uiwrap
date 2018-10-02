// Markus Borris, 2015-17
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef MESSAGEBOX_QT_H_sidxgezwgfw76rn3x16
#define MESSAGEBOX_QT_H_sidxgezwgfw76rn3x16
#include "uiwrap_build_config.h"
#ifdef UIW_LINKLIB_IMPL_CHOICE_QT

#include "uiwrap/messagebox/messagebox.h"
#include "toolib/warnings.h"
TOO_PRAGMA_WARNINGS_PUSH_DISABLE_ALL
#include <QMessageBox>
TOO_PRAGMA_WARNINGS_POP


namespace uiw
{
namespace implQt
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

    QMessageBox::StandardButton EButton2StandardButton(EButton b);
    EButton StandardButton2EButton(QMessageBox::StandardButton qb);
};
}
}

#endif // UIW_LINKLIB_IMPL_CHOICE_QT
#endif
