# rp2350pc

Note : this has been superceded by rp2350-modules. 

This is fundamentally the same thing, and will use mostly the same code, but uses the PicoDVI library rather than HSTX. I could not make HSTX function reliably with USB ; at best it *mostly* worked but would fail periodically. This is way more stable.

https://github.com/paulscottrobson/rp2350-modules

------

This is a user library for the RP2350PC from Olimex. It should work for any other RP2350 based board that uses the standard connections.

The board specification is :

- RP2350 Pico 2 chip
- 4 Port USB hub
- SD Card on board.
- HDMI / DVI connector
- 16Mb Flash
- 8Mb PSRAM
- Stereo audio either using PWM
- ES8311 Audio Codec

