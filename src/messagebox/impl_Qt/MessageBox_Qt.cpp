// Markus Borris, 2015
// This file is part of my uiwrap library. Open source.

//!
/**
 */
//! \file

#include "MessageBox_Qt.h"
#include "uiwrap/string/impl_Qt/StringConvert_Qt.h"


namespace mb::uiw
{
namespace implQt
{
QMessageBox::StandardButton CMessageBox_Qt::EButton2StandardButton(EButton b)
{
    QMessageBox::StandardButton qb = QMessageBox::Ok;
    switch (b)
    {
        case EButton::cancel:
            qb = QMessageBox::Cancel;
            break;
        case EButton::yes:
            qb = QMessageBox::Yes;
            break;
        case EButton::yestoall:
            qb = QMessageBox::YesToAll;
            break;
        case EButton::no:
            qb = QMessageBox::No;
            break;
        case EButton::notoall:
            qb = QMessageBox::NoToAll;
            break;
        case EButton::close:
            qb = QMessageBox::Close;
            break;
        case EButton::ignore:
            qb = QMessageBox::Ignore;
            break;
        case EButton::retry:
            qb = QMessageBox::Retry;
            break;
        case EButton::open:
            qb = QMessageBox::Open;
            break;
        case EButton::save:
            qb = QMessageBox::Save;
            break;
        case EButton::saveall:
            qb = QMessageBox::SaveAll;
            break;
        case EButton::reset:
            qb = QMessageBox::Reset;
            break;
        case EButton::discard:
            qb = QMessageBox::Discard;
            break;
        case EButton::apply:
            qb = QMessageBox::Apply;
            break;
        case EButton::help:
            qb = QMessageBox::Help;
            break;
        case EButton::restoredefaults:
            qb = QMessageBox::RestoreDefaults;
            break;
        case EButton::abort:
            qb = QMessageBox::Abort;
            break;
        case EButton::ok:
        default:
            qb = QMessageBox::Ok;
    }
    return qb;
}

CMessageBox_Qt::EButton CMessageBox_Qt::StandardButton2EButton(QMessageBox::StandardButton qb)
{
    EButton b = EButton::ok;
    switch (qb)
    {
        case QMessageBox::Cancel:
            b = EButton::cancel;
            break;
        case QMessageBox::Yes:
            b = EButton::yes;
            break;
        case QMessageBox::YesToAll:
            b = EButton::yestoall;
            break;
        case QMessageBox::No:
            b = EButton::no;
            break;
        case QMessageBox::NoToAll:
            b = EButton::notoall;
            break;
        case QMessageBox::Close:
            b = EButton::close;
            break;
        case QMessageBox::Ignore:
            b = EButton::ignore;
            break;
        case QMessageBox::Retry:
            b = EButton::retry;
            break;
        case QMessageBox::Open:
            b = EButton::open;
            break;
        case QMessageBox::Save:
            b = EButton::save;
            break;
        case QMessageBox::SaveAll:
            b = EButton::saveall;
            break;
        case QMessageBox::Reset:
            b = EButton::reset;
            break;
        case QMessageBox::Discard:
            b = EButton::discard;
            break;
        case QMessageBox::Apply:
            b = EButton::apply;
            break;
        case QMessageBox::Help:
            b = EButton::help;
            break;
        case QMessageBox::RestoreDefaults:
            b = EButton::restoredefaults;
            break;
        case QMessageBox::Abort:
            b = EButton::abort;
            break;
        case QMessageBox::Ok:
        default:
            b = EButton::ok;
    }
    return b;
}

void CMessageBox_Qt::addButton(EButton b)
{
    m_MsgBox.addButton(EButton2StandardButton(b));
}

void CMessageBox_Qt::setDefaultButton(EButton b)
{
    m_MsgBox.setDefaultButton(EButton2StandardButton(b));
}

void CMessageBox_Qt::setEscapeButton(EButton b)
{
    m_MsgBox.setEscapeButton(EButton2StandardButton(b));
}

void CMessageBox_Qt::setWindowTitle(const std::string& s)
{
    m_MsgBox.setWindowTitle(s2qs(s));
}

void CMessageBox_Qt::setCaption(const std::string& s)
{
    m_MsgBox.setText(s2qs(s));
}

void CMessageBox_Qt::setText(const std::string& s)
{
    m_MsgBox.setInformativeText(s2qs(s));
}

void CMessageBox_Qt::setDetailedText(const std::string& s)
{
    m_MsgBox.setDetailedText(s2qs(s));
}

void CMessageBox_Qt::setIcon(EIcon i)
{
    QMessageBox::Icon qi = QMessageBox::NoIcon;
    switch (i)
    {
        case EIcon::information:
            qi = QMessageBox::Information;
            break;
        case EIcon::question:
            qi = QMessageBox::Question;
            break;
        case EIcon::warning:
            qi = QMessageBox::Warning;
            break;
        case EIcon::critical:
            qi = QMessageBox::Critical;
            break;
        case EIcon::none:
        default:
            qi = QMessageBox::NoIcon;
    }
    m_MsgBox.setIcon(qi);
}

void CMessageBox_Qt::runModal()
{
    m_RetVal = static_cast<QMessageBox::StandardButton>(m_MsgBox.exec());
}

auto CMessageBox_Qt::getRunResult() -> EButton
{
    return StandardButton2EButton(m_RetVal);
}
} // namespace implQt
} // namespace mb::uiw
