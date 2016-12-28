// Markus Borris, 2015
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef LIBRARY__H_INCL_kdnlfglknhcguewhgureihxrexq74
#define LIBRARY__H_INCL_kdnlfglknhcguewhgureihxrexq74

#include "uiwrap/linklib/library_interface.h"
#include "Toolib/PPDEFS.h"
#include "Toolib/std/std_extensions.h"
#include <string>
#if TOO_OS_WINDOWS == 1
#include "Toolib/string/lex_cast.h"
#include "Toolib/string/str_convert.h"
#include <sstream>
#include <windows.h>
#elif TOO_OS_LINUX == 1
#include <dlfcn.h>
#endif


namespace uiw
{
#if TOO_OS_WINDOWS == 1
class CLibrary_win : public CLibrary
{
public:
    explicit CLibrary_win(std::string FilePathNameWithoutExtension = std::string(), std::string Version = std::string())
        : CLibrary(FilePathNameWithoutExtension, Version)
    {
    }

    virtual void* ResolveSymbol(std::string Symbol)
    {
        if (!m_DllHandle)
        {
            SetError("Library not loaded.");
            return nullptr;
        }
        // LPCSTR always const char*
        FARPROC ret = GetProcAddress(m_DllHandle, Symbol.c_str());
        if (!ret)
        {
            std::stringstream ssErr;
            ssErr << "GetProcAddress " << Symbol << " failed: ";
            ssErr << too::lex_cast<std::string>(GetLastError());
            SetError(ssErr.str());
        }
        return reinterpret_cast<void*>(ret);
    }

    virtual bool Load()
    {
        std::string dll(GetFileName());
        dll += ".dll";
        // LPCTSTR is const wchar_t* and UTF16 assuming Windows-Unicode
        m_DllHandle = LoadLibrary(too::str::utf8to16_s2ws(dll).c_str());
        if (!m_DllHandle)
        {
            std::stringstream ssErr;
            ssErr << "LoadLibrary " << dll << " failed: ";
            ssErr << too::lex_cast<std::string>(GetLastError());
            SetError(ssErr.str());
        }
        return m_DllHandle ? true : false;
    }

    virtual bool Unload()
    {
        bool ret = FreeLibrary(m_DllHandle) ? true : false;
        if (!ret)
        {
            std::stringstream ssErr;
            ssErr << "FreeLibrary " << GetFileName() << " failed: ";
            ssErr << too::lex_cast<std::string, DWORD>(GetLastError());
            SetError(ssErr.str());
        }
        return ret;
    }

private:
    HMODULE m_DllHandle{nullptr};
};
#elif TOO_OS_LINUX == 1
class CLibrary_linux : public CLibrary
{
public:
    explicit CLibrary_linux(
        std::string FilePathNameWithoutExtension = std::string(), std::string Version = std::string())
        : CLibrary(FilePathNameWithoutExtension, Version)
    {
    }

    virtual void* ResolveSymbol(std::string Symbol)
    {
        if (!m_DllHandle)
        {
            SetError("Library not loaded.");
            return nullptr;
        }
        return dlsym(m_DllHandle, Symbol.c_str());
    }

    virtual bool Load()
    {
        std::string dll(GetFileName());
        dll += ".so";
        m_DllHandle = dlopen(dll.c_str(), OPEN_MODE);
        return m_DllHandle;
    }

    virtual bool Unload() { return dlclose(m_DllHandle); }

private:
    void* m_DllHandle{nullptr};
    static const int OPEN_MODE = 2;
};
#endif
}

#endif