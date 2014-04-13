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
	this->widget_ = widget;
	this->name_ = gtk_widget_get_name(widget);
}

Local<Object> StWidget::GetObjectHandle() {
	if (*this->objTemplate_ == NULL) {
		this->CreateObjectTemplate();
	}

	return this->objTemplate_;
}

void StWidget::CreateObjectTemplate() {
	auto objTemplate = ObjectTemplate::New(Environment::GetIsolate());
	objTemplate->SetInternalFieldCount(1);

	objTemplate->SetAccessor(String::NewFromUtf8(Environment::GetIsolate(), "id"), GetId, NULL);
	objTemplate->SetAccessor(String::NewFromUtf8(Environment::GetIsolate(), "name"), GetName, NULL);
	this->PopulateObjectHandle(objTemplate);

	this->objTemplate_ = objTemplate->NewInstance();
	this->objTemplate_->SetInternalField(0, External::New(Environment::GetIsolate(), this));

	Persistent<Object> persistent_value(Environment::GetIsolate(), objTemplate_);
	persistent_value.SetWeak(this->widget_, WeakCallback);
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

void StWidget::WeakCallback(const WeakCallbackData<Object, GtkWidget>& data) {
	auto obj_ptr = data.GetParameter();

	delete obj_ptr;
	data.GetValue().Clear();
}

StWidget::~StWidget()
{
}
