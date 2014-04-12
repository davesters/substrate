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

void SubstrateUtils::ShowV8Error(Isolate* isolate, GtkWidget* window, Local<Value> exception, Local<Value> stackTrace) {
	auto exception_str = String::Concat(exception->ToString(), String::NewFromUtf8(isolate, "; Stack: "));
	exception_str = String::Concat(exception_str, stackTrace->ToString());

	ShowMessageDialog(window, *String::Utf8Value(exception_str));
}

void SubstrateUtils::ShowMessageDialog(GtkWidget* window, char* message) {
	auto dialog = gtk_message_dialog_new(
		(GtkWindow*)window,
		GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_MESSAGE_INFO,
		GTK_BUTTONS_OK,
		message);

	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}