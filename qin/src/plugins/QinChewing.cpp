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

#include <cstring>
#include <cstdio>

#include <chewing.h>

QinChewing::QinChewing(void): QinIMBase(true, true) {
  chewContext = chewing_new();
  chewing_Init(QIN_CHEWING_DATA_PATH, QIN_CHEWING_HASH_PATH);
  //chewing_set_KBType(chewContext, chewing_KBStr2Num("KB_DEFAULT"));
  chewing_set_ChiEngMode(chewContext, CHINESE_MODE);
  chewing_set_ShapeMode(chewContext, HALFSHAPE_MODE);
  chewing_set_candPerPage(chewContext, QIN_CHEWING_CAND_PER_PAGE);
  chewing_set_maxChiSymbolLen(chewContext, QIN_CHEWING_MAX_CHINESE_SYMBOL_LEN);
  chewing_set_spaceAsSelection(chewContext, true);
  chewing_set_escCleanAllBuf(chewContext, true);
  chewing_set_autoShiftCur(chewContext, false);
  chewing_set_phraseChoiceRearward(chewContext, true);
  int len = 1;
  int keys[] = { Qt::Key_Down };
  chewing_set_selKey(chewContext, keys, len);
  setupAll();
}

QinChewing::~QinChewing(void) {
  chewing_Terminate();
  chewing_delete(chewContext);
}

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

char* QinChewing::getPreEditString(void) {
  int preedit_len = 5;
  char* commit_str = chewing_commit_String(chewContext);
  char* zuin_str = chewing_zuin_String(chewContext, &preedit_len);
  char* preedit_str = chewing_buffer_String(chewContext);
  qDebug("Commit String: %s", commit_str);
  qDebug("Zuin String: %s", zuin_str);
  qDebug("Preedit String: %s", preedit_str);
  qDebug("Cand String: %s", chewing_cand_String(chewContext));

  if (strlen(commit_str) == 0)
    return zuin_str;
  else {
  qDebug("Buffer: %s", chewing_buffer_String(chewContext));
  qDebug("Buffer len: %d", chewing_buffer_Len(chewContext));
  qDebug("Commit: %d", chewing_commit_Check(chewContext));
  #if 0
    handle_Space();
    int len = chewing_get_phoneSeqLen(chewContext);
    uint16* seq = chewing_get_phoneSeq(chewContext);
    for (int i = 0; i < len; ++i)
      printf("%d", seq[i]);
    printf("\n");

    qDebug("Cand String: %s", chewing_cand_String(chewContext));
    qDebug("Cand cpp: %d", chewing_cand_ChoicePerPage(chewContext));
    qDebug("Cursor Current: %d", chewing_cursor_Current(chewContext));
  #endif
    return commit_str;
  }
}

void QinChewing::handle_Default(int keyId) {
  if (keyId >= 'A' && keyId <= 'Z') keyId += 32;
  chewing_handle_Default(chewContext, keyId);
}

void QinChewing::handle_Space(void) {
  chewing_handle_Space(chewContext);
}

void QinChewing::handle_Esc(void) {
  chewing_handle_Esc(chewContext);
}

void QinChewing::handle_Enter(void) {
  chewing_handle_Enter(chewContext);
}

void QinChewing::handle_Del(void) {
  chewing_handle_Del(chewContext);
}

void QinChewing::handle_Backspace(void) {
  chewing_handle_Backspace(chewContext);
}

void QinChewing::handle_Tab(void) {
  chewing_handle_Tab(chewContext);
}

void QinChewing::handle_ShiftLeft(void) {
  chewing_handle_ShiftLeft(chewContext);
}

void QinChewing::handle_Left(void) {
  chewing_handle_Left(chewContext);
}

void QinChewing::handle_ShiftRight(void) {
  chewing_handle_ShiftRight(chewContext);
}

void QinChewing::handle_Right(void) {
  chewing_handle_Right(chewContext);
}

void QinChewing::handle_Up(void) {
  chewing_handle_Up(chewContext);
}

void QinChewing::handle_Home(void) {
  chewing_handle_Home(chewContext);
}

void QinChewing::handle_End(void) {
  chewing_handle_End(chewContext);
}

void QinChewing::handle_PageUp(void) {
  chewing_handle_PageUp(chewContext);
}

void QinChewing::handle_PageDown(void) {
  chewing_handle_PageDown(chewContext);
}

void QinChewing::handle_Down(void) {
  chewing_handle_Down(chewContext);
}

void QinChewing::handle_Capslock(void) {
  chewing_handle_Capslock(chewContext);
}