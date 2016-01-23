// Markus Borris, 2015
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef MESSAGEBOX_QT_H_sidxgezwgfw76rn3x16
#define MESSAGEBOX_QT_H_sidxgezwgfw76rn3x16

#include <QMessageBox>
#include "uiwrap/messagebox/messagebox.h"
#include "Toolib/class/non_copyable.h"


namespace uiw
{
namespace implQt
{

class CMessageBox_Qt : public uiw::gui::IMessageBox, private too::non_copyable
{
public:
    virtual ~CMessageBox_Qt() = default;

    virtual void AddButton(EButton b);
    virtual void SetDefaultButton(EButton b);
    virtual void SetEscapeButton(EButton b);
    virtual void SetCaption(const std::string& s);
    virtual void SetText(const std::string& s);
    virtual void SetDetailedText(const std::string& s);
    virtual void SetIcon(EIcon i);

    virtual bool RunModal();

    virtual EButton GetRunResult();

private:
    QMessageBox m_MsgBox;
    QMessageBox::StandardButton m_RetVal{QMessageBox::NoButton};

    QMessageBox::StandardButton EButton2StandardButton(EButton b);
    EButton StandardButton2EButton(QMessageBox::StandardButton qb);
};
}
}

#endif
