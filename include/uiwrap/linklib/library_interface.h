// Markus Borris, 2015
// This file is part of my uiwrap library. Open source.

//!
/**
*/
//! \file

#ifndef LIBRARY_INTERFACE_H_INCL_siuhxg378tzn3263
#define LIBRARY_INTERFACE_H_INCL_siuhxg378tzn3263

#include <memory>
#include "Toolib/tooString.h"
#include "Toolib/class/non_copyable.h"
#include "uiwrapDEF.h"


namespace uiw
{
class UIWRAPSHARED_EXPORT ILibrary : private too::non_copyable
{
public:
    virtual ~ILibrary() = default;

    //! \param FilePathNameWithoutExtension without extension
    static std::unique_ptr<ILibrary> make(
        const too::string& FilePathNameWithoutExtension = too::string(), const too::string& Version = too::string());

    virtual void* ResolveSymbol(std::string Symbol) = 0;
    virtual void SetFileName(too::string FilePathNameWithoutExtension, too::string Version = too::string()) = 0;
    virtual too::string GetFileName() const = 0;
    virtual bool Load() = 0;
    virtual bool Unload() = 0;
    virtual too::string GetError() const = 0;
};

//! Still too abstract, not a real implementation yet.
class CLibrary : public ILibrary
{
public:
    explicit CLibrary(too::string FilePathNameWithoutExtension = too::string(), too::string Version = too::string())
        : m_FilePathName(FilePathNameWithoutExtension), m_Version(Version)
    {
        // would call Load here, if FilePathNameWithoutExtension is non-empty, but calling a virtual function
        // from a constructor would always only call the function of the base/current class
    }

    virtual void* ResolveSymbol(std::string Symbol) = 0;

    virtual void SetFileName(too::string FilePathNameWithoutExtension, too::string Version = too::string())
    {
        m_FilePathName = FilePathNameWithoutExtension;
        m_Version      = Version;
    }

    virtual too::string GetFileName() const { return m_FilePathName; }

    virtual bool Load() = 0;
    virtual bool Unload() = 0;

    virtual too::string GetError() const { return m_Error; }

protected:
    virtual void SetError(too::string Error) { m_Error = Error; }
    too::string GetVersion() const { return m_Version; }

private:
    too::string m_FilePathName;
    too::string m_Version;
    too::string m_Error;
};
}

#endif
