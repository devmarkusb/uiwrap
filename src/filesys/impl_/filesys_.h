// 2015-18

//! \file

#ifndef FILESYS_H_dklzuix3xh8734xr387x2xt35g269x5tgf
#define FILESYS_H_dklzuix3xh8734xr387x2xt35g269x5tgf

#include "uiwrap/filesys/filesys.h"
#include "uiwrapDEF.h"
#include "ul/ul.h"
#include <string>


#if UL_OS_WINDOWS == 1
#undef CopyFile
#undef DeleteFile
#endif


namespace mb::uiw::impl
{
class UIWRAPSHARED_EXPORT CFileSys_ : public uiw::IFileSys
{
public:
    bool saveToTextFile(const std::string& filePathNameExt, const std::string& Content) override;
    bool loadFromTextFile(const std::string& filePathNameExt, std::string& Content) const override;
    bool copyFile(const std::string& filePathNameExt_From, const std::string& filePathNameExt_To) override;
    bool deleteFile(const std::string& filePathNameExt) override;
    bool renameFile(const std::string& filePathNameExt_From, const std::string& filePathNameExt_To) override;
    bool createFolder(const std::string& folderPath) override;
    bool deleteFolder(const std::string& folderPath) override;
    bool renameFolder(const std::string& folderPath_From, const std::string& folderPath_To) override;
    bool folderExists(const std::string& folderPath) const override;
    bool fileExists(const std::string& filePathNameExt) const override;
    bool isFile(const std::string& filePathNameExt) const override;

    std::string toNativeSeparators(const std::string& Path) const override;

    std::string getSystemPath(ESysPathType type, bool withTrailingSeparator) const override;

    std::string getErrorOfLatestCall() const override;

private:
    UL_PRAGMA_WARNINGS_PUSH
    UL_WARNING_DISABLE_MSVC(4251)
    mutable std::string latestError;
    UL_PRAGMA_WARNINGS_POP
};
} // namespace mb::uiw::impl

#endif
