#include <v8.h>
#include <gtk/gtk.h>
#include <string>
#include <memory>

#include <substrate.h>
#include <substrate_utils.h>
#include <ui.h>
#include <environment.h>

using namespace v8;
using namespace std;

static Isolate* isolate_;
static Persistent<Context> persistentContext;
static GtkWidget* window;
static GtkBuilder* builder;
static Ui* ui;

void Substrate::init(int argc, char* argv[])
{
	isolate_ = Isolate::GetCurrent();
	Environment::SetIsolate(isolate_);
	HandleScope handle_scope(isolate_);

	Handle<Context> context = Context::New(isolate_, NULL, CreateGlobalObject());
	persistentContext.Reset(isolate_, context);

	Context::Scope context_scope(context);

	Handle<String> source = SubstrateUtils::readJsFile(isolate_, "main.js");
	TryCatch tryCatch;

	Handle<Script> script = Script::Compile(source);
	if (script.IsEmpty()) {
		SubstrateUtils::ShowV8Error(isolate_, window, tryCatch.Exception(), tryCatch.StackTrace());
	}

	tryCatch.Reset();
	Handle<Value> result = script->Run();
	if (result.IsEmpty()) {
		SubstrateUtils::ShowV8Error(isolate_, window, tryCatch.Exception(), tryCatch.StackTrace());
	}

	gtk_init(&argc, &argv);

	builder = gtk_builder_new();
	ui = new Ui();

	Handle<Value> initObj = context->Global()->Get(String::NewFromUtf8(isolate_, "on_app_ready"));
	Handle<Function> initFunc = Handle<Function>::Cast(initObj);

	tryCatch.Reset();
	result = initFunc->Call(context->Global(), 0, NULL);
	if (result.IsEmpty()) {
		SubstrateUtils::ShowV8Error(isolate_, window, tryCatch.Exception(), tryCatch.StackTrace());
	}

	gtk_main();
}

Handle<ObjectTemplate> Substrate::CreateGlobalObject() {
	auto global = ObjectTemplate::New(isolate_);
	global->Set(String::NewFromUtf8(isolate_, "_s"), CreateSubstrateObject());

	return global;
}

Handle<ObjectTemplate> Substrate::CreateSubstrateObject() {
	auto substrateObj = ObjectTemplate::New(isolate_);

	substrateObj->SetInternalFieldCount(1);
	substrateObj->Set(String::NewFromUtf8(isolate_, "alert"), FunctionTemplate::New(isolate_, alertCallback));
	substrateObj->Set(String::NewFromUtf8(isolate_, "bind_event"), FunctionTemplate::New(isolate_, bindEventCallback));
	substrateObj->Set(String::NewFromUtf8(isolate_, "exit"), FunctionTemplate::New(isolate_, exitCallback));
	substrateObj->Set(String::NewFromUtf8(isolate_, "load_window"), FunctionTemplate::New(isolate_, loadWindowCallback));
	substrateObj->Set(String::NewFromUtf8(isolate_, "load_styles"), FunctionTemplate::New(isolate_, loadStylesCallback));
	substrateObj->Set(String::NewFromUtf8(isolate_, "get_widget"), FunctionTemplate::New(isolate_, getWidgetCallback));

	return substrateObj;
}

void Substrate::alertCallback(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() < 1) {
		return;
	}

	HandleScope scope(args.GetIsolate());
	Handle<Value> title;

	if (args.Length() > 1) {
		title = args[1];
	}
	else {
		title = String::NewFromUtf8(args.GetIsolate(), "");
	}

	String::Utf8Value msgValue(args[0]);
	String::Utf8Value titleValue(title);

	SubstrateUtils::ShowMessageDialog(window, *msgValue);
}

void Substrate::loadWindowCallback(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() < 1) {
		return;
	}

	string windowName(*String::Utf8Value(args[0]));
	string mainWindow("");

	if (args.Length() > 1) {
		mainWindow = *String::Utf8Value(args[1]);
	}

	if (mainWindow == "") {
		window = ui->BuildMainWindow(builder, windowName);
	}
	else {
		ui->BuildWindow(builder, windowName, mainWindow);
	}
}

void Substrate::loadStylesCallback(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() < 1) {
		return;
	}

	string styleFilename(*String::Utf8Value(args[0]));
	auto display = gdk_display_get_default();
	auto screen = gdk_display_get_default_screen(display);
	auto cssProvider = gtk_css_provider_new();

	gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
	gtk_css_provider_load_from_path(cssProvider, (styleFilename + ".css").c_str(), NULL);

	g_object_unref(cssProvider);
}

void Substrate::getWidgetCallback(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() < 1) {
		return;
	}
	string name(*String::Utf8Value(args[0]));

	auto widget = (GtkWidget*)gtk_builder_get_object(builder, name.c_str());
	auto uiWidget = ui->GetWidget(widget, name);

	args.GetReturnValue().Set(uiWidget->GetObjectHandle().As<Value>());
}

void Substrate::bindEventCallback(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() < 3) {
		return;
	}

	HandleScope handle_scope(args.GetIsolate());
	String::Utf8Value objName(args[0]);
	String::Utf8Value eventType(args[1]);

	string eventCheck(*String::Utf8Value(args[1]));
	string callbackName(*String::Utf8Value(args[2]));

	UserData* userData = new UserData;
	userData->callbackName = callbackName;
	userData->objName = *objName;

	if (eventCheck == "clicked") {
		GObject* obj = gtk_builder_get_object(builder, *objName);
		g_signal_connect(obj, *eventType, G_CALLBACK(onEvent), userData);
	}

	args.GetReturnValue().Set(userData);
}

void Substrate::exitCallback(const FunctionCallbackInfo<Value>& args) {
	gtk_widget_destroy(window);
	gtk_main_quit();
}

void Substrate::onEvent(GtkWidget *widget, gpointer data) {
	HandleScope handle_scope(isolate_);
	Local<Context> context = Local<Context>::New(isolate_, persistentContext);
	Context::Scope context_scope(context);

	UserData *userData = (UserData*)data;

	Handle<Value> callback = String::NewFromUtf8(isolate_, userData->callbackName.c_str());
	Handle<Value> callbackObj = context->Global()->Get(callback);

	if (callbackObj->IsFunction()) {
		Handle<Function> callbackFunc = Handle<Function>::Cast(callbackObj);
		auto uiWidget = ui->GetWidget(widget, userData->objName);
		auto widgetHandle = uiWidget->GetObjectHandle().As<Value>();

		TryCatch tryCatch;
		auto result = callbackFunc->Call(context->Global(), 1, &widgetHandle);
		if (result.IsEmpty()) {
			SubstrateUtils::ShowV8Error(isolate_, window, tryCatch.Exception(), tryCatch.StackTrace());
		}
	}
}
