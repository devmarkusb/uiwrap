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
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_BEGIN"
#include <QMessageBox>
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_END"


namespace uiw
{
namespace implQt
{

class CMessageBox_Qt : public uiw::gui::IMessageBox
{
public:
    virtual ~CMessageBox_Qt() = default;

    virtual void addButton(EButton b) override;
    virtual void setDefaultButton(EButton b) override;
    virtual void setEscapeButton(EButton b) override;
    virtual void setWindowTitle(const std::string& s) override;
    virtual void setCaption(const std::string& s) override;
    virtual void setText(const std::string& s) override;
    virtual void setDetailedText(const std::string& s) override;
    virtual void setIcon(EIcon i) override;

    virtual void runModal() override;

    virtual EButton getRunResult() override;

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
