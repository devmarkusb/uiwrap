// Markus Borris, 2015-17
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef FILESYS_QT_H_INCL_sidnfgzgxfw746rn67gfqxw3gxfb7gwx
#define FILESYS_QT_H_INCL_sidnfgzgxfw746rn67gfqxw3gxfb7gwx
#include "uiwrap_build_config.h"
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
    virtual bool saveToTextFile(const std::string& filePathNameExt, const std::string& content) override;
    virtual bool loadFromTextFile(const std::string& filePathNameExt, std::string& content) const override;
    virtual bool copyFile(const std::string& filePathNameExt_From, const std::string& filePathNameExt_To) override;
    virtual bool deleteFile(const std::string& filePathNameExt) override;
    virtual bool renameFile(const std::string& filePathNameExt_From, const std::string& filePathNameExt_To) override;
    virtual bool createFolder(const std::string& folderPath) override;
    virtual bool deleteFolder(const std::string& folderPath) override;
    virtual bool renameFolder(const std::string& folderPath_From, const std::string& folderPath_To) override;
    virtual bool folderExists(const std::string& folderPath) const override;
    virtual bool fileExists(const std::string& filePathNameExt) const override;
    virtual bool isFile(const std::string& filePathNameExt) const override;

    virtual std::string toNativeSeparators(const std::string& Path) const override;

    virtual bool getSystemPath(ESysPathType Type, std::string& Path, bool WithTrailingSeparator) const override;

    virtual std::string getErrorOfLatestCall() const override;

private:
    mutable std::string latestError;

    void setFileOpErrorStr(const QFile& f, const std::string& op, const std::string& info = {}) const;
};
} // implQt
} // uiw

#endif // UIW_LINKLIB_IMPL_CHOICE_QT
#endif
