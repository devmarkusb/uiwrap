// 2014-17

//! \file

#ifndef LIBRARY_QT_H_INCL_slkhgncioue8zn87tc3t2n87t
#define LIBRARY_QT_H_INCL_slkhgncioue8zn87tc3t2n87t
#include "uiwrap_build_config.h"
#ifdef UIW_LINKLIB_IMPL_CHOICE_QT

#include "uiwrap/linklib/library_interface.h"
#include "uiwrap/string/impl_Qt/stringconvert_Qt.h"
#include "ul/ul.h"

#include "QLibrary"
#include <memory>


namespace mb::uiw::implQt
{
class CLibrary_Qt : public CLibrary
{
public:
    explicit CLibrary_Qt(const std::string& filePathNameWithoutExtension, const std::string& version)
        : CLibrary(filePathNameWithoutExtension, version)
        , m_impl(s2qs(filePathNameWithoutExtension), s2qs(version))
    {
    }

    void* ResolveSymbol(std::string symbol) override
    {
        return reinterpret_cast<void*>(m_impl.resolve(symbol.c_str()));
    }

    void SetFileName(std::string filePathNameWithoutExtension, std::string version) override
    {
        m_impl.setFileNameAndVersion(s2qs(filePathNameWithoutExtension), s2qs(version));
    }

    [[nodiscard]] std::string GetFileName() const override
    {
        return qs2s(m_impl.fileName());
    }

    bool Load() override
    {
        return m_impl.load();
    }

    bool Unload() override
    {
        return m_impl.unload();
    }

    [[nodiscard]] std::string GetError() const override
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
} // namespace mb::uiw::implQt

#endif // UIW_LINKLIB_IMPL_CHOICE_QT
#endif
