// 2015

//!
/**
 */
//! \file

#ifndef RESOURCE_STRING_H_sjzbxgf3uin4fg2n97ytgfvn32y
#define RESOURCE_STRING_H_sjzbxgf3uin4fg2n97ytgfvn32y

#include "resources.h"
#include <string>


namespace mb::uiw
{
namespace gui
{
class IResourceString
{
public:
    virtual ~IResourceString()
    {
    }

    virtual std::string GetString(ResourceID id) const = 0;
};
} // namespace gui
} // namespace mb::uiw

#endif
