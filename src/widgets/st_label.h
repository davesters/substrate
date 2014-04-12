#pragma once
#include <v8.h>
#include <gtk/gtk.h>
#include <string>

#include <widgets/st_widget.h>

class StLabel : StWidget
{
public:
	StLabel(GtkWidget* widget, string id);
	~StLabel();

	static void GetLabel(Local<String> property, const PropertyCallbackInfo<Value>& info);
	static void SetLabel(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);
};
