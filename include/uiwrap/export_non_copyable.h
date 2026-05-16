//! \file DLL-exported non-copyable base for uiwrap interface classes (MSVC C4275).

#ifndef UIWRAP_EXPORT_NON_COPYABLE_H_8f3k2m9q
#define UIWRAP_EXPORT_NON_COPYABLE_H_8f3k2m9q

#include "uiwrapDEF.h"

namespace mb::uiw {

class UIWRAPSHARED_EXPORT ExportNonCopyable {
protected:
    ExportNonCopyable() = default;
    ~ExportNonCopyable() = default;
    ExportNonCopyable(const ExportNonCopyable&) = delete;
    ExportNonCopyable& operator=(const ExportNonCopyable&) = delete;
    ExportNonCopyable(ExportNonCopyable&&) = delete;
    ExportNonCopyable& operator=(ExportNonCopyable&&) = delete;
};

} // namespace mb::uiw

#endif
