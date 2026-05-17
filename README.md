# ESP32-S3 Scroll Wheel Controller

An ESPHome-powered rotary encoder device with a 1.8" 360×360 round display (JC3636K518C), integrating with Home Assistant to control lights, covers, and switches via a tactile scroll-wheel interface.

## Hardware

| Component | Details |
|---|---|
| **MCU** | ESP32-S3 (esp32-s3-devkitc-1, 16 MB flash, octal PSRAM) |
| **Display** | 1.8" JC3636K518C, 360×360 QSPI round display |
| **Touch** | CST816 capacitive touchscreen (I²C) |
| **Input** | Rotary encoder on GPIO7 / GPIO8 (EXT1 wakeup, implemented as two `binary_sensor` GPIOs with `allow_other_uses`) |
| **Battery** | ADC on GPIO1 with voltage divider (2× multiplier), median filter, linear calibration |
| **Backlight** | Monochromatic PWM on GPIO47, 70% default brightness, 30 ms fade |
| **Fonts** | Roboto 20 / 32 / 88, fetched from Google Fonts |

### Pin Mapping

| Pin | Function |
|---|---|
| GPIO1 | Battery ADC input |
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

- **Page 1 — LED Brightness**: Scroll to adjust a Home Assistant light entity brightness (±5%). Shows current brightness, living-room temperature, and battery level. Display labels are in Polish ("LED", "Temperatura").
- **Page 2 — Cover Position**: Scroll to set a cover/blind position (±10%). Shows current position, temperature, and battery. Display labels are in Polish ("Zasłona", "Temperatura").
- **Page 3 — Switch Toggle**: Tap the on-screen switch to toggle a Home Assistant switch entity.
- **Swipe navigation**: Swipe left/right to move between pages.
- **Anti-burn**: Scheduled screen-saver (snow pattern) runs in four 30-minute bursts — 02:05–02:35, 03:05–03:35, 04:05–04:35, 05:05–05:35 — to prevent display burn-in. Also exposed as a toggleable `switch.antiburn` entity in Home Assistant.
- **Deep sleep**: After 30 s of LVGL idle the backlight turns off, LVGL pauses, and after a further 1800 s the device enters deep sleep. Wakes on any encoder rotation.
- **Touch wakeup**: Touching the screen while paused restores the display and backlight.
- **Delayed HA updates**: Encoder changes are batched — the new value is sent to Home Assistant 500 ms after the last scroll to avoid flooding.
- **HA number entity**: A `number.led_brightness` slider (0–100%, step 1) is exposed to Home Assistant for direct brightness control without using the encoder.

## Configuration

### Required

1. **`secrets.yaml`** — Place in the same directory as `Agent-Knob.yaml`:

   ```yaml
   wifi_ssid: "your-ssid"
   wifi_password: "your-password"
   ```

2. **Entity IDs** — Search the YAML for `entity_id:` and replace every hardcoded Home Assistant entity with your own:

   | Placeholder entity | Purpose |
   |---|---|
   | `light.0xa4c1388e095eafba` | LED light (brightness) |
   | `cover.zaslona_salon` | Cover / blind (position) |
   | `switch.0x70b3d52b600fde27` | Switch (page 3 toggle) |
   | `sensor.termometr_salon_temperature` | Temperature sensor |

### Optional

- **Timezone** — Default is `Europe/Warsaw`. Change under `time:` if needed.
- **Anti-burn schedule** — Adjust the `on_time` triggers under `time:` to your preferred window (currently four 30-minute bursts overnight).
- **Deep-sleep idle timeout** — `30 s` idle + `1800 s` sleep, configurable under `lvgl.on_idle`.
- **UI labels** — The hardcoded display labels ("LED", "Zasłona", "Temperatura") are in Polish; edit under `lvgl.pages` if you prefer a different language.

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

- Display gamma / dimming curve calibration points live under `battery_percentage → calibrate_linear`. Adjust if your cell chemistry differs.
- The display init sequence (`init_sequence:` under `display:`) is specific to the JC3636K518C panel — do not modify unless you know the correct command set.
- The encoder is implemented as two `binary_sensor` GPIOs (not the native `rotary_encoder` component) so both pins can participate in deep-sleep EXT1 wakeup via `allow_other_uses: true`.
- The backlight is a monochromatic `light` entity with `restore_mode: ALWAYS_ON` — it will turn on automatically after reboot or wake.
