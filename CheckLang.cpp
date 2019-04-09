/* CheckLang plugin for NSIS
 * Add and remove record from PATH environment variable
 * Copyright (C) 2015 Victor Spirin <vvs13@mail.ru>
 * some defintions I took from AccessControl plugin by Mathias Hasselmann
 */
/*
#define WIN32_LEAN_AND_MEAN
#ifdef _WIN64
#define WINVER 0x502
#else
#define WINVER 0x400
#endif
*/
#include <windows.h>
#ifdef UNICODE
#include "nsis_unicode/pluginapi.h"
#else
#include "nsis_ansi/pluginapi.h"
#endif
#include <aclapi.h>
#include <sddl.h>
//#include <winnls.h>
//#include <tchar.h>

/*****************************************************************************
 GLOBAL VARIABLES
 *****************************************************************************/

HINSTANCE g_hInstance = NULL;
int g_string_size = 1024;
extra_parameters* g_extra = NULL;

/*****************************************************************************
 UTILITIES
 *****************************************************************************/

#define SIZE_OF_ARRAY(Array) (sizeof((Array)) / sizeof(*(Array)))
#define ARRAY_CONTAINS(Array, Index) (Index >= 0 && Index < SIZE_OF_ARRAY(Array))

void* LocalAllocZero(size_t cb) { return LocalAlloc(LPTR, cb); }
inline void* LocalAlloc(size_t cb) { return LocalAllocZero(cb); }

/*****************************************************************************
 PLUG-IN HANDLING
 *****************************************************************************/

#define PUBLIC_FUNCTION(Name) \
extern "C" void __declspec(dllexport) __cdecl Name(HWND hWndParent, int string_size, TCHAR* variables, stack_t** stacktop, extra_parameters* extra) \
{ \
  EXDLL_INIT(); \
  g_string_size = string_size; \
  g_extra = extra;

#define PUBLIC_FUNCTION_END \
}

//UILANGUAGE_ENUMPROCA UiLangProc;

static BOOL langFound = FALSE;

BOOL CALLBACK UiLangProc(LPSTR id, LONG_PTR userStr)
{
	LPSTR find = (LPSTR)userStr;
	if (!lstrcmp(find, id)){
		langFound = 1;
		return FALSE;
	}
	

	return TRUE;


}

BOOL isLangPresent(LPSTR langId)
{
	langFound = FALSE;
	EnumUILanguagesA(UiLangProc, 0, (LONG_PTR)langId);
	return  langFound;
}

PUBLIC_FUNCTION(CheckLang)
{
	TCHAR *retstr = TEXT("0");
	TCHAR* param = (TCHAR*)LocalAlloc(g_string_size*sizeof(TCHAR));
	popstring(param);
	if (param && isLangPresent(param)) retstr = TEXT("1");
	LocalFree(param);
	pushstring(retstr);
}
PUBLIC_FUNCTION_END



#ifdef _VC_NODEFAULTLIB
#define DllMain _DllMainCRTStartup
#endif
EXTERN_C BOOL WINAPI DllMain(HINSTANCE hInst, ULONG ul_reason_for_call, LPVOID lpReserved)
{
  g_hInstance = (HINSTANCE)hInst;
  return TRUE;
}