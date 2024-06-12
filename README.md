# g431_minimal

Minimal STM32 CubeMX CMake project with ThreadX/USBX and SEGGER RTT functionality.

The application code located in [`Lib/Application/application.c`](Lib/Application/application.c) sets up ThreadX and USBX with a simple USB CDC ACM device. 

## Prerequisites

It is recommended to install the [STM32CubeCLT](https://www.st.com/en/development-tools/stm32cubeclt.html) software suite in conjunction with the STM32CubeMX tool. The code can then be generated for CMake, Make, EWARM or STM32CubeIDE. I recommend using the CMake option with the [STM32 VSCode Extension](https://marketplace.visualstudio.com/items?itemName=stmicroelectronics.stm32-vscode-extension) since I've set up tasks for building and debugging the code (flashing only via a [PowerShell Script](.vscode/flash.ps1)).

I personally used the [STM32G431CoreBoard](https://github.com/WeActStudio/WeActStudio.STM32G431CoreBoard) from WeActStudio, but you can use any board which has the USB peripheral connected to the USB pins. The LED is setup for this boards' LED on pin `C6`.

## Firmware Description

### Threads

There are two default application threads:

- The [`Main Thread`](Lib/Application/application.c#L14-L28) which blinks the LED on the board and sends a message over RTT every second.
- The [`USB Thread`](Lib/Application/application.c#L30-L49) which waits until a message is received over USB CDC ACM and echoes it back via CDC and RTT.

### USB CDC ACM

The default stdio functions are redirected over USB CDC ACM. This means that you can use `printf` and `gets` and similar functions in your code and the output/input will be redirected over USB. The redirection is set up in [`Core/Src/main.c`](Core/Src/main.c#L234-L262).

> [!WARNING]
> If you send a message over CDC, make sure that the line is terminated with a newline character (`\r\n`) as the USB functionality expects it.

### Logging

There is a logging functionality via RTT available in [`Lib/Logging/logging.h`](Lib/Logging/logging.h) which is used in the application code. There are five log levels available:
- `LOG_LEVEL_NONE`: No logging
- `LOG_LEVEL_ERROR`: Only errors
- `LOG_LEVEL_WARNING`: Errors and warnings
- `LOG_LEVEL_INFO`: Errors, warnings and info
- `LOG_LEVEL_DEBUG`: All logs

> [!TIP]
> You have to use a SEGGER RTT viewer to see the messages sent over RTT. The default viewer is available in the [SEGGER J-Link software](https://www.segger.com/downloads/jlink/). An alternative is via [pyOCD](https://pyocd.io/).
>
> Example output:
> ```bash
> DEB thread_main_entry:17          Main Thread started
> DEB thread_main_entry:23          Counter : 0
> DEB thread_usb_entry:33   USB Thread started
> DEB thread_main_entry:23          Counter : 1
> DEB thread_main_entry:23          Counter : 2
> INF thread_usb_entry:44   You typed: 'help'
> DEB thread_main_entry:23          Counter : 3
> DEB thread_main_entry:23          Counter : 4
> DEB thread_main_entry:23          Counter : 5
> ```
