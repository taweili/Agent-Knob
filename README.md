# ESP32-S3 Scroll Wheel Controller

An ESPHome-powered rotary encoder device with a 1.8" 360×360 round display (JC3636K518C), integrating with Home Assistant.

## Hardware

| Component | Details |
|---|---|
| **MCU** | ESP32-S3 (esp32-s3-devkitc-1, 16 MB flash, octal PSRAM) |
| **Display** | 1.8" JC3636K518C, 360×360 QSPI round display |
| **Touch** | CST816 capacitive touchscreen (I²C) |
| **Input** | Rotary encoder on GPIO7 / GPIO8 (EXT1 wakeup, implemented as two `binary_sensor` GPIOs with `allow_other_uses`) |
| **Backlight** | Monochromatic PWM on GPIO47, 70% default brightness, 30 ms fade |
| **Fonts** | Roboto 20 / 32 / 88, fetched from Google Fonts |

### Pin Mapping

| Pin | Function |
|---|---|
| GPIO7 | Encoder A (EXT1 wakeup) |
| GPIO8 | Encoder B (EXT1 wakeup) |
| GPIO9 | Touchscreen interrupt |
| GPIO10 | Touchscreen reset |
| GPIO11 | I²C SDA |
| GPIO12 | I²C SCL |
| GPIO13 | QSPI clock |
| GPIO14 | Display CS (QSPI) |
| GPIO15–18 | QSPI data (4 lines) |
| GPIO21 | Display reset |
| GPIO47 | Display backlight PWM |

## Features

- **Page 1 — Deepseek TUI**: Placeholder page showing "Deepseek TUI".
- **Page 2 — Percentage Control**: Scroll to adjust a 0–100% value shown as an arc ring with a centered percentage readout. Defaults to 50%.
- **Page 3 — Color Picker**: Scroll to cycle through 10 colors (Red, Green, Blue, Yellow, Cyan, Magenta, White, Orange, Purple, Pink). Shows color name on screen.
- **Swipe navigation**: Swipe left/right to move between pages.
- **Anti-burn**: Scheduled screen-saver (snow pattern) runs in four 30-minute bursts — 02:05–02:35, 03:05–03:35, 04:05–04:35, 05:05–05:35 — to prevent display burn-in. Also exposed as a toggleable `switch.antiburn` entity in Home Assistant.
- **Deep sleep**: After 30 s of LVGL idle the backlight turns off, LVGL pauses, and after a further 1800 s the device enters deep sleep. Wakes on any encoder rotation.
- **Touch wakeup**: Touching the screen while paused restores the display and backlight.

## Configuration

### Required

1. **`secrets.yaml`** — Place in the same directory as `Agent-Knob.yaml`:

   ```yaml
   wifi_ssid: "your-ssid"
   wifi_password: "your-password"
   ```

### Optional

- **Timezone** — Default is `Europe/Warsaw`. Change under `time:` if needed.
- **Anti-burn schedule** — Adjust the `on_time` triggers under `time:` to your preferred window (currently four 30-minute bursts overnight).
- **Deep-sleep idle timeout** — `30 s` idle + `1800 s` sleep, configurable under `lvgl.on_idle`.
- **UI labels** — The hardcoded display labels are editable under `lvgl.pages` if you prefer different text.

## Deployment

```bash
# Install ESPHome CLI if needed
pip install esphome

# Validate config
esphome config Agent-Knob.yaml

# Compile and upload via USB
esphome run Agent-Knob.yaml

# Or compile a binary for OTA / manual flash
esphome compile Agent-Knob.yaml
```

After first flash the device exposes a fallback hotspot (`scrollwheel Fallback Hotspot`, password `fallbackPassw0rd`) if Wi-Fi credentials are wrong.

## Project Structure

```
.
├── Agent-Knob.yaml   # Full ESPHome configuration
├── secrets.yaml       # Wi-Fi credentials (gitignored)
├── .gitignore         # Ignores secrets.yaml, build artifacts, and tool dirs
└── README.md          # This file
```

## Notes

- The display init sequence (`init_sequence:` under `display:`) is specific to the JC3636K518C panel — do not modify unless you know the correct command set.
- The encoder is implemented as two `binary_sensor` GPIOs (not the native `rotary_encoder` component) so both pins can participate in deep-sleep EXT1 wakeup via `allow_other_uses: true`.
- The backlight is a monochromatic `light` entity with `restore_mode: ALWAYS_ON` — it will turn on automatically after reboot or wake.
