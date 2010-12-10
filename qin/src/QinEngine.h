/**
 * @file   QinEngine.h
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


#ifndef __QIN_SRC_QIN_ENGINE_H__
#define __QIN_SRC_QIN_ENGINE_H__

#include <QWSInputMethod>
#include "QVirtualKeyboard.h"

QT_BEGIN_NAMESPACE
class QVirtualKeyboard;
QT_END_NAMESPACE

class QinEngine: public QWSInputMethod {
  Q_OBJECT

  public:
    QinEngine();
    ~QinEngine();
    void updateHandler(int type);
    void setUseDefaultIM(bool select);
    void sendContent(QString ch, int uni = 0, int keyId = 0,
        Qt::KeyboardModifiers mod = Qt::NoModifier);

  private:
    bool usingDefaultIM;
    QVirtualKeyboard* vkeyboard;
    QString inputBuffer;

  private slots:
    void confirmContent();
};

#endif /* __QIN_SRC_QIN_ENGINE_H__ */
