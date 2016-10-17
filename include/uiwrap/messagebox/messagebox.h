// Markus Borris, 2015
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef MESSAGEBOX_H_ydugfbxuzgnfv8eqgwgw23d3
#define MESSAGEBOX_H_ydugfbxuzgnfv8eqgwgw23d3

#include "uiwrapDEF.h"
#include "Toolib/class/non_copyable.h"
#include <memory>
#include <string>


namespace uiw
{
namespace gui
{
class UIWRAPSHARED_EXPORT IMessageBox : private too::non_copyable
{
public:
    virtual ~IMessageBox() = default;

    // if you wonder, there is a reason for using lower-case here: winbase #defines "IGNORE", now that's weird...
    enum class EButton
    {
        ok,
        cancel,
        yes,
        yestoall,
        no,
        notoall,
        close,
        ignore,
        retry,
        open,
        save,
        saveall,
        reset,
        discard,
        undo = discard,
        apply,
        help,
        restoredefaults,
        abort,
    };

    enum class EIcon
    {
        none,
        information,
        question,
        warning,
        critical,
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
