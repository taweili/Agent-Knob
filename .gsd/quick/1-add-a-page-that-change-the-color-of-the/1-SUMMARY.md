# Quick Task: add a page that change the color of the screen while rotating the encoder

**Date:** 2026-05-17
**Branch:** main

## What Changed
- Added `color_page_index` global (int) to track the current color index (0-9)
- Added Page 5 with a full-screen colored background obj (`color_bg`) that updates on encoder rotation
- Color name displayed in large Roboto 88 font, "Scroll to change" hint below in Roboto 20
- 10-color palette: Red, Green, Blue, Yellow, Cyan, Magenta, White, Orange, Purple, Pink
- Encoder Pin A (decrease) cycles to previous color with wrap-around
- Encoder Pin B (increase) cycles to next color with wrap-around
- Background color updated via `lv_obj_set_style_bg_color()` LVGL C API in lambda
- Page 4 swipe-left now navigates to page 5 (was page 1)
- Page 5 swipe-left navigates to page 1, swipe-right to page 4
- All encoder rotation also wakes device from LVGL paused state (existing behavior preserved)

## Files Modified
- `Agent-Knob.yaml` (+114 lines, -3 lines)

## Verification
- YAML structure validated via grep-based structural inspection (no ESPHome CLI available locally)
- All 5 pages have correct circular swipe navigation: 1→2→3→4→5→1 (left), reverse (right)
- `color_bg` obj referenced in 3 places: page definition + both encoder handlers
- `color_page_index` global defined once, referenced in both encoder handlers + label update
- Indentation uses spaces consistently (no tabs detected)
