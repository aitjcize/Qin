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
#include "QinIMBases.h"

#include <cctype>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>

#include <QStringList>
#include <chewing.h>

QinChewing::QinChewing(void): QinIMBase(":/data/Chewing.xml") {
  int keys[] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30 };
  chewContext = chewing_new();
  chewing_Init(QIN_CHEWING_DATA_PATH, QIN_CHEWING_HASH_PATH);
  chewing_set_ChiEngMode(chewContext, CHINESE_MODE);
  chewing_set_ShapeMode(chewContext, HALFSHAPE_MODE);
  chewing_set_candPerPage(chewContext, QIN_CHEWING_CAND_PER_PAGE);
  chewing_set_maxChiSymbolLen(chewContext, QIN_CHEWING_MAX_CHINESE_SYMBOL_LEN);
  chewing_set_spaceAsSelection(chewContext, true);
  chewing_set_escCleanAllBuf(chewContext, true);
  chewing_set_autoShiftCur(chewContext, true);
  chewing_set_phraseChoiceRearward(chewContext, true);
  chewing_set_selKey(chewContext, keys, sizeof(keys) / sizeof(keys[0]));
}

QinChewing::~QinChewing(void) {
  chewing_Terminate();
  chewing_delete(chewContext);
}

bool QinChewing::isPreEditing(void) {
  char* preedit = getPreEditString();
  bool ret = strlen(preedit);
  delete preedit;
  return ret;
}

bool QinChewing::getDoPopUp(void) {
  return chewing_cand_hasNext(chewContext);
}

QStringList QinChewing::getPopUpStrings(void) {
  QStringList cands;
  char* word = NULL;
  int choices = chewing_cand_ChoicePerPage(chewContext);

  if (chewing_cand_TotalChoice(chewContext)) {
    chewing_cand_Enumerate(chewContext);
    while (choices-- && chewing_cand_hasNext(chewContext)) {
      word = chewing_cand_String(chewContext);
      cands += word;
      free(word);
    }
  }
  return cands;
}

char* QinChewing::getPreEditString(void) {
  int preedit_len;
  char* buf_str = chewing_buffer_String(chewContext);
  char* zuin_str = chewing_zuin_String(chewContext, &preedit_len);
  int max_len = strlen(zuin_str) + strlen(buf_str);
  char* preedit_str = new char[max_len + 1];
  memset(preedit_str, 0, max_len + 1);

#ifdef DEBUG
  qDebug("-------------------------------------------------");
  qDebug("DEBUG: Buf: %s", buf_str);
  qDebug("DEBUG: Zuin: %s", zuin_str);
  qDebug("DEBUG: Commit: %d", chewing_commit_Check(chewContext));
  qDebug("DEBUG: Cand CheckDone: %d", chewing_cand_CheckDone(chewContext));
  qDebug("DEBUG: Cand hasNext: %d", chewing_cand_hasNext(chewContext));
#endif

  strncpy(preedit_str, buf_str, max_len);
  strncat(preedit_str, zuin_str, max_len - strlen(preedit_str));

  preedit_str[max_len] = 0;

  free(buf_str);
  free(zuin_str);

  return preedit_str;
}

char* QinChewing::getCommitString(void) {
  int committed = chewing_commit_Check(chewContext);
  char* commit_str = NULL;
  if (committed)
    commit_str = chewing_commit_String(chewContext);
  return commit_str;
}

int QinChewing::cursorCurrent(void) {
  return chewing_cursor_Current(chewContext);
}

void QinChewing::setCursor(int index) {
  int diff = cursorCurrent() - index;
  for (int i = 0; i < abs(diff); ++i) {
    if (diff > 0)
      handle_Left();
    else
      handle_Right();
  }
}

void QinChewing::reset(void) {
  chewing_handle_Esc(chewContext);
}

void QinChewing::handle_Default(int keyId) {
  chewing_handle_Default(chewContext, tolower(keyId));
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

void QinChewing::handle_Left(void) {
  chewing_handle_Left(chewContext);
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
