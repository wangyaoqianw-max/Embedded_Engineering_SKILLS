#include "main.h"

volatile uint32_t fixture_heartbeat;

int main(void)
{
    for (;;) {
        fixture_heartbeat++;
    }
}
