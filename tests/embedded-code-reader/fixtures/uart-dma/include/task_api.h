#ifndef TASK_API_H
#define TASK_API_H

void xTaskNotifyFromISR(void *task, unsigned value);
void ulTaskNotifyTake(void);
void uart_rx_task(void *argument);

#endif
