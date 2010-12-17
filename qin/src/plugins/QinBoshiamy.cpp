/**
 * @file   QinBoshiamy.cpp
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

#include "QinBoshiamy.h"

QinBoshiamy::QinBoshiamy(void):
  QinTableIMBase(false, "/usr/share/ovff/database/boshiamy.db", 5) {
  
}

QString QinBoshiamy::getQueryTemplate(void) {
  return "SELECT phrase FROM phrases WHERE m0%1 AND m1%2 AND m2%3 AND "
    "m3%4 AND m4%5 ORDER BY -freq;";
}

int QinBoshiamy::doKeyTransform(int key) {
  int ret = 0;

  switch (toupper(key)) {
    case '.':  ret = 56;  break;
    case ',':  ret = 55;  break;
    case '\'': ret = 27;  break;
    case '[':  ret = 45;  break;
    case ']':  ret = 46;  break;
    default:   ret = toupper(key) - 'A' + 1;
  }
  return ret;
}
