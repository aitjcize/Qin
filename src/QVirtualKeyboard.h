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

#include "ui_QVirtualKeyboard.h"
#include "QinEngine.h"
#include "QinIMBases.h"

#include <QPushButton>
#include <QStringList>
#include <QVector>
#include <QWidget>
#include <QWSInputMethod>

QT_BEGIN_NAMESPACE
class QinEngine;
class QSignalMapper;
QT_END_NAMESPACE

class QVirtualKeyboard : public QWidget, public Ui::QVirtualKeyboard {
  Q_OBJECT

  public:
    QVirtualKeyboard(QinEngine* im);
    ~QVirtualKeyboard();
    void insertInputMethod(const QinIMBase* im);
    void clearCandStrBar(void);
    void showCandStrBar(QStringList strlist);
    void hideAll(void);

  private:
    QinEngine* imEngine;
    QWidget* selectPanel;
    bool Capsed;
    bool Shifted;
    bool location;
    int IMIndex;
    int opacity;
    int* selkeys;
    QVector<QString> regedIMs;
    QSignalMapper *signalMapper;
    QSignalMapper *candSignalMapper;
    QList<QToolButton*> allButtons;
    QVector<QPushButton*> candButtons;

  private slots:
    void s_on_btn_clicked(int btn);
    void s_on_btnCands_clicked(int btn);
    void on_btnCaps_toggled(bool checked);
    void on_btnShiftLeft_toggled(bool checked);
    void on_btnShiftRight_toggled(bool checked);
    void on_btnIMToggle_clicked(void);
    void on_opacitySlide_valueChanged(int value);
    void on_btnLoc_clicked(void);
    bool isTextKey(int keyId);
    void changeNormalKeyMap(QinIMBase* imb);
    void changeShiftKeyMap(QinIMBase* imb);
};

#endif /* QVIRTUALKEYBOARD_H */
