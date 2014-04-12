#include <v8.h>

using namespace v8;

class Environment {
public:
	static Isolate* GetIsolate();
	static void SetIsolate(Isolate* isolate);
};
