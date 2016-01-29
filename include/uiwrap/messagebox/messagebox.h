// Markus Borris, 2015
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef MESSAGEBOX_H_ydugfbxuzgnfv8eqgwgw23d3
#define MESSAGEBOX_H_ydugfbxuzgnfv8eqgwgw23d3

#include <memory>
#include <string>
#include "Toolib/class/non_copyable.h"
#include "uiwrapDEF.h"


namespace uiw
{
namespace gui
{
class UIWRAPSHARED_EXPORT IMessageBox : private too::non_copyable
{
public:
    virtual ~IMessageBox() = default;

    enum class EButton
    {
        OK,
        CANCEL,
        YES,
        YESTOALL,
        NO,
        NOTOALL,
        CLOSE,
        IGNORE,
        RETRY,
        OPEN,
        SAVE,
        SAVEALL,
        RESET,
        DISCARD,
        UNDO = DISCARD,
        APPLY,
        HELP,
        RESTOREDEFAULTS,
        ABORT,
    };

    enum class EIcon
    {
        NONE,
        INFORMATION,
        QUESTION,
        WARNING,
        CRITICAL,
    };

    static std::unique_ptr<IMessageBox> make();

    virtual void AddButton(EButton b) = 0;
    virtual void SetDefaultButton(EButton b) = 0;
    virtual void SetEscapeButton(EButton b) = 0;
    virtual void SetCaption(const std::string& s) = 0;
    virtual void SetText(const std::string& s) = 0;
    virtual void SetDetailedText(const std::string& s) = 0;
    virtual void SetIcon(EIcon i) = 0;

    virtual bool RunModal() = 0;

    virtual EButton GetRunResult() = 0;
};
}
}

#endif
