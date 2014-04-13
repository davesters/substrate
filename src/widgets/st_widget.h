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

	void CreateObjectTemplate();
	virtual void PopulateObjectHandle(Handle<ObjectTemplate> objTemplate) = 0;

public:
	StWidget(GtkWidget* widget, string id);
	~StWidget();

	Local<Object> GetObjectHandle();

	static void WeakCallback(const WeakCallbackData<Object, GtkWidget>& data);

	static void GetName(Local<String> property, const PropertyCallbackInfo<Value>& info);
	static void GetId(Local<String> property, const PropertyCallbackInfo<Value>& info);
};
