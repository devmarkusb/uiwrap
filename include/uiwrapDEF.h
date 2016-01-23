// Markus Borris, 2015
// This file is part of my uiwrap library.

//! File serves to export symbols for uiwrap as a library.
/** Note that it uses itself a little bit to do that.
*/
//! \file

#ifndef UIWRAPDEF_H_INCL_aibuxsgfnz342975ty31
#define UIWRAPDEF_H_INCL_aibuxsgfnz342975ty31

#include "uiwrap/linklib/LINKLIB_DEFS.h"

#if defined(UIWRAP_LIBRARY)
#define UIWRAPSHARED_EXPORT UIW_DECL_EXPORT
#else
#define UIWRAPSHARED_EXPORT UIW_DECL_IMPORT
#endif

#endif
