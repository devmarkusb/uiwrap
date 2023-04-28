// 2015-17

//! \file

#ifndef LIBRARY_H_kdnlfglknhcguewhgureihxrexq74
#define LIBRARY_H_kdnlfglknhcguewhgureihxrexq74

#include "uiwrap/linklib/library_interface.h"
#include "ul/ul.h"
#include <string>

#if UL_OS_WINDOWS == 1
#include <sstream>
#include <windows.h>
#elif UL_OS_LINUX == 1
#include <dlfcn.h>
#endif


namespace mb::uiw {
#if UL_OS_WINDOWS == 1
class CLibrary_win : public CLibrary {
public:
    explicit CLibrary_win(
        const std::string& FilePathNameWithoutExtension = std::string(), const std::string& Version = std::string())
        : CLibrary(FilePathNameWithoutExtension, Version) {
    }

    virtual void* ResolveSymbol(std::string Symbol) {
        if (!m_DllHandle) {
            SetError("Library not loaded.");
            return nullptr;
        }
        // LPCSTR always const char*
        FARPROC ret = GetProcAddress(m_DllHandle, Symbol.c_str());
        if (!ret) {
            std::stringstream ssErr;
            ssErr << "GetProcAddress " << Symbol << " failed: ";
            ssErr << ul::lex_cast<std::string>(GetLastError());
            SetError(ssErr.str());
        }
        return reinterpret_cast<void*>(ret);
    }

    virtual bool Load() {
        std::string dll(GetFileName());
        dll += ".dll";
        // LPCTSTR is const wchar_t* and UTF16 assuming Windows-Unicode
        m_DllHandle = LoadLibrary(ul::str::utf8to16_s2ws(dll).c_str());
        if (!m_DllHandle) {
            std::stringstream ssErr;
            ssErr << "LoadLibrary " << dll << " failed: ";
            ssErr << ul::lex_cast<std::string>(GetLastError());
            SetError(ssErr.str());
        }
        return m_DllHandle ? true : false;
    }

    virtual bool Unload() {
        bool ret = FreeLibrary(m_DllHandle) ? true : false;
        if (!ret) {
            std::stringstream ssErr;
            ssErr << "FreeLibrary " << GetFileName() << " failed: ";
            ssErr << ul::lex_cast<std::string, DWORD>(GetLastError());
            SetError(ssErr.str());
        }
        return ret;
    }

private:
    HMODULE m_DllHandle{nullptr};
};
#elif UL_OS_LINUX == 1
class CLibrary_linux : public CLibrary {
public:
    explicit CLibrary_linux(
        const std::string& FilePathNameWithoutExtension = std::string(), const std::string& Version = std::string())
        : CLibrary(FilePathNameWithoutExtension, Version) {
    }

    void* ResolveSymbol(std::string Symbol) override {
        if (!m_DllHandle) {
            SetError("Library not loaded.");
            return nullptr;
        }
        return dlsym(m_DllHandle, Symbol.c_str());
    }

    bool Load() override {
        std::string dll(GetFileName());
        dll += ".so";
        m_DllHandle = dlopen(dll.c_str(), OPEN_MODE);
        return m_DllHandle;
    }

    bool Unload() override {
        return dlclose(m_DllHandle);
    }

private:
    void* m_DllHandle{nullptr};
    static const int OPEN_MODE = 2;
};
#endif
} // namespace mb::uiw

#endif
