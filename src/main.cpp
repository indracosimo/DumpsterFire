#include "core/application.h"
#include "core/config.h"

int main() 
{
    application app(Resolution(config::UHD4K.width, config::UHD4K.height), "DumpsterFire");
	app.Run();
    return 0;
}