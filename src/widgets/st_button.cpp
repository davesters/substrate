#include <v8.h>
#include <gtk/gtk.h>
#include <string>

#include <widgets/st_button.h>
#include <environment.h>

using namespace v8;
using namespace std;

StButton::StButton()
{
}

void StButton::PopulateObjectHandle(Local<ObjectTemplate> objTemplate) {
	StWidget::PopulateObjectHandle(objTemplate);

	objTemplate->SetAccessor(String::NewFromUtf8(Environment::GetIsolate(), "label"), GetLabel, SetLabel);
}

void StButton::GetLabel(Local<String> property, const PropertyCallbackInfo<Value>& info) {
	HandleScope scope(info.GetIsolate());

	Local<Object> self = info.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	auto widget = static_cast<GtkButton*>(wrap->Value());

	info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), gtk_button_get_label(widget)));
}

void StButton::SetLabel(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
	HandleScope scope(info.GetIsolate());

	Local<Object> self = info.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	auto widget = static_cast<GtkButton*>(wrap->Value());
	String::Utf8Value labelVal(value);

	gtk_button_set_label(widget, *labelVal);
}

StButton::~StButton()
{
}
