# MSPM0 I2C Timer Control (GMAX3412 Trigger)

A small command-line tool to configure the **MSPM0C1104** trigger generator over **I2C** (fixed address **0x49**).

It converts human-friendly inputs:
- **Frame rate (FPS)** → **frame timer ticks**
- **Exposure (ms)** → **exposure timer ticks**

…and sends the corresponding commands to the MCU.

---

## What it controls

The MCU firmware expects **16-bit little-endian timer LOAD values** (“ticks”) over I2C:

| Command | Value | Payload |
| --- | --- | --- |
| Set frame period | `0x10` | `u16 LE frame_ticks` |
| Set exposure | `0x11` | `u16 LE expo_ticks` |
| Set both | `0x12` | `u16 LE frame_ticks`, then `u16 LE expo_ticks` |

I2C target address is fixed:

- `0x49`

---

## Timing quantization

The MCU timers are quantized, so the achieved settings may differ slightly from what you request.

- **Exposure step**: `2.5 µs` per tick  
- **Frame step**: `FRM_STEP_US = 1/30*1000` µs per tick (as defined in the script)

The tool prints the **achieved** FPS and exposure time after rounding.

> Note: If both FPS and exposure are provided, exposure is clamped to **just under one frame period** to avoid an invalid pulse.

---

## Requirements

- Python 3
- I2C enabled on your host (e.g. Raspberry Pi: `/dev/i2c-1`)
- `smbus2`

Install dependency:

```bash
pip install smbus2
```

## Usage

### Set frame rate only
```bash
./set.py --fps 10
```
### Set exposure only
```bash
./set.py --exposure-ms 5
```
### Set both (recommended)
```bash
./set.py --fps 30 --exposure-ms 10
```
### Select a different I2C bus
Default bus is 4 (/dev/i2c-1). For bus 6:
```bash
./set.py --bus 6 --fps 24 --exposure-ms 2.5
```
