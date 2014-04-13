#pragma once
#include <v8.h>
#include <gtk/gtk.h>
#include <string>

#include <widgets/st_widget.h>

class StEntry : StWidget
{
protected:
	virtual void PopulateObjectHandle(Handle<ObjectTemplate> objTemplate);

public:
	StEntry(GtkWidget* widget, string id);
	~StEntry();

	static void GetText(Local<String> property, const PropertyCallbackInfo<Value>& info);
	static void SetText(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);
};
