#include <v8.h>
#include <gtk/gtk.h>
#include <string>

using namespace v8;
using namespace std;

class Substrate
{
public:
	void init(int argc, char* argv[]);

	static void alertCallback(const FunctionCallbackInfo<Value>& args);
	static void bindEventCallback(const FunctionCallbackInfo<Value>& args);
	static void exitCallback(const FunctionCallbackInfo<Value>& args);
	static void loadWindowCallback(const FunctionCallbackInfo<Value>& args);
	static void loadStylesCallback(const FunctionCallbackInfo<Value>& args);
	static void getWidgetCallback(const FunctionCallbackInfo<Value>& args);

	static void onEvent(GtkWidget *widget, gpointer data);

private:
	Handle<ObjectTemplate> CreateGlobalObject();
	Handle<ObjectTemplate> CreateSubstrateObject();
};

struct UserData { string callbackName; string objName; };
