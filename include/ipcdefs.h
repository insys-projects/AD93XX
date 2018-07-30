#ifndef _IPCDEFS_H_
#define _IPCDEFS_H_

#include <stdio.h>

#if defined (__LINUX__) || defined(__linux__) || defined (__QNX__)
#include "ipcdefs_linux.h"
#else
#ifdef _WIN64
#define _IPCS(x)		L ## x
#define IPCS_strlen		wcslen
#define IPCS_strcpy		wcscpy
#define IPCS_strncpy	wcsncpy
#define IPCS_strcmp		wcscmp
#define IPCS_stricmp	_wcsicmp
#define IPCS_strnicmp	wcsnicmp
#define IPCS_strncmp	wcsncmp
#define IPCS_strcat		wcscat
#define IPCS_strchr		wcschr
#define IPCS_strstr		wcsstr
#define IPCS_strtol		wcstol
#define IPCS_strtoll	wcstoll
#define IPCS_strtoul	wcstoul
#define IPCS_strtod		wcstod
#define IPCS_strtok		wcstok
#define IPCS_atol		_wtol
#define IPCS_atoi		_wtoi
#define IPCS_atoi64		_wtoi64
#define IPCS_strtoui64	_wcstoui64
#define IPCS_atof		_wtof
#define IPCS_strlwr		_wcslwr
#define IPCS_itoa		_itow
#define IPCS_printf		wprintf
#define IPCS_fprintf	fwprintf
#define IPCS_fscanf		fwscanf
#define IPCS_sprintf	swprintf
#define IPCS_sprintf_s	swprintf_s
#define IPCS_vsprintf	vswprintf
#define IPCS_sscanf		swscanf
#define IPCS_fopen		_wfopen
#define IPCS_fclose		fclose
#define IPCS_freopen	_wfreopen
#define IPCS_fdopen		_wfdopen
#define IPCS_sopen		_wsopen
#define IPCS_open		_wopen
#define IPCS_fgets		fgetws
#define IPCS_fputs		fputws
#define IPCS_fgetc		fgetwc
#define IPCS_getenv		_wgetenv
#define IPCS_main		wmain
#define IPCS_toupper	towupper
#define IPCS_tolower	towlower
#define IPCS_isspace	iswspace
#define IPCS_isdigit	iswdigit
#define IPCS_isalnum	iswalnum
#define IPCS_isxdigit	iswxdigit
#define IPCS_cputs      _cputws
#define IPCS_cscanf     _cwscanf
#define IPCS_fputchar   _fputwchar
#define IPCS_fgetchar   _fgetwchar
#define IPCS_flushall   flushall
#define IPCS_putch      _putwch
#define IPCS_cgets      _cgetws
#define IPCS_puts		_putws
#define IPCS_gets(str)		_getws_s( str, 65536 )		
#define IPCS_tmpnam		_wtmpnam
#define IPCS_perror		_wperror
#define IPCS_getcwd		_wgetcwd
#define IPCS_getdcwd	_wgetdcwd
#define IPCS_mkdir		_wmkdir
#define IPCS_chdir		_wchdir
#define IPCS_rmdir		_wrmdir
#define IPCS_access		_waccess
#define IPCS_chmod		_wchmod
#define IPCS_creat		_wcreat
#define IPCS_mktemp		_wmktemp
#define IPCS_remove		_wremove
#define IPCS_rename		_wrename
#define IPCS_findfirst	_wfindfirst
#define IPCS_findnext	_wfindnext
#define IPCS_asctime	_wasctime
#define IPCS_ctime		_wctime
#define	IPCS_strftime	wcsftime
#define IPCS_stat		_wstat
#define IPCS_mbstoipcs(dst, src, maxcnt) mbstowcs( dst, src, maxcnt )
#define IPCS_ipcstombs(dst, src, maxcnt) wcstombs( dst, src, maxcnt )
#define IPCS_wcstoipcs(dst, src, maxcnt) strcpy( dst, src )
#define IPCS_ipcstowcs(dst, src, maxcnt) strcpy( dst, src )
#else
#define _IPCS(x)		x
#define IPCS_strlen		strlen
#define IPCS_strcpy		strcpy
#define IPCS_strncpy	strncpy
#define IPCS_strcmp		strcmp
#define IPCS_stricmp	_stricmp
#define IPCS_strnicmp	_strnicmp
#define IPCS_strncmp	strncmp
#define IPCS_strcat		strcat
#define IPCS_strchr		strchr
#define IPCS_strstr		strstr
#define IPCS_strtol		strtol
#define IPCS_strtoll		strtoll
#define IPCS_strtoul	strtoul
#define IPCS_strtod		strtod
#define IPCS_strtok		strtok
#define IPCS_atol		atol
#define IPCS_atoi		atoi
#define IPCS_atoi64		_atoi64
#define IPCS_strtoui64	_strtoui64
#define IPCS_atof		atof
#define IPCS_strlwr		_strlwr
#define IPCS_itoa		_itoa
#define IPCS_printf		printf
#define IPCS_fprintf	fprintf
#define IPCS_fscanf		fscanf
#define IPCS_sprintf	sprintf
#define IPCS_vsprintf	vsprintf
#define IPCS_sscanf		sscanf
#define IPCS_fopen		fopen
#define IPCS_freopen	freopen
#define IPCS_fdopen		fdopen
#define IPCS_sopen		sopen
#define IPCS_open		_open
#define IPCS_filelength _filelength
#define IPCS_fgets		fgets
#define IPCS_fputs		fputs
#define IPCS_fgetc		fgetc
#define IPCS_getenv		getenv
#define IPCS_main		main
#define IPCS_toupper	toupper
#define IPCS_tolower	tolower
#define IPCS_isspace	isspace
#define IPCS_isdigit	isdigit
#define IPCS_isalnum	isalnum
#define IPCS_isxdigit	isxdigit
#define IPCS_cputs      cputs
#define IPCS_cscanf     cscanf
#define IPCS_fputchar   fputchar
#define IPCS_fgetchar   fgetchar
#define IPCS_flushall   flushall
#define IPCS_putch      putch
#define IPCS_cgets      cgets
#define IPCS_puts		puts
#define IPCS_gets(str)		gets_s( str, 65536 )
#define IPCS_tmpnam		tmpnam
#define IPCS_perror		perror
#define IPCS_getcwd		getcwd
#define IPCS_getdcwd	_getdcwd
#define IPCS_mkdir		mkdir
#define IPCS_chdir		chdir
#define IPCS_rmdir		rmdir
#define IPCS_access		access
#define IPCS_chmod		chmod
#define IPCS_creat		creat
#define IPCS_mktemp		mktemp
#define IPCS_remove		remove
#define IPCS_rename		rename
#define IPCS_findfirst	_findfirst
#define IPCS_findnext	_findnext
#define IPCS_asctime	asctime
#define IPCS_ctime		ctime
#define	IPCS_strftime	strftime
#define IPCS_stat		stat
#define IPCS_mbstoipcs(dst, src, maxcnt) strcpy( dst, src )
#define IPCS_ipcstombs(dst, src, maxcnt) strcpy( dst, src )
#define IPCS_wcstoipcs(dst, src, maxcnt) mbstowcs( dst, src, maxcnt )
#define IPCS_ipcstowcs(dst, src, maxcnt) mbstowcs( dst, src, maxcnt )
#endif

#if !defined(WIN32) && !defined(__WIN32__)
//#ifndef WIN32
	#define FENTRY
	#define STDCALL
#else
	#include <windows.h>
	#define DllImport	__declspec( dllimport )
	#define DllExport	__declspec( dllexport )
	#define FENTRY		DllExport
	#define STDCALL		__stdcall
	#define	huge
#endif  // WIN32

#endif

#endif /* _IPCDEFS_H_ */



