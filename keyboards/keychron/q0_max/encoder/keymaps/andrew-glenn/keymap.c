/* Copyright 2024 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "keychron_common.h"

enum layers {
    BASE,
    NUMPAD,
};
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_tenkey_27(
        KC_MUTE, KC_MEDIA_PLAY_PAUSE,   KC_MEDIA_NEXT_TRACK, KC_NO, KC_NO,
        LSG(KC_A), KC_NO,               KC_NO,            KC_NO, LSFT(KC_F12),
        LGUI(KC_Y), KC_NO,              KC_NO,            LSG(KC_H), LSFT(KC_F11),
        LSG(KC_SPACE), KC_NO,               KC_NO,            KC_NO,
        LGUI(KC_L), KC_NO,               KC_NO,            KC_NO, KC_KP_ENTER,
        TO(NUMPAD), KC_NO,          KC_NO          ),    
    [NUMPAD] = LAYOUT_tenkey_27(
        KC_MUTE,            KC_ESC, KC_DEL, KC_TAB, KC_BSPC,
        KC_NO,	    KC_NUM, KC_PSLS,KC_PAST,KC_PMNS,
        KC_NO,	            KC_P7,	 KC_P8,	 KC_P9,	 KC_PPLS,
        QK_BOOTLOADER,	            KC_P4,	 KC_P5,	 KC_P6,
        KC_NO,	            KC_P1,	 KC_P2,	 KC_P3,	 KC_PENT,
        TO(BASE),           KC_P0,          KC_PDOT         ),
};

// clang-format on
#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [NUMPAD] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
};
#endif // ENCODER_MAP_ENABLE


void only_active_keys_leds(uint8_t led_min, uint8_t led_max, uint8_t x, uint8_t y, uint8_t z){
  uint8_t layer = get_highest_layer(layer_state);

  for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
    for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
      uint8_t index = g_led_config.matrix_co[row][col];

      if (index >= led_min && index < led_max){
        if (index != NO_LED && keymap_key_to_keycode(layer, (keypos_t){col,row}) > KC_TRNS) {
          rgb_matrix_set_color(index, x,y,z);
        } /* else {
          rgb_matrix_set_color(index, 0,0,0);
        } */
      }
    }
  }
}

void all_keys_leds(uint8_t led_min, uint8_t led_max, uint8_t x, uint8_t y, uint8_t z){
    for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
        for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
            uint8_t index = g_led_config.matrix_co[row][col];
            rgb_matrix_set_color(index, x,y,z);
        }
    }
}


bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
  switch (get_highest_layer(layer_state)){
    case BASE:
      only_active_keys_leds(led_min, led_max, 0, 255, 255);
      break;
    case NUMPAD:
      all_keys_leds(led_min, led_max, 255, 0, 0);
      break;
    default:
      break;
    }
   return false;
}

bool tap_hold(keyrecord_t* record, uint16_t tapkey, uint16_t holdkey){
  static uint16_t key_timer;
  if (record->event.pressed){
    key_timer = timer_read();
  } else {
    if(timer_elapsed(key_timer) < TAPPING_TERM) {
      tap_code16(tapkey);
    } else {
      tap_code16(holdkey);
    }
  }
  return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  switch (keycode) {
    case KC_MUTE:
        return tap_hold(record, LSG(KC_SPACE), KC_MUTE); 
    case LSG(KC_A):
        return tap_hold(record, LSG(KC_A),    LSG(KC_V)); 
  }
  return true; 
}
