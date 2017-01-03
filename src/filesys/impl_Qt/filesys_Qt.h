// Markus Borris, 2015
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef FILESYS_QT_H_INCL_sidnfgzgxfw746rn67gfqxw3gxfb7gwx
#define FILESYS_QT_H_INCL_sidnfgzgxfw746rn67gfqxw3gxfb7gwx
#ifdef UIW_LINKLIB_IMPL_CHOICE_QT

#include "uiwrap/filesys/filesys.h"
#include "uiwrapDEF.h"

class QFile;


namespace uiw
{
namespace implQt
{

class UIWRAPSHARED_EXPORT CFileSys_Qt : public uiw::IFileSys
{
public:
    virtual bool SaveToTextFile(const std::string& filePathNameExt, const std::string& content);
    virtual bool LoadFromTextFile(const std::string& filePathNameExt, std::string& content) const;
    virtual bool CopyFile(const std::string& filePathNameExt_From, const std::string& filePathNameExt_To);
    virtual bool DeleteFile(const std::string& filePathNameExt);
    virtual bool RenameFile(const std::string& filePathNameExt_From, const std::string& filePathNameExt_To);
    virtual bool CreateFolder(const std::string& folderPath);
    virtual bool DeleteFolder(const std::string& folderPath);
    virtual bool RenameFolder(const std::string& folderPath_From, const std::string& folderPath_To);
    virtual bool FolderExists(const std::string& folderPath) const;
    virtual bool FileExists(const std::string& filePathNameExt) const;
    virtual bool isFile(const std::string& filePathNameExt) const;

    virtual std::string toNativeSeparators(const std::string& Path) const;

    virtual bool GetSystemPath(ESysPathType Type, std::string& Path, bool WithTrailingSeparator) const;

    virtual std::string getErrorOfLatestCall() const override;

private:
    mutable std::string latestError;

    void setFileOpErrorStr(const QFile& f, const std::string& op, const std::string& info = {}) const;
};
}
}

#endif // UIW_LINKLIB_IMPL_CHOICE_QT
#endif
