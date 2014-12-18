#define _WIN32_WINNT		0x0500  
#define REPARSE_MOUNTPOINT_HEADER_SIZE   8

#include <windows.h>  
#include <WINIOCTL.H>
typedef struct {
  DWORD ReparseTag;
  DWORD ReparseDataLength;
  WORD Reserved;
  WORD ReparseTargetLength;
  WORD ReparseTargetMaximumLength;
  WORD Reserved1;
  WCHAR ReparseTarget[1];
} REPARSE_MOUNTPOINT_DATA_BUFFER, *PREPARSE_MOUNTPOINT_DATA_BUFFER;
 
extern "C" __declspec(dllexport) int CreateJunction(LPCSTR szJunction, LPCSTR szTarget) { 
  int buflen = sizeof(REPARSE_MOUNTPOINT_DATA_BUFFER) + MAX_PATH * sizeof(WCHAR);
  REPARSE_MOUNTPOINT_DATA_BUFFER* pReparseBuffer = (PREPARSE_MOUNTPOINT_DATA_BUFFER)malloc( buflen );
  memset(pReparseBuffer, 0, buflen); 
  
  HANDLE hDir = CreateFile(szJunction, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS, NULL);
  if (hDir == INVALID_HANDLE_VALUE) return -1;
  
  pReparseBuffer->ReparseTag = IO_REPARSE_TAG_MOUNT_POINT;
  int len = MultiByteToWideChar(CP_ACP, 0, szTarget, -1, pReparseBuffer->ReparseTarget, MAX_PATH);
  pReparseBuffer->ReparseTargetMaximumLength = (len--) * sizeof(WCHAR);
  pReparseBuffer->ReparseTargetLength = len * sizeof(WCHAR);
  pReparseBuffer->ReparseDataLength = pReparseBuffer->ReparseTargetLength + 12;

  DWORD dwRet;
  if (!DeviceIoControl(hDir, FSCTL_SET_REPARSE_POINT, pReparseBuffer, pReparseBuffer->ReparseDataLength+REPARSE_MOUNTPOINT_HEADER_SIZE, NULL, 0, &dwRet, NULL)) {
    CloseHandle(hDir); 
    return -2;
  }

  CloseHandle(hDir); 
  return  0;
}  

extern "C" __declspec(dllexport) BOOL DeleteJunction(LPCSTR szJunction ) { 
  REPARSE_GUID_DATA_BUFFER rgdb = { 0 };
  HANDLE hDir = CreateFile(szJunction, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS, NULL);
  if (hDir == INVALID_HANDLE_VALUE) return -1;
   

  rgdb.ReparseTag = IO_REPARSE_TAG_MOUNT_POINT;
  DWORD dwBytes;
  const BOOL bOK = DeviceIoControl(hDir,
						FSCTL_DELETE_REPARSE_POINT,
						&rgdb,
						REPARSE_GUID_DATA_BUFFER_HEADER_SIZE,
						NULL,
						0,
						&dwBytes,
						0);

  CloseHandle(hDir); 
  if(bOK)return 0;
  return -2;
}  
 
/*
(
	'cl *.cpp'
	,'/W3'  
	,'/MD'  
	,'/O2 /Ot /GL /EHsc'  
	,'/D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL"'  
	,'/I"./INCLUDE"' 
	,'kernel32.lib user32.lib advapi32.lib'  
	,'/link /SUBSYSTEM:WINDOWS /MACHINE:X86'  
	,'/out:Junction.dll' 
	,'/dll'  
	,'/LIBPATH:".\LIB" /LIBPATH:".\LIB2"'  
) 
*/