#include "ui.h"
#include "logger.h"
#include <iostream>
#include <UIAutomation.h>
void getUIElement(POINT pt) {

	HRESULT hr = CoInitialize(NULL);
	IUIAutomation* pAutomation = nullptr;
	hr = CoCreateInstance(__uuidof(CUIAutomation), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pAutomation));
    // 从鼠标位置获取元素
    IUIAutomationElement* pElement = nullptr;
    hr = pAutomation->ElementFromPoint(pt, &pElement);
    if (FAILED(hr) || !pElement)
    {
		throw std::runtime_error("无法获取元素");
        pAutomation->Release();
        CoUninitialize();
    }

    // 获取元素的Automation ID
    BSTR bstrId;
    hr = pElement->get_CurrentClassName(&bstrId);
    if (SUCCEEDED(hr) && bstrId)
    {
        std::wcout << L"Automation ID: " << bstrId<<std::endl;
        SysFreeString(bstrId);
    }
    else
		throw std::runtime_error("无法获取Automation ID");
    // 释放资源
    pElement->Release();
    pAutomation->Release();
    CoUninitialize();
}