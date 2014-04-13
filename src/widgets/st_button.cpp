#include <v8.h>
#include <gtk/gtk.h>
#include <string>

#include <widgets/st_button.h>
#include <environment.h>

using namespace v8;
using namespace std;

StButton::StButton(GtkWidget* widget, string id) : StWidget(widget, id)
{
}

void StButton::PopulateObjectHandle(Handle<ObjectTemplate> objTemplate) {
	objTemplate->SetAccessor(String::NewFromUtf8(Environment::GetIsolate(), "label"), GetLabel, SetLabel);
}

void StButton::GetLabel(Local<String> property, const PropertyCallbackInfo<Value>& info) {
	Local<Object> self = info.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	auto widget = wrap->Value();

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
