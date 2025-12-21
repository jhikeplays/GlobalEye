#!/usr/bin/env python3
import argparse
from smbus2 import SMBus, i2c_msg  # pip install smbus2

I2C_ADDR = 0x49

CMD_SET_FRAME = 0x10  # payload: u16 LE frame_ticks
CMD_SET_EXPO  = 0x11  # payload: u16 LE expo_ticks
CMD_SET_BOTH  = 0x12  # payload: u16 LE frame_ticks, then u16 LE expo_ticks

# Timer quantization steps
EXP_STEP_US = 2.5
FRM_STEP_US = 1/30*1000

U16_MAX = 0xFFFF


def write_bytes(bus_num: int, payload: list[int]) -> None:
    with SMBus(bus_num) as bus:
        msg = i2c_msg.write(I2C_ADDR, payload)
        bus.i2c_rdwr(msg)


def to_u16_ticks_from_fps(fps: float) -> tuple[int, float]:
    """
    Convert FPS -> frame ticks (u16) where 1 tick = 240 us.
    Returns (ticks, achieved_fps).
    """
    if fps <= 0:
        raise ValueError("FPS must be > 0")

    period_us = 1_000_000.0 / fps
    ticks = int(round(period_us / FRM_STEP_US))
    if ticks < 1:
        ticks = 1
    if ticks > U16_MAX:
        ticks = U16_MAX

    achieved_fps = 1_000_000.0 / (ticks * FRM_STEP_US)
    return ticks, achieved_fps


def to_u16_ticks_from_exposure_ms(exposure_ms: float) -> tuple[int, float]:
    """
    Convert exposure_ms -> exposure ticks (u16) where 1 tick = 2.5 us.
    Returns (ticks, achieved_exposure_ms).
    """
    if exposure_ms <= 0:
        raise ValueError("Exposure must be > 0 ms")

    exposure_us = exposure_ms * 1000.0
    ticks = int(round(exposure_us / EXP_STEP_US))
    if ticks < 1:
        ticks = 1
    if ticks > U16_MAX:
        ticks = U16_MAX

    achieved_exposure_ms = (ticks * EXP_STEP_US) / 1000.0
    return ticks, achieved_exposure_ms


def set_frame(bus_num: int, frame_ticks: int) -> None:
    payload = [CMD_SET_FRAME, frame_ticks & 0xFF, (frame_ticks >> 8) & 0xFF]
    write_bytes(bus_num, payload)


def set_expo(bus_num: int, expo_ticks: int) -> None:
    payload = [CMD_SET_EXPO, expo_ticks & 0xFF, (expo_ticks >> 8) & 0xFF]
    write_bytes(bus_num, payload)


def set_both(bus_num: int, frame_ticks: int, expo_ticks: int) -> None:
    payload = [
        CMD_SET_BOTH,
        frame_ticks & 0xFF, (frame_ticks >> 8) & 0xFF,
        expo_ticks  & 0xFF, (expo_ticks  >> 8) & 0xFF,
    ]
    write_bytes(bus_num, payload)


def main() -> int:
    ap = argparse.ArgumentParser(
        description=f"Set MSPM0 timers over I2C (fixed address 0x{I2C_ADDR:02X}). "
                    f"Exposure step={EXP_STEP_US}us, Frame step={FRM_STEP_US}us."
    )
    ap.add_argument("-b", "--bus", type=int, default=4,
                    help="I2C bus number (default: 4 => /dev/i2c-4)")
    ap.add_argument("--fps", type=float,
                    help="Frame rate in FPS (e.g. 30, 60, 24.0). Sends frame timer ticks.")
    ap.add_argument("--exposure-ms", type=float,
                    help="Exposure time in milliseconds (e.g. 10, 2.5). Sends exposure timer ticks.")
    ap.add_argument("--quiet", action="store_true",
                    help="Don't print achieved timing")

    args = ap.parse_args()

    if args.fps is None and args.exposure_ms is None:
        ap.error("Provide at least one of --fps or --exposure-ms")

    frame_ticks = None
    expo_ticks = None
    achieved_fps = None
    achieved_exp_ms = None

    if args.fps is not None:
        frame_ticks, achieved_fps = to_u16_ticks_from_fps(args.fps)

    if args.exposure_ms is not None:
        expo_ticks, achieved_exp_ms = to_u16_ticks_from_exposure_ms(args.exposure_ms)

    # If both are provided, optionally clamp exposure to be < frame period (in time)
    if frame_ticks is not None and expo_ticks is not None:
        frame_period_us = frame_ticks * FRM_STEP_US
        expo_us = expo_ticks * EXP_STEP_US
        if expo_us >= frame_period_us:
            # Clamp to just under one frame period
            max_expo_ticks = int((frame_period_us - EXP_STEP_US) // EXP_STEP_US)
            if max_expo_ticks < 1:
                max_expo_ticks = 1
            if max_expo_ticks > U16_MAX:
                max_expo_ticks = U16_MAX
            expo_ticks = max_expo_ticks
            achieved_exp_ms = (expo_ticks * EXP_STEP_US) / 1000.0

    # Send command(s)
    if frame_ticks is not None and expo_ticks is not None:
        set_both(args.bus, frame_ticks, expo_ticks)
    elif frame_ticks is not None:
        set_frame(args.bus, frame_ticks)
    else:
        set_expo(args.bus, expo_ticks)

    if not args.quiet:
        if frame_ticks is not None:
            print(f"Frame: ticks={frame_ticks}  (step {FRM_STEP_US:.4f}us)  achieved_fps={achieved_fps:.6f}")
        if expo_ticks is not None:
            print(f"Expo : ticks={expo_ticks}   (step {EXP_STEP_US}us)  achieved_exposure_ms={achieved_exp_ms:.6f}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
