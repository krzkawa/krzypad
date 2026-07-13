// Copyright 2024 Krzysztof
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/* key matrix size */
#define MATRIX_ROWS 3
#define MATRIX_COLS 3

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

/* RP2040 I2C Pin Configuration */
#define I2C1_SCL_PIN GP7
#define I2C1_SDA_PIN GP6
