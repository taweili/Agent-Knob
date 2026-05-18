# Quick Task: fix this: Failed config

binary_sensor.gpio: [source Agent-Knob.yaml:450]
  platform: gpio
  id: encoder_pin_a
  pin:
    number: GPIO7
    mode: INPUT_PULLUP
    allow_other_uses: True
  on_press:
    - if:
        condition:
          lambda: return id(current_page) == 5;
        then:
          - lambda: |-
              static const uint32_t colors[] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0x00FFFF, 0xFF00FF, 0xFFFFFF, 0xFF8C00, 0x800080, 0xFFC0CB};
              static const char* names[] = {"Red", "Green", "Blue", "Yellow", "Cyan", "Magenta", "White", "Orange", "Purple", "Pink"};
              int count = 10;
              id(color_page_index) = (id(color_page_index) - 1 + count) % count;
              int idx = id(color_page_index);
              lv_obj_set_style_bg_color(color_bg, lv_color_hex(colors[idx]), LV_PART_MAIN);
          - lvgl.label.update:
              id: color_name_label
              text: !lambda |-
                static const char* names[] = {"Red", "Green", "Blue", "Yellow", "Cyan", "Magenta", "White", "Orange", "Purple", "Pink"};
                return names[id(color_page_index)];
    - if:
        condition:
          lambda: return id(current_page) == 6;
        then:
          - lambda: if (id(percentage_value) > 0) {
              id(percentage_value)--;
            }
          - lvgl.widget.update:
              id: percentage_arc

              [value] is an invalid option for [lvgl.widget.update]. Please check the indentation.
              value: !lambda |-
                return id(percentage_value);
          - lvgl.label.update:

**Date:** 2026-05-18
**Branch:** main

## What Changed
- Replaced `lvgl.widget.update` with `lvgl.arc.update` for updating the arc widget's `value` property in both `encoder_pin_a` and `encoder_pin_b` handlers (page 6, percentage control).
- The `value` property is arc-specific and not supported by the generic `lvgl.widget.update` action. ESPHome provides dedicated type-specific actions like `lvgl.arc.update` for widget-specific properties.

## Files Modified
- `Agent-Knob.yaml`

## Verification
- Confirmed zero remaining `lvgl.widget.update` calls in the config file.
- Both percentage arc update handlers (decrement on pin A, increment on pin B) now use the correct `lvgl.arc.update` action.
- Commit: `f8e29ac` - fix: use lvgl.arc.update instead of lvgl.widget.update for arc value
