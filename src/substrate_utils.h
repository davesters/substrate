#pragma once
#include <v8.h>
#include <gtk/gtk.h>
#include <string>

using namespace v8;
using namespace std;

class SubstrateUtils
{
public:
	static Handle<String> readJsFile(Isolate* isolate, const string& name);
	static string readFile(const string& name);
	static void ShowV8Error(Isolate* isolate, GtkWidget* window, Local<Value> exception, Local<Value> stackTrace);
	static void ShowMessageDialog(GtkWidget* window, char* message);
};
