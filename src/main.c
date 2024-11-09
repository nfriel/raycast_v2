#include "../lib/start.h"
#include "../lib/loop.h"

int main(void)
{
    if (!start()) return 1;

    loop();

    stop();

    return 0;
}
