#include <v8.h>
#include <gtk/gtk.h>
#include <string>

#include <ui.h>
#include <widgets/st_widget.h>
#include <widgets/st_button.h>
#include <widgets/st_entry.h>
#include <widgets/st_label.h>

using namespace v8;
using namespace std;

Ui::Ui()
{
}

GtkWidget* Ui::BuildMainWindow(GtkBuilder* builder, string name) {
	string fileName(name + ".ui");
	gtk_builder_add_from_file(builder, fileName.c_str(), NULL);
	auto window = (GtkWidget*)gtk_builder_get_object(builder, name.c_str());

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	return window;
}

void Ui::BuildWindow(GtkBuilder* builder, string name, string parent) {

}

StWidget* Ui::GetWidget(GtkWidget* widget, string id) {
	auto widgetType = string(gtk_widget_get_name(widget));

	if (widgetType == "GtkButton") {
		auto w = (StWidget*)(new StButton(widget, id));
		return w;
	}
	if (widgetType == "GtkLabel") {
		auto w = (StWidget*)(new StLabel(widget, id));
		return w;
	}
	if (widgetType == "GtkEntry") {
		auto w = (StWidget*)(new StEntry(widget, id));
		return w;
	}

	return NULL;
}

Ui::~Ui()
{
}
