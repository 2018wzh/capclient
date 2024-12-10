// src/ui.cpp

#include "ui.h"
#include "utils.h"
#include "logger.h"
#include <iostream>
#include <UIAutomation.h>

namespace UI {
    Element Get(POINT pt) {
        Element uie;
        HRESULT hr = CoInitialize(NULL);
        IUIAutomation* pAutomation = nullptr;
        hr = CoCreateInstance(__uuidof(CUIAutomation), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pAutomation));

        // �����λ�û�ȡԪ��
        IUIAutomationElement* pElement = nullptr;
        hr = pAutomation->ElementFromPoint(pt, &pElement);
        if (FAILED(hr) || !pElement)
        {
            pAutomation->Release();
            CoUninitialize();
            throw std::runtime_error("Failed to get Element");
        }

        // ��ȡ ClassName
        BSTR bstrClassName = nullptr;
        hr = pElement->get_CurrentClassName(&bstrClassName);
        if (SUCCEEDED(hr) && bstrClassName)
        {
            uie.ClassName = Utils::toStr(std::wstring(bstrClassName));
            SysFreeString(bstrClassName);
        }

        // ��ȡ Name
        BSTR bstrName = nullptr;
        hr = pElement->get_CurrentName(&bstrName);
        if (SUCCEEDED(hr) && bstrName)
        {
            uie.Name = Utils::toStr(std::wstring(bstrName));
            SysFreeString(bstrName);
        }

        // ��ȡ AutomationId
        BSTR bstrAutomationId = nullptr;
        hr = pElement->get_CurrentAutomationId(&bstrAutomationId);
        if (SUCCEEDED(hr) && bstrAutomationId)
        {
            uie.AutomationId = Utils::toStr(std::wstring(bstrAutomationId));
            SysFreeString(bstrAutomationId);
        }

        // ��ȡ FrameworkId
        BSTR bstrFrameworkId = nullptr;
        hr = pElement->get_CurrentFrameworkId(&bstrFrameworkId);
        if (SUCCEEDED(hr) && bstrFrameworkId)
        {
            uie.FrameworkId = Utils::toStr(std::wstring(bstrFrameworkId));
            SysFreeString(bstrFrameworkId);
        }

        // ��ȡ BoundingRect
        RECT rect;
        hr = pElement->get_CurrentBoundingRectangle(&rect);
        if (SUCCEEDED(hr)) {
            uie.BoundingRect[0] = rect.left;
            uie.BoundingRect[1] = rect.top;
            uie.BoundingRect[2] = rect.right;
            uie.BoundingRect[3] = rect.bottom;
        }
        Logger::get_instance()->debug("Successfully get UI Element");
        // �ͷ���Դ
        pElement->Release();
        pAutomation->Release();
        CoUninitialize();
        return uie;
    }
}
