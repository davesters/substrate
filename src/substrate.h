#include <v8.h>
#include <gtk/gtk.h>
#include <string>
#include <widgets/st_widget.h>

using namespace v8;
using namespace std;

class Substrate
{
public:
	void init(int argc, char* argv[]);

	static void alertCallback(const FunctionCallbackInfo<Value>& args);
	static void exitCallback(const FunctionCallbackInfo<Value>& args);
	static void loadMainWindowCallback(const FunctionCallbackInfo<Value>& args);
	static void loadWindowCallback(const FunctionCallbackInfo<Value>& args);
	static void loadStylesCallback(const FunctionCallbackInfo<Value>& args);

private:
	Handle<ObjectTemplate> CreateGlobalObject();
	Handle<ObjectTemplate> CreateSubstrateObject();
};
