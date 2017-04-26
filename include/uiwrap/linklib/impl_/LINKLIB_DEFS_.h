// Markus Borris, 2015-17
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef LINKLIB_DEFS__H_INCL_sjdnxfzuergf3ui7yt83gzf7r87wr
#define LINKLIB_DEFS__H_INCL_sjdnxfzuergf3ui7yt83gzf7r87wr

#include "uiwrap_build_config.h"
#include "Toolib/PPDEFS.h"


#if TOO_OS_WINDOWS == 1
//! Perhaps UIW_LINKLIB_IMPL_CHOICE_OWN_CFG_STATICLIB has to be defined when you compile a library as static lib and
//! also when you use it.
#if !defined(UIW_LINKLIB_IMPL_CHOICE_OWN_CFG_STATICLIB)
#define UIW_DECL_EXPORT __declspec(dllexport)
#define UIW_DECL_IMPORT __declspec(dllimport)
#elif UIW_LINKLIB_IMPL_CHOICE_OWN_CFG_STATICLIB == 1
#define UIW_DECL_EXPORT
#define UIW_DECL_IMPORT
#endif
#elif TOO_OS_LINUX == 1
#define UIW_DECL_EXPORT
#define UIW_DECL_IMPORT
#else
// untested, don't think this alone works for all remaining platforms ;)
#define UIW_DECL_EXPORT
#define UIW_DECL_IMPORT
#endif


//! Declare the general dll calling convention.
#if defined(_STDCALL_SUPPORTED)
#define UIW_CALLINGCONVENTION __stdcall
#else
#define UIW_CALLINGCONVENTION __cdecl
#endif


//! Declare the dll calling convention for DllMain, Windows.
#if TOO_OS_WINDOWS == 1 && ((_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED))
#define UIW_WINAPI_DLLMAIN __stdcall
#else
#define UIW_WINAPI_DLLMAIN
#endif
#define UIW_APIENTRY WINAPI_DLLMAIN


#endif
