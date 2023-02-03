# Graphics

Even though the STM32 is a relatively powerful microcontroller, it will likely
not be fast enough for running game logic, sound logic and a graphics processor
on it's single CPU core. To minimize the risk of overloading the STM32
processor, all graphics processing and compositing will be outsourced to a
custom PPU implemented on the Basys 3 FPGA.

Because we're creating a sidescrolling 2D platformer, we don't need a very
complicated graphics processor. We also lack any knowledge about how
'real-world' graphics processors work, so we're basing our design on a product
closest to our target result, the NES.

## NES graphics

The NES has a separate coprocessor chip designed to generate video signals
called the RP2C02, which is manufactured by Ricoh. This chip has a lot of
features, some of which we don't need for our game. Here's a quick rundown of
this chip's features and limitations:

- composite output at 256x224 @ 60Hz (NTSC) or 256x240 @ 50Hz (PAL)
- 256 tiles per tilemap (with 8x8 tiles)
- 2 tilemaps
- 4 palettes per sprite with 64 possible colors
- 512x480 background palette with scrolling
- background scrolling splits
- 64 total sprites on screen (8 per scanline)
- sprites can be drawn before or after the background layer
- PPU control using DMA
- 8K character memory (tilemaps)
- 2K nametable memory
- 256B object attribute memory (OAM)
- tiles can be flipped using OAM
- no frame buffer

## Custom PPU

Here's a list of features our PPU should have:
<!-- TODO: expand list with PPU spreadsheet -->

- 256x240 @ 60Hz VGA output
- single tilemap with room for 2048 tiles of 8x8 pixels
- 8 colors per palette, with 4096 possible colors (12-bit color depth)
- 512x480 background palette with scrolling
- **NO** background scrolling splits
- 128 total sprites on screen (**NO** scanline sprite limit)
- sprites are always drawn on top of the background layer
- PPU control using DMA (dual-port asynchronous RAM)
- tiles can be flipped using OAM
- no frame buffer
- vertical and horizontal sync output

[nesppuspecs]: https://www.copetti.org/writings/consoles/nes/
[nesppudocs]: https://www.nesdev.org/wiki/PPU_programmer_reference
[nesppupinout]: https://www.nesdev.org/wiki/PPU_pinout

# Generating audio signals

In order to generate sound for this project, a few posibilities exist (see chapters below)

## Sound chips

A sound chip is made to use digital, analog or mixed signals and produce a tone or sound based on that. 


| Manufacturer       | Chip                                                                                                          | Year | Channels | Stand-alone | Cost/availability |
| ------------------ | ------------------------------------------------------------------------------------------------------------- | ---- | -------- | ----------- | ----------------- |
| Atari, Inc.        | [POKEY](https://en.wikipedia.org/wiki/POKEY)                                                                  | 1979 | 4        | Somewhat    | N/A               |
| General Instrument | [AY-3-8910](https://en.wikipedia.org/wiki/AY-3-8910)                                                          | 1978 | 3        | Yes         | N/A               |
| General Instrument | [SP0250](https://en.wikipedia.org/wiki/General_Instrument_SP0256)                                             | 1981 | 1        | Yes         | N/A               |
| Konami             | [VRC6](https://en.wikipedia.org/wiki/VRC6)                                                                    | 1987 | 3        | Yes         | N/A               |
| Philips            | [Philips SAA1099](https://en.wikipedia.org/wiki/Philips_SAA1099 "Philips SAA1099")                            | 1984 | 6        | Yes         | N/A               |
| Sunsoft 5B         | [Sunsoft 5B](https://en.wikipedia.org/wiki/Memory_management_controller#FME-7 "Memory management controller") | 1992 | 3        | Yes?        | N/A               |
| Texas Instruments  | [SN76477](https://en.wikipedia.org/wiki/SN76477)                                                              | 1978 | 1        | Yes         | N/A               |
| Texas Instruments  | [SN76489](https://en.wikipedia.org/wiki/SN76489 "SN76489")                                                    | 1980 | 4        | Yes         | N/A               |
| Texas Instruments  | [Sega PSG (SN76496)](https://en.wikipedia.org/wiki/SN76496 "SN76496")                                         | 1982 | 4        | Yes         | N/A               | 

This chart shows different audio chips that might have been used in retro consoles. While all of them aren't easily available anymore it is still interesting to take a close look at how these IC's (Intergrated Ciruit) work. 

Most chips work on one of two pricibles, this being either a "Programmable sound generator" or "Sound Chip"


### Programmable sound generator (PSG)

PSG's are a subset of sound chips that generate sound based on a set of pre programmed tones they can generate. For example, this can be done by: modulating a input square wave to a different frequency and or amplitude.

### Pulse-code modulation

Pulse-code modulation uses pre-sampled data to generate sounds. This causes a lot more work for the engineer before gaining a working system. The flip side of that coin is that the Output can be of a better quality (depending on the sampled audio used).

## FPGA Circuit

