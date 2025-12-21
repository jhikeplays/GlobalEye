# GMAX3412 breakout board

A basic minimum **GMAX3412** (1" / 4096×3072 / Global Shutter) board for Raspberry Pi.

<img width="1280" alt="image" src="https://github.com/user-attachments/assets/07d92453-3e88-4c81-9928-7470d9cfda32" />

In general this is a simple breakout board for the GMAX3412 sensor — see the driver repo [here](https://github.com/will127534/gmax3412-v4l2-driver).

The sensor itself is quite interesting: it needs a trigger pulse to start a new frame, and it can’t generate that pulse by itself. Because of that, this board includes a MSPM0C1104 MCU to generate the signal and listen for commands over I2C.

The MCU firmware and the Python script to talk to it live under `software/`.  
But if you are planning to send the clock/trigger via other means, you can ignore that and use the U.FL connector to feed signals in.

The driver is set up as **external exposure**:
- **Rising edge** triggers a new frame
- **Pulse width** is the exposure duration

## Repo layout

- `Gerbers/` — manufacturing files
- `bom/` — BOM / parts list
- `software/` — MSPM0 firmware + Python I2C control script
- `datasheet/` — datasheet backup copy

## Images

<img width="1280" alt="image" src="https://github.com/user-attachments/assets/53eb4a42-8ea5-4f12-b764-6d9b37767cd4" />
<img width="1280" alt="image" src="https://github.com/user-attachments/assets/5b95bd54-53f1-42e3-ba69-38ca70e62af9" />

See it in action here: [Youtube](https://www.youtube.com/watch?v=J_Mvx6Y6Drg).  
The board is tested with Raspberry Pi 5.  

The trigger signal looks like this:  
<img width="1280" alt="image" src="https://github.com/user-attachments/assets/56f11915-6237-4873-89a1-e6653319db5b" />
Yellow is the TEXP, and blue one is the TDIG output from the sensor showing "Frame Overhead Time".

<img width="1280" alt="image" src="https://github.com/will127534/GlobalEye/blob/5182ffc99479f624bad5b5fabc79d2c9a3974b81/pcb.jpg" />
<img width="1280" alt="image" src="https://github.com/user-attachments/assets/c2645506-5cdd-45c0-bff6-32828b94745d" />

## Datasheet

It is not every day you see a detailed datasheet directly from the manufacture appared on the top search result:  
https://gpixel.com/public/uploads/admin/file/pdf-7917552d763a512ce4860ce10e70934f.pdf

…but I also keep a backup under the `datasheet/` folder.

## Notes

Final note is that the power consumption is a step higher then normal rolling shutter sensors, so the board might be a bit warm when operating.
