#pragma once
#include <v8.h>
#include <gtk/gtk.h>
#include <string>
#include <map>

#include <widgets/st_widget.h>
#include <widgets/st_window.h>

using namespace v8;
using namespace std;

class Ui
{
public:
	Ui();
	~Ui();

	static void SetContext(Handle<Context> context);
	static Handle<Context> GetContext();

	static Local<Object> LoadMainWindow(string name);
	static Local<Object> LoadWindow(string name);

	static void ShowV8Error(Isolate* isolate, Local<Value> exception, Local<Value> stackTrace);
	static void ShowMessageDialog(char* message);
	static Local<Object> GetWidgetObjectTemplate(string window, string name);
	static void CloseWindowCallback(GtkWidget *widget, gpointer data);
	static void CloseWindow(string name);

	static void WeakCallback(const WeakCallbackData<Object, GtkWidget>& data);
};
