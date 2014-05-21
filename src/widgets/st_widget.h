#pragma once
#include <v8.h>
#include <gtk/gtk.h>
#include <string>

using namespace v8;
using namespace std;

class StWidget
{
public:
	StWidget();
	~StWidget();

	static void PopulateObjectHandle(Local<ObjectTemplate> objTemplate);

	static void GetName(Local<String> property, const PropertyCallbackInfo<Value>& info);
	static void GetId(Local<String> property, const PropertyCallbackInfo<Value>& info);
};
