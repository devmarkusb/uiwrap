//! \file

#ifndef LIBRARY_INTERFACE_H_siuhxg378tzn3263
#define LIBRARY_INTERFACE_H_siuhxg378tzn3263

#include "uiwrapDEF.h"
#include "../config.h"
#include "ul/ul.h"
#include <memory>
#include <string>
#include <utility>

namespace mb::uiw {
class UIWRAPSHARED_EXPORT ILibrary : private ul::NonCopyable {
public:
    virtual ~ILibrary() = default;

    //! \param filePathNameWithoutExtension without extension
    static std::unique_ptr<ILibrary> make(const std::string& filePathNameWithoutExtension, const std::string& version);

    virtual void* ResolveSymbol(std::string symbol) = 0;
    virtual void SetFileName(std::string filePathNameWithoutExtension, std::string version) = 0;
    [[nodiscard]] virtual std::string GetFileName() const = 0;
    virtual bool Load() = 0;
    virtual bool Unload() = 0;
    [[nodiscard]] virtual std::string GetError() const = 0;
};

//! Still too abstract, not a real implementation yet.
class CLibrary : public ILibrary {
public:
    explicit CLibrary(std::string filePathNameWithoutExtension, std::string version)
        : m_FilePathName(std::move(filePathNameWithoutExtension))
        , m_Version(std::move(version)) {
        // would call Load here, if FilePathNameWithoutExtension is non-empty, but calling a virtual function
        // from a constructor would always only call the function of the base/current class
    }

    void* ResolveSymbol(std::string symbol) override = 0;

    void SetFileName(std::string filePathNameWithoutExtension, std::string version) override {
        m_FilePathName = filePathNameWithoutExtension;
        m_Version = version;
    }

    [[nodiscard]] std::string GetFileName() const override {
        return m_FilePathName;
    }

    bool Load() override = 0;
    bool Unload() override = 0;

    [[nodiscard]] std::string GetError() const override {
        return m_Error;
    }

protected:
    virtual void SetError(std::string error) {
        m_Error = std::move(error);
    }

    [[nodiscard]] std::string GetVersion() const {
        return m_Version;
    }

private:
    std::string m_FilePathName;
    std::string m_Version;
    std::string m_Error;
};
} // namespace mb::uiw

UIW_HEADER_END

#endif
