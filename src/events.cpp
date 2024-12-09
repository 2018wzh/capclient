#include "events.h"
#include "utils.h"
#include <ctime>
#include "ui.h"
#include "logger.h"
Event::Journal::Journal(MSLLHOOKSTRUCT *ks, WPARAM w,bool isDown,time_t tm){
	if (isDown)
		data["action"] = "down";
	else
		data["action"] = "up";
	data["value"] = int(w);
	data["friendly"] = Utils::wmConvert(w);
	data["posX"] = int(ks->pt.x);
	data["posY"] = int(ks->pt.y);
	id = uuidxx::uuid::Generate();
	time = tm;
	type = Event::Type::Mouse;
	UI::Element uie;
	try{
		uie = UI::Get(ks->pt); 
	}
	catch (std::exception& e) {
		Logger::get_instance()->error(e.what());
		uie.ClassName = "Unknown";
		uie.Name = "Unknown";
		uie.AutomationId = "Unknown";
		uie.FrameworkId = "Unknown";
		uie.BoundingRect[0] = 0;
		uie.BoundingRect[1] = 0;
		uie.BoundingRect[2] = 0;
		uie.BoundingRect[3] = 0;
	}
	Json::Value UIElement;
	UIElement["ClassName"] = uie.ClassName;
	UIElement["Name"] = uie.Name;
	UIElement["AutomationId"] = uie.AutomationId;
	UIElement["FrameworkId"] = uie.FrameworkId;
	UIElement["BoundingRect"]["left"] = uie.BoundingRect[0];
	UIElement["BoundingRect"]["top"] = uie.BoundingRect[1];
	UIElement["BoundingRect"]["right"] = uie.BoundingRect[2];
	UIElement["BoundingRect"]["bottom"] = uie.BoundingRect[3];
	data["UIElement"] = UIElement;
	//Logger::get_instance()->debug(Utils::toStr(UIElement));
}
Event::Journal::Journal(KBDLLHOOKSTRUCT* ks,bool isDown,time_t tm) {
	id = uuidxx::uuid::Generate();
	type = Event::Type::Keyboard;
	time = tm;
	if (isDown)
		data["action"] = "down";
	else
		data["action"] = "up";
	data["value"] = int(ks->vkCode);
	data["friendly"] = Utils::vkConvert(ks->vkCode);
}
Event::Journal::Journal(time_t tm) {
	id = uuidxx::uuid::Generate();
	Json::StreamWriterBuilder writer;
	type = Event::Type::Screen;
	time = tm;
	data["screenshot"] = Screenshot::Make();
}