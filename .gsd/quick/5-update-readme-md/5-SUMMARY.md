# Quick Task: update @README.md

**Date:** 2026-05-18
**Branch:** main

## What Changed
- Renamed title from "ESP32-S3 Scroll Wheel Controller" to "Agent Knob" to match project name
- Fixed page order: Page 2 = Color Picker, Page 3 = Percentage Control (was swapped)
- Fixed fallback hotspot name: "Agent Knob Fallback Hotspot" (was "scrollwheel")

## Files Modified
- `README.md`

## Verification
- Cross-referenced all changes against `Agent-Knob.yaml` config (friendly_name, lvgl.pages order, wifi.ap.ssid)
- Committed as `ab0de66` — docs: update README to match current config
