
#include "platform_pc.h"

ApplicationPlatform* pPlatform;

int main(int argc, char* argv[])
{
    pPlatform = new PlatformPC();
    pPlatform->setup();
    pPlatform->loop();
    delete pPlatform;
    return 0;
}

