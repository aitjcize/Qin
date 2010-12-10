/**
 * @file   QinEngine.cpp
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

#include "QinEngine.h"
#include "QVirtualKeyboard.h"

QinEngine::QinEngine() {
  vkeyboard = new QVirtualKeyboard(this);
}

QinEngine::~QinEngine() {
  delete vkeyboard;
}

void QinEngine::sendContent(const QString& newcontent) {
  inputBuffer += newcontent;
  sendPreeditString(inputBuffer, 0);
}

void QinEngine::sendKeyEventById(int keyId, Qt::KeyboardModifiers modifiers) {
  if (keyId == Qt::Key_Return && inputBuffer.length())
    confirmContent();
  else
    QWSServer::sendKeyEvent(0, keyId, modifiers, true, false);
}

void QinEngine::confirmContent() {
  sendCommitString(inputBuffer);
  inputBuffer.clear();
}

void QinEngine::updateHandler(int type) {
  switch (type) {
    case QWSInputMethod::FocusIn:
      vkeyboard->show();
      break;

    case QWSInputMethod::FocusOut:
      inputBuffer.clear();
      vkeyboard->hide();
      break;

    default:
      break;
  }
}
