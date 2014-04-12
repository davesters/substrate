#include <v8.h>
#include <gtk/gtk.h>
#include <string>

#include <widgets/st_button.h>
#include <environment.h>

using namespace v8;
using namespace std;

StButton::StButton(GtkWidget* widget, string id) : StWidget(widget, id)
{
	this->widget_ = widget;

	Handle<ObjectTemplate> button_templ = ObjectTemplate::New(Environment::GetIsolate());
	button_templ->SetInternalFieldCount(1);

	button_templ->SetAccessor(String::NewFromUtf8(Environment::GetIsolate(), "id"), GetId, NULL);
	button_templ->SetAccessor(String::NewFromUtf8(Environment::GetIsolate(), "name"), GetName, NULL);
	button_templ->SetAccessor(String::NewFromUtf8(Environment::GetIsolate(), "label"), GetLabel, SetLabel);

	objTemplate_ = button_templ->NewInstance();
	objTemplate_->SetInternalField(0, External::New(Environment::GetIsolate(), this));
}

void StButton::GetLabel(Local<String> property, const PropertyCallbackInfo<Value>& info) {
	Local<Object> self = info.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	void* widget = wrap->Value();

	auto gtkWidget = static_cast<StButton*>(widget)->widget_;
	info.GetReturnValue().Set(String::NewFromUtf8(Environment::GetIsolate(), gtk_button_get_label((GtkButton*)gtkWidget)));
}

void StButton::SetLabel(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
	Local<Object> self = info.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	void* widget = wrap->Value();

	string labelVal(*String::Utf8Value(value));
	auto gtkWidget = static_cast<StButton*>(widget)->widget_;

	gtk_button_set_label((GtkButton*)gtkWidget, labelVal.c_str());
}

StButton::~StButton()
{
}
