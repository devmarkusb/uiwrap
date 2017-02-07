// Markus Borris, 2014-16
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef LIBRARY_QT_H_INCL_slkhgncioue8zn87tc3t2n87t
#define LIBRARY_QT_H_INCL_slkhgncioue8zn87tc3t2n87t
#ifdef UIW_LINKLIB_IMPL_CHOICE_QT

#include "uiwrap/linklib/library_interface.h"
#include "uiwrap/string/impl_Qt/StringConvert_Qt.h"
#include "Toolib/std/std_extensions.h"
#include <QLibrary>
#include <memory>


namespace uiw
{
namespace implQt
{

class CLibrary_Qt : public CLibrary
{
public:
    explicit CLibrary_Qt(std::string FilePathNameWithoutExtension = std::string(), std::string Version = std::string())
        : CLibrary(FilePathNameWithoutExtension, Version), m_impl(s2qs(FilePathNameWithoutExtension), s2qs(Version))
    {
    }

    virtual void* ResolveSymbol(std::string Symbol) { return reinterpret_cast<void*>(m_impl.resolve(Symbol.c_str())); }

    virtual void SetFileName(std::string FilePathNameWithoutExtension, std::string Version = std::string())
    {
        m_impl.setFileNameAndVersion(s2qs(FilePathNameWithoutExtension), s2qs(Version));
    }

    virtual std::string GetFileName() const { return qs2s(m_impl.fileName()); }

    virtual bool Load() { return m_impl.load(); }

    virtual bool Unload() { return m_impl.unload(); }

    virtual std::string GetError() const
    {
        std::string ret(qs2s(m_impl.errorString()));
        // unfortunately this is the string Qt returns in case of no error
        if (ret == "Unknown error")
            ret.clear();
        return ret;
    }

private:
    QLibrary m_impl;
};
}
}

#endif // UIW_LINKLIB_IMPL_CHOICE_QT
#endif
