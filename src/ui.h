#pragma once
#include <windows.h>
#include <string>
namespace UI {
	struct Element {
		std::string ClassName, Name, AutomationId,FrameworkId;
		int BoundingRect[4];
	};
	Element Get(POINT pos);
}