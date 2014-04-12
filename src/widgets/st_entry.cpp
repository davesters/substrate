#include <v8.h>
#include <gtk/gtk.h>
#include <string>

#include <widgets/st_entry.h>
#include <environment.h>

using namespace v8;
using namespace std;

StEntry::StEntry(GtkWidget* widget, string id) : StWidget(widget, id)
{
	this->widget_ = widget;

	Handle<ObjectTemplate> entry_templ = ObjectTemplate::New(Environment::GetIsolate());
	entry_templ->SetInternalFieldCount(1);

	entry_templ->SetAccessor(String::NewFromUtf8(Environment::GetIsolate(), "id"), GetId, NULL);
	entry_templ->SetAccessor(String::NewFromUtf8(Environment::GetIsolate(), "name"), GetName, NULL);
	entry_templ->SetAccessor(String::NewFromUtf8(Environment::GetIsolate(), "text"), GetText, SetText);

	objTemplate_ = entry_templ->NewInstance();
	objTemplate_->SetInternalField(0, External::New(Environment::GetIsolate(), this));
}

void StEntry::GetText(Local<String> property, const PropertyCallbackInfo<Value>& info) {
	Local<Object> self = info.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	void* widget = wrap->Value();

	auto gtkWidget = static_cast<StEntry*>(widget)->widget_;
	info.GetReturnValue().Set(String::NewFromUtf8(Environment::GetIsolate(), gtk_entry_get_text((GtkEntry*)gtkWidget)));
}

void StEntry::SetText(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
	Local<Object> self = info.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	void* widget = wrap->Value();

	string textVal(*String::Utf8Value(value));
	auto gtkWidget = static_cast<StEntry*>(widget)->widget_;

	gtk_entry_set_text((GtkEntry*)gtkWidget, textVal.c_str());
}

StEntry::~StEntry()
{
}
