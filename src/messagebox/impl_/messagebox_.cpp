#include "messagebox_.hpp"

namespace mb::uiw::impl {
void MessageBox::addButton(EButton b) {
    ul::ignore_unused(b);
}

void MessageBox::setDefaultButton(EButton b) {
    ul::ignore_unused(b);
}

void MessageBox::setEscapeButton(EButton b) {
    ul::ignore_unused(b);
}

void MessageBox::setWindowTitle(const std::string& s) {
    ul::ignore_unused(s);
}

void MessageBox::setCaption(const std::string& s) {
    ul::ignore_unused(s);
}

void MessageBox::setText(const std::string& s) {
    ul::ignore_unused(s);
}

void MessageBox::setDetailedText(const std::string& s) {
    ul::ignore_unused(s);
}

void MessageBox::setIcon(EIcon i) {
    ul::ignore_unused(i);
}

void MessageBox::runModal() {
}

auto MessageBox::getRunResult() -> EButton {
    if (!this->retVal)
        return EButton::ignore;
    return *this->retVal;
}
} // namespace mb::uiw::impl
