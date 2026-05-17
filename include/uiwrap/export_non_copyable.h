//! \file DLL-exported non-copyable base for uiwrap interface classes (MSVC C4275).

#ifndef UIWRAP_EXPORT_NON_COPYABLE_H_8f3k2m9q
#define UIWRAP_EXPORT_NON_COPYABLE_H_8f3k2m9q

#include "uiwrapDEF.h"

namespace mb::uiw {
class UIWRAPSHARED_EXPORT ExportNonCopyable {
public:
    ExportNonCopyable(const ExportNonCopyable&) = delete;
    ExportNonCopyable& operator=(const ExportNonCopyable&) = delete;
    ExportNonCopyable(ExportNonCopyable&&) = delete;
    ExportNonCopyable& operator=(ExportNonCopyable&&) = delete;

protected:
    ExportNonCopyable() = default;
    ~ExportNonCopyable() = default;
};
} // namespace mb::uiw

#endif
