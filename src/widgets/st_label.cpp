#include <v8.h>
#include <gtk/gtk.h>
#include <string>

#include <widgets/st_label.h>
#include <environment.h>

using namespace v8;
using namespace std;

StLabel::StLabel()
{
}

void StLabel::PopulateObjectHandle(Local<ObjectTemplate> objTemplate) {
	StWidget::PopulateObjectHandle(objTemplate);

	objTemplate->SetAccessor(String::NewFromUtf8(Environment::GetIsolate(), "label"), GetLabel, SetLabel);
}

void StLabel::GetLabel(Local<String> property, const PropertyCallbackInfo<Value>& info) {
	HandleScope scope(info.GetIsolate());

	Local<Object> self = info.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	auto widget = static_cast<GtkLabel*>(wrap->Value());

	info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), gtk_label_get_label(widget)));
}

void StLabel::SetLabel(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
	HandleScope scope(info.GetIsolate());

	Local<Object> self = info.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	auto widget = static_cast<GtkLabel*>(wrap->Value());
	String::Utf8Value labelVal(value);

	gtk_label_set_label(widget, *labelVal);
}

StLabel::~StLabel()
{
}
