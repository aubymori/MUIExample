#include <windows.h>
#include <stdio.h>
#include "resource.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	WCHAR szHello[256] = { 0 };
	SetThreadUILanguage(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	LoadStringW(GetModuleHandleW(NULL), IDS_HELLO, szHello, 256);
	MessageBoxW(NULL, szHello, L"In en-US:", MB_ICONINFORMATION);
	SetThreadUILanguage(MAKELANGID(LANG_JAPANESE, SUBLANG_JAPANESE_JAPAN));
	LoadStringW(GetModuleHandleW(NULL), IDS_HELLO, szHello, 256);
	MessageBoxW(NULL, szHello, L"In ja-JP:", MB_ICONINFORMATION);
	return 0;
}