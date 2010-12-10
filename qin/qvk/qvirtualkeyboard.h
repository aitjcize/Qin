/*
 * Modified by Wei-Ning Huang
 *
 * Copyright 2010 Wei-Ning Huang <aitjcize@gmail.com>
 *
 * Copyright 2009 EMBITEL (http://www.embitel.com)
 * 
 * This file is part of Virtual Keyboard Project.
 * 
 * Virtual Keyboard is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation
 * 
 * Virtual Keyboard is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Virtual Keyboard. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef QVIRTUALKEYBOARD_H
#define QVIRTUALKEYBOARD_H

#include <QWidget>
#include "ui_qvirtualkeyboard.h"
class QSignalMapper;

class QVirtualKeyboard : public QWidget, public Ui::QVirtualKeyboard
{
  Q_OBJECT

  private:
    QWidget *m_pParent;
    bool Capsed;
    bool Shifted;
    bool Ctrled;
    bool Alted;
    bool Chinesed;
    QSignalMapper *signalMapper;
    QList<QToolButton *> allButtons;
    QHash<QString, QString> chewing2ascii;

  private slots:
    void s_on_btn_clicked(int btn);
    void on_btnCaps_toggled(bool checked);
    void on_btnShiftLeft_toggled(bool checked);
    void on_btnShiftRight_toggled(bool checked);
    void on_btnCtrlLeft_toggled(bool checked);
    void on_btnAltLeft_toggled(bool checked);
    void on_btnCh_toggled(bool checked);
    void changeTextShift(bool Shifted);
    void changeTextCaps(bool Capsed);
    void changeTextChinese(bool Chinesed);
    bool isTextKey(int keyId);

  public:
    QVirtualKeyboard(QWidget *parent = NULL);
    ~QVirtualKeyboard();
};
#endif /* QVIRTUALKEYBOARD_H */
