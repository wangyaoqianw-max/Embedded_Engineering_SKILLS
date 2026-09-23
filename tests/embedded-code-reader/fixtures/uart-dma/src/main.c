#include "app_config.h"
#include "uart_service.h"

int main(void)
{
    uart_service_start();
    uart_rx_task(0);
}
