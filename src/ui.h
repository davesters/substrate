#pragma once
#include <v8.h>
#include <gtk/gtk.h>
#include <string>
#include <map>

#include <widgets/st_widget.h>

using namespace v8;
using namespace std;

class Ui
{
public:
	Ui();
	~Ui();

	static GtkWidget* BuildMainWindow(GtkBuilder* builder, string name);
	static void BuildWindow(GtkBuilder* builder, string name, string parent);

	static StWidget* GetWidget(GtkWidget* widget, string id);
};
