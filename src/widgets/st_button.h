#pragma once
#include <v8.h>
#include <gtk/gtk.h>
#include <string>

#include <widgets/st_widget.h>

using namespace v8;
using namespace std;

class StButton : StWidget
{
protected:
	virtual void PopulateObjectHandle(Handle<ObjectTemplate> objTemplate);

public:
	StButton(GtkWidget* widget, string id);
	~StButton();

	static void GetLabel(Local<String> property, const PropertyCallbackInfo<Value>& info);
	static void SetLabel(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);
};
