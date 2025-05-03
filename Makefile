probers-download:
	probe-rs download .\build\Debug\mechtruder-testg431.elf --chip STM32G431CBUx --speed 50000
	probe-rs reset --chip STM32G431CBUx

probers-run:
	probe-rs run .\build\Debug\mechtruder-testg431.elf --chip STM32G431CBUx --speed 50000

probers-reset:
	probe-rs reset --chip STM32G431CBUx

# PyOCD is broken for STM32G4 and YaPicoProbe
pyocd-download:
	pyocd flash -t stm32g431cbux -vv .\build\Debug\mechtruder-testg431.elf

openocd-download:
	openocd -f interface/cmsis-dap.cfg -f target/stm32g4x.cfg -c "adapter_khz 50000; program ./build/Debug/mechtruder-testg431.elf verify reset exit"