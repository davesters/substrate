#pragma once
#include <v8.h>
#include <gtk/gtk.h>
#include <string>

using namespace v8;
using namespace std;

class StWidget
{
protected:
	string id_;
	string name_;
	GtkWidget* widget_;
	Local<Object> objTemplate_;

public:
	StWidget(GtkWidget* widget, string id);
	~StWidget();

	Local<Object> GetObjectHandle();

	static void GetName(Local<String> property, const PropertyCallbackInfo<Value>& info);
	static void GetId(Local<String> property, const PropertyCallbackInfo<Value>& info);
};
