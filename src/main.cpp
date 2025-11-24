#include "core/application.h"
#include "core/config.h"

int main() 
{
    application app(Resolution(config::SD.width, config::SD.height), "DumpsterFire");
	app.Run();
    return 0;
}