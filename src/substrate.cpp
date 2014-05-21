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

void Substrate::init(int argc, char* argv[])
{
	isolate_ = Isolate::GetCurrent();
	Environment::SetIsolate(isolate_);
	HandleScope handle_scope(isolate_);

	Handle<Context> context = Context::New(isolate_, NULL, CreateGlobalObject());
	Ui::SetContext(context);
	Context::Scope context_scope(context);

	Handle<String> source = SubstrateUtils::readJsFile(isolate_, "main.js");
	TryCatch tryCatch;

	Handle<Script> script = Script::Compile(source);
	if (script.IsEmpty()) {
		Ui::ShowV8Error(isolate_, tryCatch.Exception(), tryCatch.StackTrace());
	}

	tryCatch.Reset();
	Handle<Value> result = script->Run();
	if (result.IsEmpty()) {
		Ui::ShowV8Error(isolate_, tryCatch.Exception(), tryCatch.StackTrace());
	}

	gtk_init(&argc, &argv);

	Handle<Value> initObj = context->Global()->Get(String::NewFromUtf8(isolate_, "on_app_ready"));
	Handle<Function> initFunc = Handle<Function>::Cast(initObj);

	tryCatch.Reset();
	result = initFunc->Call(context->Global(), 0, NULL);
	if (result.IsEmpty()) {
		Ui::ShowV8Error(isolate_, tryCatch.Exception(), tryCatch.StackTrace());
	}

	gtk_main();
}

Handle<ObjectTemplate> Substrate::CreateGlobalObject() {
	auto global = ObjectTemplate::New(isolate_);
	global->Set(String::NewFromUtf8(isolate_, "substrate"), CreateSubstrateObject());

	return global;
}

Handle<ObjectTemplate> Substrate::CreateSubstrateObject() {
	auto substrateObj = ObjectTemplate::New(isolate_);

	substrateObj->SetInternalFieldCount(1);
	substrateObj->Set(String::NewFromUtf8(isolate_, "alert"), FunctionTemplate::New(isolate_, alertCallback));
	substrateObj->Set(String::NewFromUtf8(isolate_, "exit"), FunctionTemplate::New(isolate_, exitCallback));
	substrateObj->Set(String::NewFromUtf8(isolate_, "loadMainWindow"), FunctionTemplate::New(isolate_, loadMainWindowCallback));
	substrateObj->Set(String::NewFromUtf8(isolate_, "loadWindow"), FunctionTemplate::New(isolate_, loadWindowCallback));
	substrateObj->Set(String::NewFromUtf8(isolate_, "loadStyles"), FunctionTemplate::New(isolate_, loadStylesCallback));

	return substrateObj;
}

void Substrate::alertCallback(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() < 1) {
		return;
	}

	HandleScope handle_scope(args.GetIsolate());
	Handle<Value> title;

	if (args.Length() > 1) {
		title = args[1];
	}
	else {
		title = String::NewFromUtf8(args.GetIsolate(), "");
	}

	String::Utf8Value msgValue(args[0]);
	String::Utf8Value titleValue(title);

	Ui::ShowMessageDialog(*msgValue);
}

void Substrate::loadMainWindowCallback(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() < 1) {
		return;
	}

	HandleScope handle_scope(args.GetIsolate());
	string windowName(*String::Utf8Value(args[0]));
	auto windowObj = Ui::LoadMainWindow(windowName);

	args.GetReturnValue().Set(windowObj);
}

void Substrate::loadWindowCallback(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() < 1) {
		return;
	}

	HandleScope handle_scope(args.GetIsolate());
	string windowName(*String::Utf8Value(args[0]));
	auto windowObj = Ui::LoadWindow(windowName);

	args.GetReturnValue().Set(windowObj);
}

void Substrate::loadStylesCallback(const FunctionCallbackInfo<Value>& args) {
	HandleScope handle_scope(args.GetIsolate());

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

void Substrate::exitCallback(const FunctionCallbackInfo<Value>& args) {
	gtk_main_quit();
}
