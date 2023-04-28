//! \file

#ifndef RESOURCE_STRING_H_sjzbxgf3uin4fg2n97ytgfvn32y
#define RESOURCE_STRING_H_sjzbxgf3uin4fg2n97ytgfvn32y

#include "resources.h"
#include "../config.h"
#include <string>

namespace mb::uiw::gui {
class IResourceString {
public:
    virtual ~IResourceString() = default;

    [[nodiscard]] virtual std::string getString(ResourceID id) const = 0;
};
} // namespace mb::uiw::gui

UIW_HEADER_END

#endif
