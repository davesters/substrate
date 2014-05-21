#include <v8.h>
#include <gtk/gtk.h>
#include <string>

#include <widgets/st_widget.h>
#include <environment.h>

using namespace v8;
using namespace std;

StWidget::StWidget()
{
}

void StWidget::PopulateObjectHandle(Local<ObjectTemplate> objTemplate) {
	objTemplate->SetAccessor(String::NewFromUtf8(Environment::GetIsolate(), "id"), GetId, NULL);
	objTemplate->SetAccessor(String::NewFromUtf8(Environment::GetIsolate(), "name"), GetName, NULL);
}

void StWidget::GetName(Local<String> property, const PropertyCallbackInfo<Value>& info) {
	HandleScope scope(info.GetIsolate());

	Local<Object> self = info.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	auto widget = static_cast<GtkWidget*>(wrap->Value());

	auto nameVal = gtk_widget_get_name(widget);
	info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), nameVal));
}

void StWidget::GetId(Local<String> property, const PropertyCallbackInfo<Value>& info) {
	HandleScope scope(info.GetIsolate());

	Local<Object> self = info.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	auto widget = static_cast<GtkBuildable*>(wrap->Value());

	auto nameVal = gtk_buildable_get_name(widget);
	info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), nameVal));
}

StWidget::~StWidget()
{
}
