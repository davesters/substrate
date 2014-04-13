#pragma once
#include <v8.h>
#include <gtk/gtk.h>
#include <string>

#include <widgets/st_widget.h>

using namespace v8;
using namespace std;

class Ui
{
public:
	Ui();
	~Ui();

	GtkWidget* BuildMainWindow(GtkBuilder* builder, string name);
	void BuildWindow(GtkBuilder* builder, string name, string parent);

	StWidget* GetWidget(GtkWidget* widget, string id);
};
