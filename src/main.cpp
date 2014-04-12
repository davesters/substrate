#include <gtk/gtk.h>

#include <substrate.h>

int main(int argc, char* argv[])
{
	Substrate app = Substrate();

	app.init(argc, argv);

	return 0;
}
