# STM32 Hardware-Agnostic Firmware

Firmware decoupled from hardware using a HAL to enable testing without physical devices.

## Project Structure
- `common/` — HAL interface (hal_io.h)
- `app/` — Hardware-agnostic logic (main_logic.c)
- `hw_sim/` — PC simulator implementation
- `hw_stm32/` — Real STM32 hardware implementation
- `tests/` — Test files

## Running on host (no board needed)
Requirements: gcc, make
```bash
make sim    # build simulator
make run    # build and run
make clean  # remove binary
```

## Running on STM32
Open `STM_Agnostic_HIL/` in STM32CubeIDE and flash normally.