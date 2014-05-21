#include <v8.h>
#include <gtk/gtk.h>
#include <string>

#include <ui.h>
#include <environment.h>
#include <widgets/st_widget.h>
#include <widgets/st_button.h>
#include <widgets/st_entry.h>
#include <widgets/st_label.h>

using namespace v8;
using namespace std;

static map<string, StWindow*> windows_;
static StWindow* mainWindow_;
static Persistent<Context> persistentContext_;

Ui::Ui()
{
}

void Ui::SetContext(Handle<Context> context) {
	persistentContext_.Reset(Environment::GetIsolate(), context);
}

Handle<Context> Ui::GetContext() {
	return Local<Context>::New(Environment::GetIsolate(), persistentContext_);
}

Local<Object> Ui::LoadMainWindow(string name) {
	windows_[name] = new StWindow(name);
	mainWindow_ = windows_[name];

	g_signal_connect(mainWindow_->GetWindow(), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	auto objTemplate = ObjectTemplate::New(Environment::GetIsolate());
	objTemplate->SetInternalFieldCount(1);

	mainWindow_->PopulateObjectHandle(objTemplate);

	auto objTemplateInst = objTemplate->NewInstance();
	objTemplateInst->SetInternalField(0, External::New(Environment::GetIsolate(), mainWindow_));

	return objTemplateInst;
}

Local<Object> Ui::LoadWindow(string name) {
	windows_[name] = new StWindow(name);

	UserData* userData = new UserData;
	userData->windowName = name;
	g_signal_connect(windows_[name]->GetWindow(), "destroy", G_CALLBACK(CloseWindowCallback), userData);

	auto objTemplate = ObjectTemplate::New(Environment::GetIsolate());
	objTemplate->SetInternalFieldCount(1);

	windows_[name]->PopulateObjectHandle(objTemplate);

	auto objTemplateInst = objTemplate->NewInstance();
	objTemplateInst->SetInternalField(0, External::New(Environment::GetIsolate(), windows_[name]));

	return objTemplateInst;
}

void Ui::ShowV8Error(Isolate* isolate, Local<Value> exception, Local<Value> stackTrace) {
	auto exception_str = String::Concat(exception->ToString(), String::NewFromUtf8(isolate, "; Stack: "));
	exception_str = String::Concat(exception_str, stackTrace->ToString());

	ShowMessageDialog(*String::Utf8Value(exception_str));
}

void Ui::ShowMessageDialog(char* message) {
	auto dialog = gtk_message_dialog_new(
		(GtkWindow*)mainWindow_,
		GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_MESSAGE_INFO,
		GTK_BUTTONS_OK,
		message);

	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

Local<Object> Ui::GetWidgetObjectTemplate(string window, string name) {
	auto widget = (GtkWidget*)gtk_builder_get_object(windows_[window]->GetBuilder(), name.c_str());
	auto objTemplate = ObjectTemplate::New(Environment::GetIsolate());
	objTemplate->SetInternalFieldCount(1);

	string widgetType = string(gtk_widget_get_name(widget));

	if (widgetType == "GtkButton") {
		StButton::PopulateObjectHandle(objTemplate);
	}
	if (widgetType == "GtkLabel") {
		StLabel::PopulateObjectHandle(objTemplate);
	}
	if (widgetType == "GtkEntry") {
		StEntry::PopulateObjectHandle(objTemplate);
	}

	auto objTemplateInst = objTemplate->NewInstance();
	objTemplateInst->SetInternalField(0, External::New(Environment::GetIsolate(), widget));

	Persistent<Object> persistent_value(Environment::GetIsolate(), objTemplateInst);
	persistent_value.SetWeak(widget, WeakCallback);

	return objTemplateInst;
}

void Ui::WeakCallback(const WeakCallbackData<Object, GtkWidget>& data) {
	auto obj_ptr = data.GetParameter();

	delete obj_ptr;
	data.GetValue().Clear();
}

void Ui::CloseWindowCallback(GtkWidget *widget, gpointer data) {
	UserData *userData = (UserData*)data;

	delete windows_[userData->windowName];
}

void Ui::CloseWindow(string name) {
	delete windows_[name];
}

Ui::~Ui()
{
}
