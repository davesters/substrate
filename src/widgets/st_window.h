#pragma once
#include <v8.h>
#include <gtk/gtk.h>
#include <string>

#include <widgets/st_widget.h>

using namespace v8;
using namespace std;

class StWindow
{
private:
	string name_;
	GtkWidget* window_;
	GtkBuilder* builder_;

public:
	StWindow(string name);
	~StWindow();

	GtkBuilder* GetBuilder();
	GtkWidget* GetWindow();

	static void PopulateObjectHandle(Local<ObjectTemplate> objTemplate);

	static void GetWidget(const FunctionCallbackInfo<Value>& args);
	static void AddEventListener(const FunctionCallbackInfo<Value>& args);
	static void OnEvent(GtkWidget *widget, gpointer data);
};

struct UserData { string windowName; string widgetName; Local<Function> callback; };
