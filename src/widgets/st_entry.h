#pragma once
#include <v8.h>
#include <gtk/gtk.h>
#include <string>

#include <widgets/st_widget.h>

class StEntry : StWidget
{
public:
	StEntry();
	~StEntry();

	static void PopulateObjectHandle(Local<ObjectTemplate> objTemplate);

	static void GetText(Local<String> property, const PropertyCallbackInfo<Value>& info);
	static void SetText(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);
};
