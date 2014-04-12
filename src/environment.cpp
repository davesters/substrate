#include <v8.h>

#include <environment.h>

using namespace v8;

static Isolate* isolate_;

Isolate* Environment::GetIsolate() {
	return isolate_;
}

void Environment::SetIsolate(Isolate* isolate) {
	isolate_ = isolate;
}
