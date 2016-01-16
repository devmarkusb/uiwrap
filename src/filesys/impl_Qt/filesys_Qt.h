// Markus Borris, 2015
// This file is part of my uiwrap library. Open source.

//!
/**
*/
//! \file

#ifndef FILESYS_QT_H_INCL_sidnfgzgxfw746rn67gfqxw3gxfb7gwx
#define FILESYS_QT_H_INCL_sidnfgzgxfw746rn67gfqxw3gxfb7gwx

#include "uiwrap/filesys/filesys.h"
#include "uiwrap/uiwrapDEF.h"


namespace uiw
{
namespace implQt
{

class UIWRAPSHARED_EXPORT CFileSys_Qt : public too::IFileSys
{
public:
    virtual bool SaveToTextFile(const std::string& FilePathNameExt, const std::string& Content);
    virtual bool LoadFromTextFile(const std::string& FilePathNameExt, std::string& Content);
    virtual bool CopyFile(const std::string& FilePathNameExt_From, const std::string& FilePathNameExt_To);
    virtual bool DeleteFile(const std::string& FilePathNameExt);
    virtual bool RenameFile(const std::string& FilePathNameExt_From, const std::string& FilePathNameExt_To);
    virtual bool CreateFolder(const std::string& FolderPath);
    virtual bool DeleteFolder(const std::string& FolderPath);
    virtual bool RenameFolder(const std::string& FolderPath_From, const std::string& FolderPath_To);
    virtual bool FolderExists(const std::string& FolderPath);
    virtual bool FileExists(const std::string& FilePathNameExt);

    virtual std::string toNativeSeparators(const std::string& Path);

    virtual bool GetSystemPath(ESysPathType Type, std::string& Path, bool WithTrailingSeperator);
};
}
}

#endif
