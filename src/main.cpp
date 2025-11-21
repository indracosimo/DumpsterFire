#include "core/application.h"
#include "core/config.h"

int main() 
{
    application app(config::HD.width, config::HD.height, "DumpsterFire");
	app.Run();
    return 0;
}