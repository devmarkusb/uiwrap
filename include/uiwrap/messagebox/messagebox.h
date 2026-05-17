//! \file

#ifndef MESSAGEBOX_H_ydugfbxuzgnfv8eqgwgw23d3
#define MESSAGEBOX_H_ydugfbxuzgnfv8eqgwgw23d3

#include "uiwrapDEF.h"
#include "../config.h"
#include "../export_non_copyable.h"
#include "mb/ul/ul.hpp"
#include <cstdint>
#include <memory>
#include <string>

namespace mb::uiw::gui {
class UIWRAPSHARED_EXPORT IMessageBox : private ExportNonCopyable {
public:
    virtual ~IMessageBox() = default;

    enum class EButton : std::uint8_t {
        ok = 0,
        cancel = 1,
        yes = 2,
        yestoall = 3,
        no = 4,
        notoall = 5,
        close = 6,
        ignore = 7,
        retry = 8,
        open = 9,
        save = 10,
        saveall = 11,
        reset = 12,
        discard = 13,
        undo = discard,
        apply = 14,
        help = 15,
        restoredefaults = 16,
        abort = 17,
    };

    enum class EIcon : std::uint8_t {
        none = 0,
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
} // namespace mb::uiw::gui

UIW_HEADER_END

#endif
