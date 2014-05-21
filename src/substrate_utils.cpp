#include <v8.h>
#include <gtk/gtk.h>
#include <string>

#include <substrate_utils.h>

using namespace v8;
using namespace std;

Handle<String> SubstrateUtils::readJsFile(Isolate* isolate, const string& name) {
	FILE* file = fopen(name.c_str(), "rb");
	if (file == NULL) return Handle<String>();

	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	rewind(file);

	char* chars = new char[size + 1];
	chars[size] = '\0';
	for (int i = 0; i < size;) {
		int read = static_cast<int>(fread(&chars[i], 1, size - i, file));
		i += read;
	}

	fclose(file);
	Handle<String> result = String::NewFromUtf8(isolate, chars, String::kNormalString, size);
	delete[] chars;
	return result;
}

string SubstrateUtils::readFile(const string& name) {
	FILE* file = fopen(name.c_str(), "rb");
	if (file == NULL) return "";

	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	rewind(file);

	char* chars = new char[size + 1];
	chars[size] = '\0';
	for (int i = 0; i < size;) {
		int read = static_cast<int>(fread(&chars[i], 1, size - i, file));
		i += read;
	}

	fclose(file);
	string result(chars);
	delete[] chars;

	return result;
}
