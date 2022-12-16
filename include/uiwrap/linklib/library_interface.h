// Markus Borris, 2015
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef LIBRARY_INTERFACE_H_INCL_siuhxg378tzn3263
#define LIBRARY_INTERFACE_H_INCL_siuhxg378tzn3263

#include "uiwrapDEF.h"
#include "ul/ul.h"
#include <memory>
#include <string>


namespace mb::uiw
{
class UIWRAPSHARED_EXPORT ILibrary : private ul::NonCopyable
{
public:
    virtual ~ILibrary() = default;

    //! \param FilePathNameWithoutExtension without extension
    static std::unique_ptr<ILibrary> make(
        const std::string& FilePathNameWithoutExtension = std::string(), const std::string& Version = std::string());

    virtual void* ResolveSymbol(std::string Symbol) = 0;
    virtual void SetFileName(std::string FilePathNameWithoutExtension, std::string Version = std::string()) = 0;
    virtual std::string GetFileName() const = 0;
    virtual bool Load() = 0;
    virtual bool Unload() = 0;
    virtual std::string GetError() const = 0;
};

//! Still too abstract, not a real implementation yet.
class CLibrary : public ILibrary
{
public:
    explicit CLibrary(std::string FilePathNameWithoutExtension = std::string(), std::string Version = std::string())
        : m_FilePathName(FilePathNameWithoutExtension), m_Version(Version)
    {
        // would call Load here, if FilePathNameWithoutExtension is non-empty, but calling a virtual function
        // from a constructor would always only call the function of the base/current class
    }

    virtual void* ResolveSymbol(std::string Symbol) = 0;

    virtual void SetFileName(std::string FilePathNameWithoutExtension, std::string Version = std::string())
    {
        m_FilePathName = FilePathNameWithoutExtension;
        m_Version      = Version;
    }

    virtual std::string GetFileName() const { return m_FilePathName; }

    virtual bool Load() = 0;
    virtual bool Unload() = 0;

    virtual std::string GetError() const { return m_Error; }

protected:
    virtual void SetError(std::string Error) { m_Error = Error; }
    std::string GetVersion() const { return m_Version; }

private:
    std::string m_FilePathName;
    std::string m_Version;
    std::string m_Error;
};
}

#endif
