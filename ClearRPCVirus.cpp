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
			MessageBox( NULL, _T("����ϵͳû�и�ȾRPC�������������û�а�װRPC©���������뾡�찲װ�ò�����"), _T("RPC�������"), MB_OK);
			return 0;
		}
	}

	RegCloseKey( hKey );

	MessageBox( NULL, _T("����ϵͳ�Ѿ���ȾRPC�������밴ȷ�����������"), _T("RPC�������"), MB_OK);

	SHDeleteKey( HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Services\\RpcPatch") );
	SHDeleteKey( HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Services\\RpcTftpd") );
	
	TCHAR szFilePath[MAX_PATH] = {0};
	StrCpy( szFilePath, argv[0] );
	StrCat( szFilePath, " -d");

	SHRegSetPath( HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), _T("ClearRPCVirus"), szFilePath, 0);
	//SHRegSetUSValue(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce"), _T("ClearRPCVirus"), REG_SZ, (void*)szFilePath, _tcslen(szFilePath), SHREGSET_HKLM);

	MessageBox( NULL, _T("����������ϵͳ��ɲ��������"), _T("RPC�������"), MB_OK);

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
		//MessageBox( NULL, _T("�޷�ɾ�������ļ������������ֹ�ɾ�� system32\\wins\\svchost.exe �ļ�"), _T("RPC�������"), MB_OK );
		_tprintf( _T("�޷�ɾ�������ļ������������ֹ�ɾ�� %s �ļ�"), szFilePath );
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
		_tprintf( _T("�޷�ɾ�������ļ������������ֹ�ɾ�� %s �ļ�"), szFilePath );
		return;
	}

	_tprintf( _T("�����ļ��Ѿ�ɾ�����������ϵͳû�а�װRPC©��������\n�뾡�찲װ������Ȼ�������б������ⲡ����"));
	_gettchar();
	return;
}