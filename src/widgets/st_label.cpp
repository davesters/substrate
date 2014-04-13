#include <v8.h>
#include <gtk/gtk.h>
#include <string>

#include <widgets/st_label.h>
#include <environment.h>

using namespace v8;
using namespace std;

StLabel::StLabel(GtkWidget* widget, string id) : StWidget(widget, id)
{
}

void StLabel::PopulateObjectHandle(Handle<ObjectTemplate> objTemplate) {
	objTemplate->SetAccessor(String::NewFromUtf8(Environment::GetIsolate(), "label"), GetLabel, SetLabel);
}

void StLabel::GetLabel(Local<String> property, const PropertyCallbackInfo<Value>& info) {
	Local<Object> self = info.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	void* widget = wrap->Value();

	auto gtkWidget = static_cast<StLabel*>(widget)->widget_;
	info.GetReturnValue().Set(String::NewFromUtf8(Environment::GetIsolate(), gtk_label_get_label((GtkLabel*)gtkWidget)));
}

void StLabel::SetLabel(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
	Local<Object> self = info.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	void* widget = wrap->Value();

	string labelVal(*String::Utf8Value(value));
	auto gtkWidget = static_cast<StLabel*>(widget)->widget_;

	gtk_label_set_label((GtkLabel*)gtkWidget, labelVal.c_str());
}

StLabel::~StLabel()
{
}
