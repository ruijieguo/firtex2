#ifndef __FILEIMPLWIN32_H
#define __FILEIMPLWIN32_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Timestamp.h"

FX_NS_DEF(utility);

class FileImpl
{
public:
	typedef int64_t FileSizeImpl;

    static void transPath(tstring& path);    
    static bool isFile(const tstring& sPath) ;
	static bool isDirectory(const tstring& sPath) ;	
	static bool isLink(const tstring& sPath) ;
	static Timestamp created(const tstring& sPath) ;
	static Timestamp lastModified(const tstring& sPath) ;
	static void setLastModified(const tstring& sPath, const Timestamp& ts);
	static FileSizeImpl getSize(const tstring& sPath) ;
	static void setSize(const tstring& sPath, FileSizeImpl size);
    static void setWriteable(const tstring& sPath, bool flag);
	static void copyTo(const tstring& sPath, const tstring& sDestPath) ;
	static void renameTo(const tstring& sPath, const tstring& sDestPath);
	static void remove(const tstring& sPath);
	static bool createFile(const tstring& sPath);
	static bool createDirectory(const tstring& sPath);
	static void handleLastError(const tstring& sPath);
    static bool exists(const tstring& sPath);
private:
	friend class FileHandle;
};

FX_NS_END

#endif
