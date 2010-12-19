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

#include <chewing.h>

QinChewing::QinChewing(void): QinIMBase(":/data/Chewing.xml") {
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
  int len = 1;
  int keys[] = { Qt::Key_Down };
  chewing_set_selKey(chewContext, keys, len);
}

QinChewing::~QinChewing(void) {
  chewing_Terminate();
  chewing_delete(chewContext);
}

void QinChewing::setupKeyMap(void) {
}

bool QinChewing::isPreEditing(void) {
  char* preedit = getPreEditString();
  bool ret = strlen(preedit);
  delete preedit;
  return ret;
}

char* QinChewing::getPreEditString(void) {
  int preedit_len;
  char* buf_str = chewing_buffer_String(chewContext);
  char* zuin_str = chewing_zuin_String(chewContext, &preedit_len);
  int max_len = strlen(zuin_str) + strlen(buf_str);
  char* preedit_str = new char[max_len + 1];
  memset(preedit_str, 0, max_len + 1);

#ifdef DEBUG
  printf("Buf: %s\n", buf_str);
  printf("Zuin: %s\n", zuin_str);
  printf("Commit: %d\n", chewing_commit_Check(chewContext));
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
