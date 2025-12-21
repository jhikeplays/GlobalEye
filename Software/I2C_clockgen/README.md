## Firmware Summary (MSPM0C1104)

This is **I2C-controlled trigger generator** for the GMAX3412 board.

It uses:
- **Timer FRM** to generate the **rising edge** (start of frame / exposure)
- **Timer EXP** to generate the **falling edge** (end of exposure)
- **I2C target** interface to accept commands from a host (e.g. Raspberry Pi)


## What it does

- The sensor needs an **external trigger pulse**.
- The firmware outputs a pulse where:
  - **Rising edge** = start new frame
  - **Pulse width** = exposure time
- The host sets timing over I2C using **16-bit timer ticks** (already computed host-side).

## I2C Protocol (16-bit ticks)

The device is an **I2C target**. Commands are:

| Command | Value | Payload |
| --- | --- | --- |
| `CMD_SET_FRAME` | `0x10` | `u16` little-endian (`frame_ticks`) |
| `CMD_SET_EXPO`  | `0x11` | `u16` little-endian (`expo_ticks`) |
| `CMD_SET_BOTH`  | `0x12` | `u16` LE `frame_ticks`, then `u16` LE `expo_ticks` |

Notes:
- Payload values are **timer LOAD values** in **ticks**.
- The device applies updates in the main loop (not inside the ISR) to keep timing stable.


## Peripherals & Pin Assignments

> The exact timer/I2C pin mux depends on your SysConfig settings (`ti_msp_dl_config.h`), but this project uses:
- `TIMER_FRM_INST` : periodic frame timer (rising edge)
- `TIMER_EXP_INST` : one-shot exposure timer (falling edge)
- `I2C_INST` : I2C target (RX commands)

## Example Usage
Build + flash with CCS / UniFlash (standard MSPM0 flow).
