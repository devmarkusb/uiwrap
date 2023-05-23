// 2016

//! \file

#ifndef MESSAGEBOX_H_skdljfh7ex5n7xt3
#define MESSAGEBOX_H_skdljfh7ex5n7xt3

#include "uiwrap/messagebox/messagebox.h"
#include "ul/ul.h"

namespace mb::uiw::impl {
class MessageBox : public uiw::gui::IMessageBox {
public:
    ~MessageBox() override = default;

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
    ul::Opt<EButton> retVal;
};
} // namespace mb::uiw::impl

#endif
