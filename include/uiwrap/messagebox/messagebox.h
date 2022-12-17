// 2015

//! \file

#ifndef MESSAGEBOX_H_ydugfbxuzgnfv8eqgwgw23d3
#define MESSAGEBOX_H_ydugfbxuzgnfv8eqgwgw23d3

#include "uiwrapDEF.h"
#include "ul/ul.h"
#include <memory>
#include <string>


namespace mb::uiw::gui
{
class UIWRAPSHARED_EXPORT IMessageBox : private ul::NonCopyable
{
public:
    virtual ~IMessageBox() = default;

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

    virtual void addButton(EButton b) = 0;
    virtual void setDefaultButton(EButton b) = 0;
    virtual void setEscapeButton(EButton b) = 0;
    virtual void setWindowTitle(const std::string& s) = 0;
    virtual void setCaption(const std::string& s) = 0;
    virtual void setText(const std::string& s) = 0;
    virtual void setDetailedText(const std::string& s) = 0;
    virtual void setIcon(EIcon i) = 0;

    virtual void runModal() = 0;

    virtual EButton getRunResult() = 0;
};
} // namespace mb::uiw

#endif
