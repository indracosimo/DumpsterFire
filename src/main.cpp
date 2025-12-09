#include "core/application.h"
#include "core/config.h"
#include "../MessageManager.h"

int main() 
{
    application app(Resolution(config::HD.width, config::HD.height), "DumpsterFire");

    //Message Manager testing
    RaycastMessage raycastMsg(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), 100.0f);
    raycastMsg.processMessage();
    StringMessage stringMsg("Hello, this is a string message!");

    stringMsg.processMessage();

    FloatMessage floatMsg(42.0f);
    floatMsg.processMessage();

    MouseMessage mouseMsg(100, 200, true);
    mouseMsg.processMessage();
    //Message Manager testing

	app.Run();



    return 0;
}