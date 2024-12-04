#include "ui.h"
#include "logger.h"
#include <iostream>
#include <UIAutomation.h>
void getUIElement(POINT pt) {

	HRESULT hr = CoInitialize(NULL);
	IUIAutomation* pAutomation = nullptr;
	hr = CoCreateInstance(__uuidof(CUIAutomation), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pAutomation));
    // �����λ�û�ȡԪ��
    IUIAutomationElement* pElement = nullptr;
    hr = pAutomation->ElementFromPoint(pt, &pElement);
    if (FAILED(hr) || !pElement)
    {
		throw std::runtime_error("�޷���ȡԪ��");
        pAutomation->Release();
        CoUninitialize();
    }

    // ��ȡԪ�ص�Automation ID
    BSTR bstrId;
    hr = pElement->get_CurrentClassName(&bstrId);
    if (SUCCEEDED(hr) && bstrId)
    {
        std::wcout << L"Automation ID: " << bstrId<<std::endl;
        SysFreeString(bstrId);
    }
    else
		throw std::runtime_error("�޷���ȡAutomation ID");
    // �ͷ���Դ
    pElement->Release();
    pAutomation->Release();
    CoUninitialize();
}