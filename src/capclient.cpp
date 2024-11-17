// capclient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include "utils.h"
#include "hook.h"


kbdHook khook;
mouseHook mhook;
int main()
{
	bool adm = checkAdm();
	if (!adm)
		return elevateAdm();
	khook.installHook();
	mhook.installHook();
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	khook.unInstallHook();
	mhook.unInstallHook();
}

