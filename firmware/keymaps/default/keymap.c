// Copyright 2024 Krzysztof (@hackpad-user)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// Define custom keycodes for mode switching
enum custom_keycodes {
    FUSION_MODE = QK_USER,
    KICAD_MODE,
    TOGGLE_MODE
};

// Define layers
enum layer_names {
    FUSION,  // Fusion 360 mode
    KICAD,   // KiCAD mode
    SYS      // System/RGB controls
};

// Mode state variable
uint8_t current_mode = FUSION;
uint16_t knob_press_time = 0;
bool knob_pressed = false;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * Fusion 360 Mode (Layer 0)
     *             ┌───┐
     *             │MUT│
     * ┌───┬───┬───┼───┤
     * │ B │ E │ F │ P │ (Box, Extrude, Fillet, Pad)
     * ├───┼───┼───┼───┤
     * │ S │ R │ M │ X │ (Scale, Rotate, Mirror, Cut)
     * ├───┼───┼───┼───┤
     * │ V │ Z │ G │ D │ (Create View, Zoom fit, Grab, Delete)
     * ├───┼───┼───┼───┤
     * │ C │ U │LT1│ A │ (Copy, Undo, Layer Toggle, All select)
     * └───┴───┴───┴───┘
     */
    [FUSION] = LAYOUT(
        TOGGLE_MODE,
        KC_B, KC_E, KC_F, KC_P,
        KC_S, KC_R, KC_M, KC_X,
        KC_V, KC_Z, KC_G, KC_D,
        KC_C, KC_U, MO(SYS), KC_A
    ),

    /*
     * KiCAD Mode (Layer 1)
     *             ┌───┐
     *             │MUT│
     * ┌───┬───┬───┼───┤
     * │ E │ W │ G │ D │ (Edit, Wire, Grab, Delete)
     * ├───┼───┼───┼───┤
     * │ R │ L │ P │ U │ (Route, Labels, Place, Unroute)
     * ├───┼───┼───┼───┤
     * │ Z │ X │ Y │ V │ (Zoom fit, Mirror X, Mirror Y, Via)
     * ├───┼───┼───┼───┤
     * │ C │ F │LT1│ A │ (Copy, Fill, Layer Toggle, All select)
     * └───┴───┴───┴───┘
     */
    [KICAD] = LAYOUT(
        TOGGLE_MODE,
        KC_E, KC_W, KC_G, KC_D,
        KC_R, KC_L, KC_P, KC_U,
        KC_Z, KC_X, KC_Y, KC_V,
        KC_C, KC_F, MO(SYS), KC_A
    ),

    /*
     * System/RGB Control Mode (Layer 2)
     *             ┌───┐
     *             │RST│
     * ┌───┬───┬───┼───┤
     * │RGB│R+ │G+ │B+ │
     * ├───┼───┼───┼───┤
     * │TOG│R- │G- │B- │
     * ├───┼───┼───┼───┤
     * │ < │ > │SPD│SPI│
     * ├───┼───┼───┼───┤
     * │HU+│HU-│SAT│VAL│
     * └───┴───┴───┴───┘
     */
    [SYS] = LAYOUT(
        QK_BOOT,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    )
};

// Handle custom keycodes
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TOGGLE_MODE:
            if (record->event.pressed) {
                knob_pressed = true;
                knob_press_time = timer_read();
            } else {
                knob_pressed = false;
                // Check if it's a double-press (quick presses)
                if (timer_elapsed(knob_press_time) < 300) {
                    current_mode = (current_mode == FUSION) ? KICAD : FUSION;
                    layer_clear();
                    layer_on(current_mode);
                }
            }
            return true;
        case FUSION_MODE:
            if (record->event.pressed) {
                current_mode = FUSION;
                layer_clear();
                layer_on(FUSION);
            }
            return false;
        case KICAD_MODE:
            if (record->event.pressed) {
                current_mode = KICAD;
                layer_clear();
                layer_on(KICAD);
            }
            return false;
    }
    return true;
}
