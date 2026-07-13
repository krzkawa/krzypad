// Copyright 2024 Krzysztof (@hackpad-user)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/* key matrix size */
#define MATRIX_ROWS 5
#define MATRIX_COLS 4

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5

/* Encoder support */
#define ENCODER_RESOLUTION 4

/* OLED Display Configuration */
#define OLED_DISPLAY_128X32
#define OLED_FONT_H "glcdfont.c"

/* I2C Configuration for OLED */
#define I2C_DRIVER I2CD1
#define I2C_FREQUENCY 400000
