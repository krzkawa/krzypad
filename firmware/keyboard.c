// Copyright 2024 Krzysztof (@hackpad-user)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// Extern variables from keymap.c
extern uint8_t current_mode;
extern uint16_t knob_press_time;
extern bool knob_pressed;

// Volume tracking for display
static uint8_t current_volume = 100;
static uint16_t volume_display_timer = 0;
static bool show_volume = false;

// Time display variables
static uint32_t last_time_update = 0;
static uint8_t display_hours = 0;
static uint8_t display_minutes = 0;
static uint8_t display_seconds = 0;

// Initialize keyboard
void keyboard_post_init_user(void) {
    #ifdef OLED_ENABLE
    oled_init(OLED_ROTATION_0);
    #endif
}

// Matrix initialization
void matrix_init_user(void) {
    // Initialize any matrix-specific code here
}

// Handle encoder rotation for volume control
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (clockwise) {
            tap_code(KC_VOLU);
            if (current_volume < 255) {
                current_volume += 5;
                if (current_volume > 255) current_volume = 255;
            }
        } else {
            tap_code(KC_VOLD);
            if (current_volume > 0) {
                current_volume -= 5;
            }
        }
        
        // Show volume on display for 2 seconds
        show_volume = true;
        volume_display_timer = timer_read();
    }
    return false;
}

// OLED display update
#ifdef OLED_ENABLE

// Simple time simulation (increments based on ticks)
void update_display_time(void) {
    uint32_t current_time = timer_read32();
    
    if (current_time - last_time_update >= 1000) {
        last_time_update = current_time;
        display_seconds++;
        
        if (display_seconds >= 60) {
            display_seconds = 0;
            display_minutes++;
            
            if (display_minutes >= 60) {
                display_minutes = 0;
                display_hours++;
                
                if (display_hours >= 24) {
                    display_hours = 0;
                }
            }
        }
    }
}

// Draw volume bar (16 characters for 128px width)
void draw_volume_bar(void) {
    uint8_t bar_length = (current_volume / 16); // 255 / 16 ≈ 16 levels
    if (bar_length > 16) bar_length = 16;
    
    oled_write_P(PSTR("VOL: ["), false);
    for (uint8_t i = 0; i < 16; i++) {
        if (i < bar_length) {
            oled_write_char(0xFF, false); // Filled block
        } else {
            oled_write_char(0xB0, false); // Empty block
        }
    }
    oled_write_P(PSTR("]\n"), false);
}

// Main OLED task
bool oled_task_user(void) {
    // Update time continuously
    update_display_time();
    
    // Clear and draw header
    oled_clear();
    oled_write_P(PSTR("== HACKPAD ==\n"), false);
    
    // Show mode indicator
    if (current_mode == 0) { // FUSION
        oled_write_P(PSTR("MODE: FUSION360\n"), false);
    } else if (current_mode == 1) { // KICAD
        oled_write_P(PSTR("MODE: KICAD\n"), false);
    } else {
        oled_write_P(PSTR("MODE: UNKNOWN\n"), false);
    }
    
    // Show volume or time based on recent activity
    if (show_volume && timer_elapsed(volume_display_timer) < 2000) {
        draw_volume_bar();
        oled_write_P(PSTR("Drag wheel to adjust"), false);
    } else {
        show_volume = false;
        
        // Display time in HH:MM:SS format
        char time_str[9];
        snprintf(time_str, sizeof(time_str), "%02d:%02d:%02d", 
                 display_hours, display_minutes, display_seconds);
        oled_write_P(PSTR("TIME: "), false);
        oled_write(time_str, false);
        oled_write_P(PSTR("\n"), false);
        
        // Show instructions
        oled_write_P(PSTR("2x Click=Mode"), false);
    }
    
    return false;
}

#endif
