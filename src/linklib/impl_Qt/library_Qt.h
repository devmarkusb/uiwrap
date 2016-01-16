// Markus Borris, 2014
// This file is part of my uiwrap library. Open source.

//!
/**
*/
//! \file

#ifndef LIBRARY_QT_H_INCL_slkhgncioue8zn87tc3t2n87t
#define LIBRARY_QT_H_INCL_slkhgncioue8zn87tc3t2n87t

#include "uiwrap/linklib/library_interface.h"
#include <QLibrary>
#include <memory>
#include "Toolib/mem/make_unique.h"
#include "../../string/impl_Qt/StringConvert_Qt.h"


namespace uiw
{
namespace implQt
{
class CLibrary_Qt : public CLibrary
{
public:
    explicit CLibrary_Qt(std::string FilePathNameWithoutExtension = std::string(), std::string Version = std::string())
        : CLibrary(FilePathNameWithoutExtension, Version)
        , m_impl(toos2qs(FilePathNameWithoutExtension), toos2qs(Version))
    {
    }

    virtual void* ResolveSymbol(std::string Symbol) { return reinterpret_cast<void*>(m_impl.resolve(Symbol.c_str())); }

    virtual void SetFileName(std::string FilePathNameWithoutExtension, std::string Version = std::string())
    {
        m_impl.setFileNameAndVersion(toos2qs(FilePathNameWithoutExtension), toos2qs(Version));
    }

    virtual std::string GetFileName() const { return qs2toos(m_impl.fileName()); }

    virtual bool Load() { return m_impl.load(); }

    virtual bool Unload() { return m_impl.unload(); }

    virtual std::string GetError() const
    {
        std::string ret(qs2toos(m_impl.errorString()));
        // unfortunately this is the string Qt returns in case of no error
        if (ret == "Unknown error"))
            ret.clear();
        return ret;
    }

private:
    QLibrary m_impl;
};
}
}

#endif
