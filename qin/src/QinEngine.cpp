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

#include <cstdlib>

#include "QinEngine.h"
#include "QVirtualKeyboard.h"

#include "plugins/QinEnglish.h"
#include "plugins/QinChewing.h"

QinEngine::QinEngine() {
  vkeyboard = new QVirtualKeyboard(this);
  regInputMethod("English", new QinEnglish());
  regInputMethod("Chewing", new QinChewing());
}

QinEngine::~QinEngine() {
  delete vkeyboard;
  for (QVector<QinIMBase*>::iterator it = inputMethods.begin();
      it != inputMethods.end(); ++it)
    delete *it;
}

void QinEngine::regInputMethod(QString name, QinIMBase* imb) {
  if (!imb) {
    qDebug("error: no input method specified\n");
    return;
  }

  inputMethods.push_back(imb);
  vkeyboard->insertInputMethod(name);
}

void QinEngine::sendContent(QString ch, int uni, int keyId,
        Qt::KeyboardModifiers mod) {
  bool isSpecialKey = true;

  if (!currentIM->getPreEditable())
    QWSServer::sendKeyEvent(uni, keyId, mod, true, false);
  else {
    switch (keyId) {
      case Qt::Key_Space: currentIM->handle_Space(); break;
      case Qt::Key_Escape: currentIM->handle_Esc(); break;
      case Qt::Key_Enter:
      case Qt::Key_Return:
        currentIM->handle_Enter();
        if (inputBuffer.length()) {
          confirmContent();
          return;
        } else
          break;
      case Qt::Key_Delete: currentIM->handle_Del(); break;
      case Qt::Key_Backspace:
        currentIM->handle_Backspace();
        updatePreEditBuffer();
        return;
      case Qt::Key_Tab: currentIM->handle_Tab(); break;
      case Qt::Key_Shift: currentIM->handle_ShiftLeft(); break;
      case Qt::Key_Left: currentIM->handle_Left(); break;
      case Qt::Key_Right: currentIM->handle_Right(); break;
      case Qt::Key_Up: currentIM->handle_Up(); break;
      case Qt::Key_Home: currentIM->handle_Home(); break;
      case Qt::Key_End: currentIM->handle_End(); break;
      case Qt::Key_PageUp: currentIM->handle_PageUp(); break;
      case Qt::Key_PageDown: currentIM->handle_PageDown(); break;
      case Qt::Key_Down: currentIM->handle_Down(); break;
      case Qt::Key_CapsLock: currentIM->handle_Capslock(); break;
      default:
        isSpecialKey = false;
        currentIM->handle_Default(keyId);
        updatePreEditBuffer();
    }
    if (isSpecialKey)
      QWSServer::sendKeyEvent(0, keyId, mod, true, false);
  }
}

void QinEngine::confirmContent() {
  sendCommitString(inputBuffer);
  inputBuffer.clear();
}

void QinEngine::updatePreEditBuffer() {
  char* preedit = currentIM->getPreEditString();
  inputBuffer = QString(preedit);
  sendPreeditString(inputBuffer, 0);
  free(preedit);
}

void QinEngine::updateHandler(int type) {
  switch (type) {
    case QWSInputMethod::FocusIn:
      currentIM->reset();
      vkeyboard->show();
      break;

    case QWSInputMethod::FocusOut:
      inputBuffer.clear();
      vkeyboard->hide();
      break;

    default:
      inputBuffer.clear();
      break;
  }
}

void QinEngine::mouseHandler(int offset, int state) {
  if (state == QWSServer::MousePress) {
    sendPreeditString(inputBuffer, offset, 1);
  }
}
