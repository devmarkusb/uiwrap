// Markus Borris, 2015
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef RESOURCE_STRING_H_sjzbxgf3uin4fg2n97ytgfvn32y
#define RESOURCE_STRING_H_sjzbxgf3uin4fg2n97ytgfvn32y

#include <string>
#include "resources.h"

namespace uiw
{
namespace gui
{
class IResourceString
{
public:
    virtual ~IResourceString() {}

    virtual std::string GetString(TResID id) const = 0;
};
}
}

#endif
