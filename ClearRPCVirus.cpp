// ClearRPCVirus.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "tchar.h"
#include "windows.h"
#include "shlwapi.h"

void DeleteVirusFiles();

int _tmain(int argc, _TCHAR* argv[])
{
	if( argc > 1 )
	{
		if( StrCmpI( argv[1], _T("-d") ) == 0 )
		{
			DeleteVirusFiles();

			return 0;
		}
	}

	HKEY hKey;
	LONG lRes = 0;

	lRes = RegOpenKey(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Services\\RpcPatch"), &hKey);
	if( lRes != ERROR_SUCCESS )
	{
		lRes = RegOpenKey(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Services\\RpcTftpd"), &hKey);
		if( lRes != ERROR_SUCCESS )
		{
			RegCloseKey( hKey );
			MessageBox( NULL, _T("您的系统没有感染RPC病毒，如果您还没有安装RPC漏洞补丁，请尽快安装该补丁！"), _T("RPC病毒检测"), MB_OK);
			return 0;
		}
	}

	RegCloseKey( hKey );

	MessageBox( NULL, _T("您的系统已经感染RPC病毒，请按确定进行清除！"), _T("RPC病毒检测"), MB_OK);

	SHDeleteKey( HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Services\\RpcPatch") );
	SHDeleteKey( HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Services\\RpcTftpd") );
	
	TCHAR szFilePath[MAX_PATH] = {0};
	StrCpy( szFilePath, argv[0] );
	StrCat( szFilePath, " -d");

	SHRegSetPath( HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), _T("ClearRPCVirus"), szFilePath, 0);
	//SHRegSetUSValue(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce"), _T("ClearRPCVirus"), REG_SZ, (void*)szFilePath, _tcslen(szFilePath), SHREGSET_HKLM);

	MessageBox( NULL, _T("请重启您的系统完成病毒清除！"), _T("RPC病毒检测"), MB_OK);

	return 0;
}

void DeleteVirusFiles()
{
	SHDeleteValue(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), _T("ClearRPCVirus"));

	TCHAR szSysPath[MAX_PATH] = {0};
	GetSystemDirectory( szSysPath, MAX_PATH );

	TCHAR szFilePath[MAX_PATH] = {0};
	StrCpy( szFilePath, szSysPath );
	StrCat( szFilePath, "\\wins\\svchost.exe" );

	DWORD dwAttrs =	GetFileAttributes( szFilePath );
	dwAttrs = dwAttrs & (~FILE_ATTRIBUTE_READONLY);
	SetFileAttributes(szFilePath, dwAttrs);

	if( DeleteFile(szFilePath) == ERROR_ACCESS_DENIED )
	{
		//MessageBox( NULL, _T("无法删除病毒文件，请重启后手工删除 system32\\wins\\svchost.exe 文件"), _T("RPC病毒检测"), MB_OK );
		_tprintf( _T("无法删除病毒文件，请重启后手工删除 %s 文件"), szFilePath );
		return;
	}

	StrCpy( szFilePath, szSysPath );
	StrCat( szFilePath, "\\wins\\DLLHOST.EXE" );
	dwAttrs = 0;
	dwAttrs = GetFileAttributes( szFilePath );
	dwAttrs = dwAttrs & (~FILE_ATTRIBUTE_READONLY);
	SetFileAttributes(szFilePath, dwAttrs);
	if( DeleteFile(szFilePath) == ERROR_ACCESS_DENIED )
	{
		_tprintf( _T("无法删除病毒文件，请重启后手工删除 %s 文件"), szFilePath );
		return;
	}

	_tprintf( _T("病毒文件已经删除，如果您的系统没有安装RPC漏洞补丁，\n请尽快安装补丁，然后再运行本程序监测病毒！"));
	_gettchar();
	return;
}