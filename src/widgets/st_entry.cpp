#include <v8.h>
#include <gtk/gtk.h>
#include <string>

#include <widgets/st_entry.h>
#include <environment.h>

using namespace v8;
using namespace std;

StEntry::StEntry()
{
}

void StEntry::PopulateObjectHandle(Local<ObjectTemplate> objTemplate) {
	StWidget::PopulateObjectHandle(objTemplate);

	objTemplate->SetAccessor(String::NewFromUtf8(Environment::GetIsolate(), "text"), GetText, SetText);
}

void StEntry::GetText(Local<String> property, const PropertyCallbackInfo<Value>& info) {
	HandleScope scope(info.GetIsolate());

	Local<Object> self = info.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	auto text = String::NewFromUtf8(info.GetIsolate(), gtk_entry_get_text((GtkEntry*)wrap->Value()));

	info.GetReturnValue().Set(text);
}

void StEntry::SetText(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
	HandleScope scope(info.GetIsolate());

	Local<Object> self = info.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	String::Utf8Value textVal(value);

	gtk_entry_set_text((GtkEntry*)wrap->Value(), *textVal);
}

StEntry::~StEntry()
{
}
