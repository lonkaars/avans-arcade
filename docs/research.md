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
- 2 tilemaps (pattern tables)
- 4 palettes per sprite with 64 possible colors
- 512x480 background canvas with scrolling
- background scrolling splits
- 64 total sprites on screen (max 8 per scanline)
- sprites can be drawn before or after the background layer
- PPU control using DMA
- 8K character memory (tilemaps)
- 2K nametable memory (OAM for background tiles)
- 256B object attribute memory (OAM)
- tiles can be flipped using OAM
- no frame buffer

### Usage

The NES PPU has a lot of capabilities, so here's a quick run-down of how the
PPU is used by games to produce pictures.

On boot, the NES copies the contents of the so-called CHR-ROM (in game
cartridge) into the PPU's pattern tables. The NES has two 256-tile tilemaps
called the pattern tables which store all sprites in the game. By modifying the
nametable memory directly, the game can control which tile, and which palette
will be used for any given tile on the background layer. Because the background
layer only displays tiles in a fixed grid, the nametable memory area is not
very big.

The same process happens for the foreground sprites, though these can each be
positioned using screen coordinates, and don't have to conform to any grid. The
NES also has some limitations on how many sprites can be drawn and how many
palettes can be used. An important note is that the first color of any palette
used on any foreground sprite, is treated as the transparency color. Our PPU
also copies this behavior.

The following is a small section of pseudocode, depicting a program that will
display a triangle moving in a circle:

```c
unsigned frame = 0;

void setup() {
  // copy character rom to PPU
  memcpy(CHR, PPU->CHR, 0x8000);
  // character rom contains a sprite of a triangle at index 0

  // set palette index 0 to have 4 random colors
  PAL[0] = (palette) {
    PALETTE_COLOR_25,
    PALETTE_COLOR_F3,
    PALETTE_COLOR_00,
    PALETTE_COLOR_3D,
  };
  // copy palette data to PPU
  memcpy(PAL, PPU->PAL, 0x40);
}

void loop() {
  frame++; // increment frame counter

  OAM[0] = (sprite) {
    .x = sin(frame) * 20 + 10, // calculate circle position using frame counter
    .y = cos(frame) * 20 + 10, // calculate circle position using frame counter
    .pattern_index = 0, // triangle sprite
    .palette_index = 0, // palette 0 (see setup)
    .attributes = PPU_FX_FLIP_H | PPU_FX_FLIP_V, // flip horizontally and vertically
  };

  memcpy(OAM, PPU->OAM, 0x100); // update PPU with local copy of OAM
}

int main() {
  setup();
  while(1) loop();
}
```

## Custom PPU

Here's a list of features our PPU should have:
<!-- TODO: expand list with PPU spreadsheet -->

- 256x224 @ 60Hz VGA output
- single tilemap with room for 1024 tiles of 16x16 pixels
- 8 colors per palette, with 4096 possible colors (12-bit color depth)
- 512x448 background canvas with scrolling
- NO background scrolling splits
- 128 total sprites on screen (NO scanline sprite limit)
- sprites are always drawn on top of the background layer
- PPU control using DMA (dual-port asynchronous RAM)
- tiles can be flipped using FAM or BAM
- no frame buffer
- vertical and horizontal sync output

Notable differences:

- NES nametable equivalent is called BAM (background attribute register)
- NES OAM equivalent is called FAM (foreground attribute register)
- No scanline sprite limit  
  
  Unless not imposing any sprite limit makes the hardware implementation
  impossible, or much more difficult, this is a restriction that will likely
  lead to frustrating debugging sessions, so will not be replicated in our
  custom PPU.
- Sprites are 16x16
  
  Most NES games already tile multiple 8x8 tiles together into "metatiles" to
  create the illusion of larger sprites. This was likely done to save on memory
  costs as RAM was expensive in the '80s, but since we're running on an FPGA
  cost is irrelevant.
- Single 1024 sprite tilemap shared between foreground and background sprites
  
  The NES OAM registers contain a bit to select which tilemap to use (of two),
  which effectively expands each tile's index address by one byte. Instead of
  creating the illusion of two separate memory areas for tiles, having one
  large tilemap seems like a more sensible solution to indexed tiles.
- 8 total palettes, with 8 colors each
  
  More colors is better. Increasing the total palette count is a very memory
  intensive operation, while increaing the palette color count is likely slower
  when looking up color values for each pixel on real hardware.
- Sprites can be positioned paritally off-screen on all screen edges using only
  the offset bits in the FAM register
  
  The NES has a separate PPUMASK register to control special color effects, and
  to shift sprites off the left and top screen edges, as the sprite offsets
  count from 0. Our PPU's FAM sprite offset bits count from -15, so the sprite
  can shift past the top and left screen edges, as well as the standard bottom
  and right edges.
- No status line register, only V-sync and H-sync outputs are supplied back to
  CPU
  
  The NES status line register contains some handy lines, such as a buggy
  status line for reaching the max sprite count per scanline, and a status line
  for detecting collisions between background and foreground sprites. Our PPU
  doesn't have a scanline limit, and all hitbox detection is done in software.
  Software hacks involving swapping tiles during a screen draw cycle can still
  be achieved by counting the V-sync and H-sync pulses using interrupts.
- No background scrolling splits
  
  This feature allows only part of the background canvas to be scrolled, while
  another portion stays still. This was used to draw HUD elements on the
  background layer for displaying things like health bars or score counters.
  Since we are working with a higher foreground sprite limit, we'll use regular
  foreground sprites to display HUD elements.
- Sprites are always drawn on top of the background layer
  
  Our game doesn't need this capability for any visual effects. Leaving this
  feature out will lead to a simpler hardware design

### Hardware design schematics

#### Top (level 1)

![PPU top-level design](../assets/ppu-level-1.svg)

Important notes:

- The STM32 receives 'feedback' lines from the PPU. These are mirrors of the
  VSYNC and HSYNC lines from the VGA signal generator. These lines can be used
  to trigger interrupts for counting frames, and to make sure no read/write
  conflicts occur for protected memory regions in the PPU.
- The STM32 can enable and reset the PPU. These lines will also be connected to
  physical buttons on the FPGA.
- The STM32 uses direct memory access to control the PPU.

#### Level 2

![PPU level 2 design (data flows from top to bottom)](../assets/ppu-level-2.svg)

Important notes:

- The pixel fetch logic is pipelined in 5 stages:
  1. - (Foreground sprite info) calculate if foreground sprite exists at
     current pixel using FAM register
     - (Background sprite info) get background sprite info from BAM register
  3. - (Sprite render) calculate pixel to read from TMM based on sprite info
  5. - (Compositor) get pixel with 'highest' priority (pick first foreground
     sprite with non-transparent color at current pixel in order, fallback to
     background)
     - (Palette lookup) lookup palette color using palette register
     - (VGA signal generator) output real color to VGA signal generator
- The pipeline stages with two clock cycles contain an address set and memory
  read step.
- The pipeline takes 5 clock ticks in total. About 18 are available during each
  pixel. For optimal display compatibility, the output color signal should be
  stable before 50% of the pixel clock pulse width (9 clock ticks).
- Since the "sprite info" and "sprite render" steps are fundamentally different
  for the foreground and background layer, these components will be combined
  into one for each layer respectively. They are separated in the above diagram
  for pipeline stage illustration.
- The BAX, FAM, and PAL registers are implemented in the component that
  directly accesses them, but are exposed to the PPU RAM bus for writing.
- Each foreground sprite render component holds its own sprite data copy from
  the RAM in it's own cache memory. The cache updates are fetched during the
  VBLANK time between each frame.

#### Level 3

This diagram has several flaws, but a significant amount of time has already
been spent on these, so they are highlighted here instead of being fixed.

![PPU level 3 design](../assets/ppu-level-3.svg)

Flaws:

- Pipeline stages 1-4 aren't properly connected in this diagram, see level 2
  notes for proper functionality
- The global RESET input resets all PPU RAM, but isn't connected to all RAM
  ports
- All DATA inputs on the same line as an ADDR output are connections to a
  memory component. Not all of these are connected in the diagram, though they
  should be.

Important notes:

- The background sprite and foreground sprite component internally share some
  components for coordinate transformations
- The foreground sprite component is only shown once here, but is cloned for
  each foreground sprite the PPU allows.
- The CIDX lines between the sprite and compositor components is shared by all
  sprite components, and is such tri-state. A single sprite component outputs a
  CIDX signal based on the \*EN signal from the compositor.
- All DATA and ADDR lines are shared between all RAM ports. WEN inputs are
  controlled by the address decoder.

### Registers

|Address|Size (bytes)|Alias|Description|
|-|-|-|-|
|`0x00000`|`0x00000`|TMM  |[tilemap memory][TMM]|
|`0x00000`|`0x00000`|BAM  |[background attribute memory][BAM]|
|`0x00000`|`0x00000`|FAM  |[foreground attribute memory][FAM]|
|`0x00000`|`0x00000`|PAL  |[palettes][PAL]|
|`0x00000`|`0x00000`|BAX  |[background auxiliary memory][BAX]|

[TMM]: #tilemap-memory
#### Tilemap memory

- TODO: list format

[BAM]: #background-attribute-memory
#### Background attribute memory

- TODO: list format

[FAM]: #foreground-attribute-memory
#### Foreground attribute memory

- TODO: list format

[PAL]: #palettes
#### Palettes

- TODO: list format

[BAX]: #background-auxiliary-memory
#### Background auxiliary memory

- background scrolling

[nesppuspecs]: https://www.copetti.org/writings/consoles/nes/
[nesppudocs]: https://www.nesdev.org/wiki/PPU_programmer_reference
[nesppupinout]: https://www.nesdev.org/wiki/PPU_pinout
[custompputimings]: https://docs.google.com/spreadsheets/d/1MU6K4c4PtMR_JXIpc3I0ZJdLZNnoFO7G2P3olCz6LSc

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

