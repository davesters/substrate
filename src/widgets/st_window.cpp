#include <v8.h>
#include <gtk/gtk.h>
#include <string>

#include <widgets/st_window.h>
#include <ui.h>
#include <environment.h>

using namespace v8;
using namespace std;

StWindow::StWindow(string name)
{
	this->name_ = name;
	this->builder_ = gtk_builder_new();

	string fileName(name + ".ui");
	gtk_builder_add_from_file(this->builder_, fileName.c_str(), NULL);
	this->window_ = (GtkWidget*)gtk_builder_get_object(this->builder_, name.c_str());
}

GtkBuilder* StWindow::GetBuilder() {
	return this->builder_;
}

GtkWidget* StWindow::GetWindow() {
	return this->window_;
}

void StWindow::PopulateObjectHandle(Local<ObjectTemplate> objTemplate) {
	StWidget::PopulateObjectHandle(objTemplate);

	objTemplate->Set(String::NewFromUtf8(Environment::GetIsolate(), "getWidget"),
		FunctionTemplate::New(Environment::GetIsolate(), AddEventListener));
	objTemplate->Set(String::NewFromUtf8(Environment::GetIsolate(), "addEventListener"),
		FunctionTemplate::New(Environment::GetIsolate(), AddEventListener));
}

void StWindow::GetWidget(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() < 1) {
		return;
	}

	HandleScope scope(args.GetIsolate());

	Local<Object> self = args.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	auto window = static_cast<StWindow*>(wrap->Value());

	string name(*String::Utf8Value(args[0]));

	args.GetReturnValue().Set(Ui::GetWidgetObjectTemplate(window->name_, name));
}

void StWindow::AddEventListener(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() < 3) {
		return;
	}
	
	HandleScope handle_scope(args.GetIsolate());
	string objName(*String::Utf8Value(args[0]));
	String::Utf8Value eventType(args[1]);

	Local<Object> self = args.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	auto window = static_cast<StWindow*>(wrap->Value());

	UserData* userData = new UserData;
	userData->windowName = window->name_;
	userData->widgetName = objName;
	userData->callback = args[2].As<Function>();

	auto obj = gtk_builder_get_object(window->builder_, objName.c_str());
	g_signal_connect(obj, *eventType, G_CALLBACK(OnEvent), userData);

	args.GetReturnValue().Set(userData);
}

void StWindow::OnEvent(GtkWidget *widget, gpointer data) {
	HandleScope handle_scope(Environment::GetIsolate());

	auto context = Ui::GetContext();
	Context::Scope context_scope(context);
	UserData *userData = (UserData*)data;

	auto uiWidgetTemplate = Ui::GetWidgetObjectTemplate(userData->windowName, userData->widgetName).As<Value>();

	TryCatch tryCatch;
	auto result = userData->callback->Call(context->Global(), 1, &uiWidgetTemplate);
	if (result.IsEmpty()) {
		Ui::ShowV8Error(Environment::GetIsolate(), tryCatch.Exception(), tryCatch.StackTrace());
	}
}

StWindow::~StWindow()
{
	delete this->window_;
	delete this->builder_;
}
