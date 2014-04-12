#include <v8.h>
#include <gtk/gtk.h>
#include <string>

#include <widgets/st_widget.h>
#include <environment.h>

using namespace v8;
using namespace std;

StWidget::StWidget(GtkWidget* widget, string id)
{
	this->id_ = id;
	this->name_ = gtk_widget_get_name(widget);
}

Local<Object> StWidget::GetObjectHandle() {
	return this->objTemplate_;
}

void StWidget::GetName(Local<String> property, const PropertyCallbackInfo<Value>& info) {
	Local<Object> self = info.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	void* widget = wrap->Value();

	string nameVal = static_cast<StWidget*>(widget)->name_;
	info.GetReturnValue().Set(String::NewFromUtf8(Environment::GetIsolate(), nameVal.c_str()));
}

void StWidget::GetId(Local<String> property, const PropertyCallbackInfo<Value>& info) {
	Local<Object> self = info.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	void* widget = wrap->Value();

	string nameVal = static_cast<StWidget*>(widget)->id_;
	info.GetReturnValue().Set(String::NewFromUtf8(Environment::GetIsolate(), nameVal.c_str()));
}

StWidget::~StWidget()
{
}
