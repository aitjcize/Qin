/**
 * @file   QinChewing.cpp
 * @brief  
 * @author Wei-Ning Huang (AZ) <aitjcize@gmail.com>
 *
 * Copyright (C) 2010 -  Wei-Ning Huang (AZ) <aitjcize@gmail.com>
 * All Rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "QinChewing.h"
#include "QinIMBase.h"

void QinChewing::setupKeyMap(void) {
  QIN_KEYMAP_REG("ㄅ", "1");
  QIN_KEYMAP_REG("ㄉ", "2");
  QIN_KEYMAP_REG("ˇ", "3");
  QIN_KEYMAP_REG("ˋ", "4");
  QIN_KEYMAP_REG("ㄓ", "5");
  QIN_KEYMAP_REG("ˊ", "6");
  QIN_KEYMAP_REG("˙", "7");
  QIN_KEYMAP_REG("ㄚ", "8");
  QIN_KEYMAP_REG("ㄞ", "9");
  QIN_KEYMAP_REG("ㄢ", "0");

  QIN_KEYMAP_REG("ㄆ", "q");
  QIN_KEYMAP_REG("ㄊ", "w");
  QIN_KEYMAP_REG("ㄍ", "e");
  QIN_KEYMAP_REG("ㄐ", "r");
  QIN_KEYMAP_REG("ㄔ", "t");
  QIN_KEYMAP_REG("ㄗ", "y");
  QIN_KEYMAP_REG("ㄧ", "u");
  QIN_KEYMAP_REG("ㄛ", "i");
  QIN_KEYMAP_REG("ㄟ", "o");
  QIN_KEYMAP_REG("ㄣ", "p");

  QIN_KEYMAP_REG("ㄇ", "a");
  QIN_KEYMAP_REG("ㄋ", "s");
  QIN_KEYMAP_REG("ㄎ", "d");
  QIN_KEYMAP_REG("ㄑ", "f");
  QIN_KEYMAP_REG("ㄕ", "g");
  QIN_KEYMAP_REG("ㄘ", "h");
  QIN_KEYMAP_REG("ㄨ", "j");
  QIN_KEYMAP_REG("ㄜ", "k");
  QIN_KEYMAP_REG("ㄠ", "l");
  QIN_KEYMAP_REG("ㄤ", ";");
  QIN_KEYMAP_REG("【", "[");
  QIN_KEYMAP_REG("】", "]");

  QIN_KEYMAP_REG("ㄈ", "z");
  QIN_KEYMAP_REG("ㄌ", "x");
  QIN_KEYMAP_REG("ㄏ", "c");
  QIN_KEYMAP_REG("ㄒ", "v");
  QIN_KEYMAP_REG("ㄖ", "b");
  QIN_KEYMAP_REG("ㄙ", "n");
  QIN_KEYMAP_REG("ㄩ", "m");
  QIN_KEYMAP_REG("ㄝ", ",");
  QIN_KEYMAP_REG("ㄡ", ".");
  QIN_KEYMAP_REG("ㄥ", "/");
}
