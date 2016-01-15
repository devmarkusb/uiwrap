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
    virtual bool SaveToTextFile(const too::string& FilePathNameExt, const too::string& Content);
    virtual bool LoadFromTextFile(const too::string& FilePathNameExt, too::string& Content);
    virtual bool CopyFile(const too::string& FilePathNameExt_From, const too::string& FilePathNameExt_To);
    virtual bool DeleteFile(const too::string& FilePathNameExt);
    virtual bool RenameFile(const too::string& FilePathNameExt_From, const too::string& FilePathNameExt_To);
    virtual bool CreateFolder(const too::string& FolderPath);
    virtual bool DeleteFolder(const too::string& FolderPath);
    virtual bool RenameFolder(const too::string& FolderPath_From, const too::string& FolderPath_To);
    virtual bool FolderExists(const too::string& FolderPath);
    virtual bool FileExists(const too::string& FilePathNameExt);

    virtual too::string toNativeSeparators(const too::string& Path);

    virtual bool GetSystemPath(ESysPathType Type, too::string& Path, bool WithTrailingSeperator);
};
}
}

#endif
