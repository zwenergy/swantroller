# swantroller
A WonderSwan Color controller PCB for the swancolorHD.

<img src="./res/swantrollere3.png" alt="drawing" width="700"/>

## About
The idea of this project is to turn an (empty) WonderSwan Color shell into a controller for the swancolorHD (a WonderSwan Color consolizer).

## Variants
There are two variants of the swantroller: One variant based on shift registers and one variant based on an RP2040 microcontroller.
* **Shift Register variant (CD4021):** PCB Gerbers can be found in the 4021 folder.
This variant has the advantage that it is fully working, also if there is some under-voltage or under-current for a short time at the startup.
This is a particular possibility with the swancolorHD, as the board may be crossfed by the HDMI voltage.
The disadvantage of this variant is that the controller protocol is fixed, making it only usefully working with the swancolorHD and not usable with, e.g., a SNES console.
* **RP240 variant:** PCB Gerbers can be found in the rp2040/pcb folder.
FW can be found in the release tab.
This variant has the advantage that the controller protocol can be fully adjusted in SW, making it potentially to be used with, e.g., a SNES console.
The disadvantage is that it is susceptible to under-voltage or under-current.
In case of the swancolorHD, it may happen that it is crossfed via the HDMI voltage when turned off, leading to a faulty bootup and then being stuck.
If the controller is conneceted when the swancolorHD is already powered on (or if the crossfed power in the off-state does not happen, e.g., with many modern Samsung TVs), it works fine.

## Parts
### Shift Register Variant
| **Reference** | **Value**| **Links**
|---------------|----------|----------|
| U1, U2        | CD4021 Shift Register |[LCSC](https://www.lcsc.com/product-detail/Shift-Registers_Texas-Instruments-CD4021BM96_C134120.html)|
| RN1, RN2           | 10 kOhm Resistor Array |[LCSC](https://www.lcsc.com/product-detail/Resistor-Networks-Arrays_UNI-ROYAL-Uniroyal-Elec-16P8WGF1002T4E_C422188.html)|

### RP2040 Variant
| **Reference** | **Value**| **Links**
|---------------|----------|----------|
| U2        | RP2040 Zero ||
| Q1, Q2, Q3 | N-MOSFET (SOT-23) | [LCSC](https://www.lcsc.com/product-detail/MOSFETs_Shikues-BSS138_C112239.html) |
| R1, R2, R3, R4, R5, R6 | 10 kOhm resistor (0805) | [LCSC](https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0805W8F1002T5E_C17414.html) |


Besides you need an **empty WonderSwan Color shell** (a replacement shell or an original shell with the display still installed should work) and a **SNES controller extension cable**.

## PCB
The controller PCB should be ordered with 1 mm thickness and ENIG.

## Connection
The easiest way to get a SNES controller cable is probably to get a SNES controller extension cable and cut off the female port.
Use a multimeter to figure out the signals.
Here's the signal pinout of the controller plug:

![SNES pinput](./res/snes.png "SNES pinout")

## Disclaimer
**Use the files and/or schematics to build your own board at your own risk**.
This board works fine for me, but it's a simple hobby project, so there is no liability for errors in the schematics and/or board files.
**Use at your own risk**.

