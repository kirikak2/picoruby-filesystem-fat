# picoruby-filesystem-fat

FAT filesystem driver for PicoRuby's VFS, built on FatFs (R0.14b). It is
used for SD cards on ESP32 (the card stays readable on a PC), next to
`picoruby-littlefs` on the internal flash.

Upstream PicoRuby removed this gem in July 2026 (it uses littlefs for every
volume). This copy is maintained with
[MIDoRI](https://github.com/kirikak2/midori), which needs FAT on the SD card.

## Usage

```ruby
# SPI-attached card
spi = SPI.new(unit: :ESP32_SPI2_HOST, frequency: 5_000_000,
              sck_pin: 36, cipo_pin: 35, copi_pin: 37, cs_pin: 4)
sd = FAT.new(:sd, label: "SD", driver: spi)
VFS.mount(sd, "/sd")

# SDMMC-attached card (1-bit). sdmmc_slot / sdmmc_freq_khz are optional;
# -1 (the default) lets the port choose.
sdmmc = SDMMC.new(clk_pin: 43, cmd_pin: 44, d0_pin: 39)
sd = FAT.new(:sd, label: "SD", driver: sdmmc,
             sdmmc_slot: 0, sdmmc_freq_khz: 10_000)
VFS.mount(sd, "/sd")

File.open("/sd/hello.txt", "w") { |f| f.write("Hello") }
```

`FAT.new(device, label:, driver:, sdmmc_slot:, sdmmc_freq_khz:)`

- `device` — FatFs volume: `:sd` (`:ram` also exists, for a RAM disk)
- `driver` — an `SPI` or `SDMMC` object, or `nil` if nothing needs setting up.
  Only the pin accessors are read, so any class providing them will do.
- `sdmmc_slot` — the SDMMC host slot the card is wired to. The pins do not
  always say which: on the ESP32-P4 the slot 0 pins are IOMUX-only, so a card
  on them is unreachable through slot 1.
- `sdmmc_freq_khz` — bus clock ceiling

`mkfs`, `getlabel` / `setlabel`, `sector_count`, `erase` and the usual
VFS driver methods are available.

## Building it in

The Ruby side and the VM bindings (`src/mruby`, `src/mrubyc`) are built by
the gem. The port (`ports/esp32/sd_disk.c`) needs ESP-IDF, so the host
project compiles it, with `ports/esp32`, `include` and `src/hal` on its
include path. Define `USE_FAT_SD_DISK` on both sides to enable the SD
volume.

ESP32 is the only port. The flash disk the upstream gem had is gone: it
used the same fixed flash range as `picoruby-littlefs`'s ESP32 port.

### Next to picoruby-littlefs

Both gems can be in one build. The C symbols they used to share are prefixed
here (`mrb_fat__exist_p`, `c_fat__exist_q`, `mrb_fat_vfs_methods_type`, ...).

### VM teardown

`ff_clear_volumes()` drops FatFs' volume table. Call it when a VM is thrown
away without being closed: the table lives outside the VM heap but points at
FATFS objects inside it.

## License

MIT (the gem). FatFs is under its own BSD-style license, see
`lib/ff14b/LICENSE.txt`.
