#include "application.h"
#include "main.h"
#include "ux_device_cdc_acm.h"

#include "logging.h"

#define MS_TO_TICKS(ms) ((ms * TX_TIMER_TICKS_PER_SECOND / 1000))

TX_THREAD thread_main_handle;
ULONG thread_main_stack[1024 / sizeof(ULONG)];
TX_THREAD thread_usb_handle;
ULONG thread_usb_stack[1500 / sizeof(ULONG)];

VOID thread_main_entry(ULONG thread_input)
{
    UNUSED(thread_input);
    LOG_D("Main Thread started");

    uint8_t counter = 0;

    while (1)
    {
        LOG_D("Counter : %u", counter++);
        HAL_GPIO_TogglePin(LED_BUILTIN_GPIO_Port, LED_BUILTIN_Pin);
        // sleep 100ms
        tx_thread_sleep(MS_TO_TICKS(1000));
    }
}

VOID thread_usb_entry(ULONG thread_input)
{
    UNUSED(thread_input);
    LOG_D("USB Thread started");

    while (1)
    {
        char buffer[64] = {0};
        gets(buffer);

        if (strlen(buffer) > 0)
        {
            // remove trailing newline
            buffer[strlen(buffer) - 1] = '\0';
            LOG_I("You typed: '%s'", buffer);
        }

        tx_thread_sleep(MS_TO_TICKS(100));
    }
}

void stack_error_handler(TX_THREAD *ptr)
{
    LOG_E("Stack error on thread: %s", ptr->tx_thread_name);

    while (1)
    {
    }
}

UINT application_define(VOID *memory_ptr)
{
    UNUSED(memory_ptr);
    UINT ret = TX_SUCCESS;

    ret = tx_thread_stack_error_notify(stack_error_handler);
    if (ret != TX_SUCCESS)
    {
        return ret;
    }

    ret = tx_thread_create(&thread_main_handle, "Main Thread", thread_main_entry, 0, thread_main_stack, sizeof(thread_main_stack), 2, 2, TX_NO_TIME_SLICE, TX_AUTO_START);
    if (ret != TX_SUCCESS)
    {
        return ret;
    }

    ret = tx_thread_create(&thread_usb_handle, "USB Thread", thread_usb_entry, 0, thread_usb_stack, sizeof(thread_usb_stack), 4, 4, 10, TX_AUTO_START);
    if (ret != TX_SUCCESS)
    {
        return ret;
    }

    return ret;
}