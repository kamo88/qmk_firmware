/* Copyright 2020 illness072
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

#ifdef OLED_DRIVER_ENABLE
#include <stdio.h>
#define STEP 32
uint8_t kp = 0;
#endif


// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    QWERTY = SAFE_RANGE,
    LOWER,
    RAISE
};

#define KC_VU KC__VOLUP
#define KC_VD KC__VOLDOWN
#define KC_MU KC__MUTE
#define KC_LSPC LT(_LOWER, KC_SPC)
#define KC_RSPC LT(_RAISE, KC_SPC)
#define KC_LOW MO(_LOWER)
#define KC_RAI MO(_RAISE)


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT( \
  //,-------+-------+-------+-------+-------+-------.                                    ,-------+-------+-------+-------+-------+-------.
     KC_TAB , KC_Q  , KC_W  , KC_E  , KC_R  , KC_T  ,                                      KC_Y  , KC_U  , KC_I  , KC_O  , KC_P  ,KC_BSLS,
  //|-------+-------+-------+-------+-------+-------|                                    |-------+-------+-------+-------+-------+-------|
     KC_LCTL, KC_A  , KC_S  , KC_D  , KC_F  , KC_G  ,                                      KC_H  , KC_J  , KC_K  , KC_L  ,KC_SCLN,KC_QUOT,
  //`-------+-------+-------+-------+-------+-------|                                    |-------+-------+-------+-------+-------+-------'
              KC_Z  , KC_X  , KC_C  , KC_V  , KC_B  ,                                      KC_N  , KC_M  ,KC_COMM,KC_DOT ,KC_SLSH,
  //        `-------+-------+-------+-------+-------+-------+-------.    ,-------+-------+-------+-------+-------+-------+-------'
                                     KC_LALT,KC_LCMD,KC_LSPC,KC_LSFT,     KC_DEL ,KC_RSPC,KC_ENT ,KC_BSPC
  //                                `-------+-------+-------+-------'    `-------+-------+-------+-------'
  ),

  [_LOWER] = LAYOUT(
  //,-------+-------+-------+-------+-------+-------.                                    ,-------+-------+-------+-------+-------+-------.
     KC_ESC , KC_1  , KC_2  , KC_3  , KC_4  , KC_5  ,                                      KC_6  , KC_7  , KC_8  , KC_9  , KC_0  ,KC_TILD,
  //|-------+-------+-------+-------+-------+-------|                                    |-------+-------+-------+-------+-------+-------|
     KC_LCTL,KC_EXLM,KC_AT  ,KC_HASH,KC_DLR ,KC_PERC,                                     KC_CIRC,KC_AMPR,KC_ASTR,KC_MINS,KC_PLUS,KC_GRV,
  //`-------+-------+-------+-------+-------+-------|                                    |-------+-------+-------+-------+-------+-------|
             XXXXXXX,XXXXXXX,KC_LCBR,KC_LBRC,KC_LPRN,                                     KC_RPRN,KC_RBRC,KC_RCBR,KC_UNDS,KC_EQL ,
  //        `-------+-------+-------+-------+-------+-------+-------.    ,-------+-------+-------+-------+-------+-------+-------'
                                     KC_LALT,KC_LCMD,_______,KC_LSFT,     KC_DEL ,_______,KC_ENT ,KC_BSPC
  //                                `-------+-------+-------+-------'    `-------+-------+-------+-------'
  ),

  [_RAISE] = LAYOUT(
  //,-------+-------+-------+-------+-------+-------.                                    ,-------+-------+-------+-------+-------+-------.
     KC_ESC , KC_F1 , KC_F2 , KC_F3 , KC_F4 , KC_F5 ,                                      KC_F6 , KC_F7 , KC_F8 , KC_F9 ,KC_F10 ,KC_F11 ,
  //|-------+-------+-------+-------+-------+-------|                                    |-------+-------+-------+-------+-------+-------|
     KC_LCTL, KC_VD , KC_VU , KC_MU ,RGB_HUI,RGB_VAI,                                     XXXXXXX, KC_UP ,XXXXXXX,KC_HOME,KC_PGUP,KC_F12 ,
  //`-------+-------+-------+-------+-------+-------|                                    |-------+-------+-------+-------+-------+-------'
             KC_BRMD,KC_BRMU,RGB_TOG,RGB_MOD,RGB_VAD,                                     KC_LEFT,KC_DOWN,KC_RGHT,KC_END ,KC_PGDN,
  //        `-------+-------+-------+-------+-------+-------+-------.    ,-------+-------+-------+-------+-------+-------+-------'
                                     KC_LALT,KC_LCMD,_______,KC_LSFT,     KC_DEL ,_______,KC_ENT ,KC_BSPC
  //                                `-------+-------+-------+-------'    `-------+-------+-------+-------'
  )
};

static bool lower_pressed = false;
static uint16_t lower_pressed_time = 0;
static bool raise_pressed = false;
static uint16_t raise_pressed_time = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  case QWERTY:
    if (record->event.pressed) {
      set_single_persistent_default_layer(_QWERTY);
    }
    return false;
    break;
  case LOWER:
    if (record->event.pressed) {
      lower_pressed = true;
      lower_pressed_time = record->event.time;

      layer_on(_LOWER);
    } else {
      layer_off(_LOWER);
      if (lower_pressed && (TIMER_DIFF_16(record->event.time, lower_pressed_time) < TAPPING_TERM)) {
        tap_code(KC_SPC);
      }
      lower_pressed = false;
    }
    return false;
    break;
  case RAISE:
    if (record->event.pressed) {
      raise_pressed = true;
      raise_pressed_time = record->event.time;

      layer_on(_RAISE);
    } else {
      layer_off(_RAISE);
      if (raise_pressed && (TIMER_DIFF_16(record->event.time, raise_pressed_time) < TAPPING_TERM)) {
        tap_code(KC_SPC);
      }
      raise_pressed = false;
    }
    return false;
    break;
  default:
    if (record->event.pressed) {
      lower_pressed = false;
      raise_pressed = false;
    }
    break;
  }
#ifdef OLED_DRIVER_ENABLE
  if (record->event.pressed) {
    kp = (kp + 1) % STEP;
  }
#endif
  return true;
}

void matrix_init_user(void) {}

void matrix_scan_user(void) {}

void led_set_user(uint8_t usb_led) {}

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
      return OLED_ROTATION_180;
    }
    return rotation;
}
void oled_task_user(void) {
  if (is_keyboard_master()) {
    char disp[(21*4)+1] = {0};
    static char layer_names[_NUM_OF_LAYERS][10] = {"Default", "Lower", "Raise", "Adjust"};
    static char l1[] = "                \x94\x95\x96\x97";
    static char l2[] = "                \xB4\xB5\xB6\xB7";
    static char r1[] = "                \x98\x99\x9A\x9B";
    static char r2[] = "                \xB8\xB9\xBA\xBB";
    int space = kp % STEP;
    if (space > STEP / 2) space = STEP - space;
    if (kp < STEP / 2) {
      snprintf(disp, 84, "Layer: %s\n\n%s\n%s\n",
               layer_names[get_highest_layer(layer_state)], l1 + space, l2 + space);
    } else {
      snprintf(disp, 84, "Layer: %s\n\n%s\n%s\n",
               layer_names[get_highest_layer(layer_state)], r1 + space, r2 + space);
    }
    oled_write(disp, false);
  } else {
    static char *logo = "\n"
      "\x8f\x90\x91\x92\x93\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\n"
      "\xaf\xb0\xb1\xb2\xb3\xa0\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\n"
      "\xcf\xd0\xd1\xd2\xd3\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca\xcb\xcc\xcd\xce\n";
    oled_write(logo, false);
  }
}
#endif
