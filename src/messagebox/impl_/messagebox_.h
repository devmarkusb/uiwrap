// Markus Borris, 2016
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef MESSAGEBOX__H_skdljfh7ex5n7xt3
#define MESSAGEBOX__H_skdljfh7ex5n7xt3

#include "uiwrap/messagebox/messagebox.h"
#include "toolib/optional.h"


namespace uiw
{
namespace impl
{
class MessageBox : public uiw::gui::IMessageBox
{
public:
    virtual ~MessageBox() = default;

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
    too::opt<EButton> retVal;
};
}
}

#endif
