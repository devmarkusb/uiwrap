#include "messagebox_.hpp"

namespace mb::uiw::impl {
void MessageBox::addButton(EButton) {
}

void MessageBox::setDefaultButton(EButton) {
}

void MessageBox::setEscapeButton(EButton) {
}

void MessageBox::setWindowTitle(const std::string&) {
}

void MessageBox::setCaption(const std::string&) {
}

void MessageBox::setText(const std::string&) {
}

void MessageBox::setDetailedText(const std::string&) {
}

void MessageBox::setIcon(EIcon) {
}

void MessageBox::runModal() {
}

auto MessageBox::getRunResult() -> EButton {
    if (!this->retVal)
        return EButton::ignore;
    return *this->retVal;
}
} // namespace mb::uiw::impl
