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

#include "QVirtualKeyboard.h"
#include "QinEngine.h"
#include "QinIMBases.h"

#include <QKeyEvent>
#include <QSignalMapper>
#include <QDesktopWidget>
#include <QDebug>
#include <cstdio>

QVirtualKeyboard::QVirtualKeyboard(QinEngine* im)
  :QWidget(0, Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
  setupUi(this);
  this->move((QApplication::desktop()->width() - 600)/2,
      QApplication::desktop()->height() - 210);

  imEngine = im;
  Capsed = false;
  Shifted = false;
  Ctrled = false;
  Alted = false;
  isStdKeyMap = true;
  changeTextCaps(false);
  opacitySlide->setRange(20, 100);

  allButtons = findChildren<QToolButton*>();
  signalMapper = new QSignalMapper(this);

  for (int i = 0; i < allButtons.count(); i++) {
    connect(allButtons.at(i), SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(allButtons.at(i), i);
  }
  connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(s_on_btn_clicked(int)));
}

QVirtualKeyboard::~QVirtualKeyboard() {
  delete signalMapper;
}

int QVirtualKeyboard::insertInputMethod(const QString name) {
  IMSelect->addItem(name);
  return IMSelect->count() -1;
}

void QVirtualKeyboard::on_opacitySlide_valueChanged(int value) {
  setWindowOpacity((120.0 - value) / 100.0);
}

void QVirtualKeyboard::s_on_btn_clicked(int btn) {
  QString strKeyId = allButtons.at(btn)->accessibleName();
  bool isOk;
  int keyId = strKeyId.toInt(&isOk, 16);

  if (strKeyId.isEmpty() || !isOk)
    return;

  int involvedKeys = 1;
  Qt::KeyboardModifiers Modifier = Qt::NoModifier;

  if (Ctrled) {
    Modifier = Modifier | Qt::ControlModifier;
    involvedKeys++;
  }
  if (Shifted) {
    Modifier = Modifier | Qt::ShiftModifier;
    involvedKeys++;
  }
  if (Alted) {
    Modifier = Modifier | Qt::AltModifier;
    involvedKeys++;
  }    

  QString ch = allButtons.at(btn)->text().trimmed();
  if (keyId == Qt::Key_Space)
    ch = QString(" ");

  QWSServer::sendKeyEvent(ch.unicode()[0].unicode(), keyId, Modifier, true,
      false);

  btnShiftLeft->setChecked(false);
  btnShiftRight->setChecked(false);
  btnCtrlLeft->setChecked(false);
  btnAltLeft->setChecked(false);
}

void QVirtualKeyboard::on_btnCaps_toggled(bool checked) {
  changeTextCaps(checked);
  Capsed = checked;
}

void QVirtualKeyboard::on_btnShiftLeft_toggled(bool checked) {
  Shifted = checked;
  if (Capsed) {
    changeTextShift(checked);
  } else {
    changeTextShift(checked);
    changeTextCaps(checked);
  }
}

void QVirtualKeyboard::on_btnShiftRight_toggled(bool checked) {
  on_btnShiftLeft_toggled(checked);
}

void QVirtualKeyboard::on_btnCtrlLeft_toggled(bool checked) {
  Ctrled = checked;
}

void QVirtualKeyboard::on_btnAltLeft_toggled(bool checked) {
  Alted = checked;
}

void QVirtualKeyboard::on_IMSelect_currentIndexChanged(int index) {
  if (index < 0  || index >= imEngine->inputMethods.size()) return;

  imEngine->setCurrentIM(index);
  if (imEngine->inputMethods[index]->getUseCustomKeyMap()) {
    isStdKeyMap = false;
    changeKeyMap(imEngine->inputMethods[index]);
  } else
    restoreStdKeyMap();
}

void QVirtualKeyboard::changeTextShift(bool select) {    
  if (!isStdKeyMap) return;

  changeTextCaps(!select);
  if (select) {
    btnTilt->setText(QChar('~'));
    btn1->setText(QChar('!'));
    btn2->setText(QChar('@'));
    btn3->setText(QChar('#'));
    btn4->setText(QChar('$'));
    btn5->setText(QChar('%'));
    btn6->setText(QChar('^'));        
    btn7->setText("&&");
    btn8->setText(QChar('*'));
    btn9->setText(QChar('('));
    btn0->setText(QChar(')'));
    btnHiphen->setText(QChar('_'));
    btnAssign->setText(QChar('+'));

    btnStartSquare->setText(QChar('{'));
    btnCloseSquare->setText(QChar('}'));
    btnBckSlash->setText(QChar('|'));

    btnSemiColon->setText(QChar(':'));
    btnSp->setText(QChar('"'));

    btnComma->setText(QChar('<'));
    btnPeriod->setText(QChar('>'));
    btnSlash->setText(QChar('?'));
  } else
    restoreStdKeyMap();
}

void QVirtualKeyboard::changeTextCaps(bool select) {
  if (!isStdKeyMap) return;
  if (select) {
    btnQ->setText(QChar('Q'));
    btnW->setText(QChar('W'));
    btnE->setText(QChar('E'));
    btnR->setText(QChar('R'));
    btnT->setText(QChar('T'));
    btnY->setText(QChar('Y'));
    btnU->setText(QChar('U'));
    btnI->setText(QChar('I'));
    btnO->setText(QChar('O'));
    btnP->setText(QChar('P'));

    btnA->setText(QChar('A'));
    btnS->setText(QChar('S'));
    btnD->setText(QChar('D'));
    btnF->setText(QChar('F'));
    btnG->setText(QChar('G'));
    btnH->setText(QChar('H'));
    btnJ->setText(QChar('J'));
    btnK->setText(QChar('K'));
    btnL->setText(QChar('L'));

    btnZ->setText(QChar('Z'));
    btnX->setText(QChar('X'));
    btnC->setText(QChar('C'));
    btnV->setText(QChar('V'));
    btnB->setText(QChar('B'));
    btnN->setText(QChar('N'));
    btnM->setText(QChar('M'));        
  } else
    restoreStdKeyMap();
}

void QVirtualKeyboard::changeKeyMap(QinIMBase* imb) {
  btn1->setText(imb->fromStdKB("1"));
  btn2->setText(imb->fromStdKB("2"));
  btn3->setText(imb->fromStdKB("3"));
  btn4->setText(imb->fromStdKB("4"));
  btn5->setText(imb->fromStdKB("5"));
  btn6->setText(imb->fromStdKB("6"));
  btn7->setText(imb->fromStdKB("7"));
  btn8->setText(imb->fromStdKB("8"));
  btn9->setText(imb->fromStdKB("9"));
  btn0->setText(imb->fromStdKB("0"));
  btnHiphen->setText(imb->fromStdKB("-"));

  btnQ->setText(imb->fromStdKB("q"));
  btnW->setText(imb->fromStdKB("w"));
  btnE->setText(imb->fromStdKB("e"));
  btnR->setText(imb->fromStdKB("r"));
  btnT->setText(imb->fromStdKB("t"));
  btnY->setText(imb->fromStdKB("y"));
  btnU->setText(imb->fromStdKB("u"));
  btnI->setText(imb->fromStdKB("i"));
  btnO->setText(imb->fromStdKB("o"));
  btnP->setText(imb->fromStdKB("p"));
  btnStartSquare->setText(imb->fromStdKB("("));
  btnCloseSquare->setText(imb->fromStdKB(")"));

  btnA->setText(imb->fromStdKB("a"));
  btnS->setText(imb->fromStdKB("s"));
  btnD->setText(imb->fromStdKB("d"));
  btnF->setText(imb->fromStdKB("f"));
  btnG->setText(imb->fromStdKB("g"));
  btnH->setText(imb->fromStdKB("h"));
  btnJ->setText(imb->fromStdKB("j"));
  btnK->setText(imb->fromStdKB("k"));
  btnL->setText(imb->fromStdKB("l"));
  btnSemiColon->setText(imb->fromStdKB(";"));

  btnZ->setText(imb->fromStdKB("z"));
  btnX->setText(imb->fromStdKB("x"));
  btnC->setText(imb->fromStdKB("c"));
  btnV->setText(imb->fromStdKB("v"));
  btnB->setText(imb->fromStdKB("b"));
  btnN->setText(imb->fromStdKB("n"));
  btnM->setText(imb->fromStdKB("m"));        
  btnComma->setText(imb->fromStdKB(","));
  btnPeriod->setText(imb->fromStdKB("."));
  btnSlash->setText(imb->fromStdKB("/"));
}

void QVirtualKeyboard::restoreStdKeyMap(void) {
  isStdKeyMap = true;

  btnTilt->setText(QChar('`'));
  btn1->setText(QChar('1'));
  btn2->setText(QChar('2'));
  btn3->setText(QChar('3'));
  btn4->setText(QChar('4'));
  btn5->setText(QChar('5'));
  btn6->setText(QChar('6'));
  btn7->setText(QChar('7'));
  btn8->setText(QChar('8'));
  btn9->setText(QChar('9'));
  btn0->setText(QChar('0'));
  btnHiphen->setText(QChar('-'));
  btnAssign->setText(QChar('='));

  btnQ->setText(QChar('q'));
  btnW->setText(QChar('w'));
  btnE->setText(QChar('e'));
  btnR->setText(QChar('r'));
  btnT->setText(QChar('t'));
  btnY->setText(QChar('y'));
  btnU->setText(QChar('u'));
  btnI->setText(QChar('i'));
  btnO->setText(QChar('o'));
  btnP->setText(QChar('p'));
  btnStartSquare->setText(QChar('['));
  btnCloseSquare->setText(QChar(']'));
  btnBckSlash->setText(QChar('\\'));

  btnA->setText(QChar('a'));
  btnS->setText(QChar('s'));
  btnD->setText(QChar('d'));
  btnF->setText(QChar('f'));
  btnG->setText(QChar('g'));
  btnH->setText(QChar('h'));
  btnJ->setText(QChar('j'));
  btnK->setText(QChar('k'));
  btnL->setText(QChar('l'));
  btnSemiColon->setText(QString(";"));
  btnSp->setText(QChar('\''));

  btnZ->setText(QChar('z'));
  btnX->setText(QChar('x'));
  btnC->setText(QChar('c'));
  btnV->setText(QChar('v'));
  btnB->setText(QChar('b'));
  btnN->setText(QChar('n'));
  btnM->setText(QChar('m'));
  btnComma->setText(QString(","));
  btnPeriod->setText(QString("."));
  btnSlash->setText(QString("/"));
}

bool QVirtualKeyboard::isTextKey(int keyId)
{
  return !(keyId == Qt::Key_Shift
      || keyId == Qt::Key_Control
      || keyId == Qt::Key_Tab
      || keyId == Qt::Key_Return
      || keyId == Qt::Key_Enter
      || keyId == Qt::Key_CapsLock
      || keyId == Qt::Key_Backspace
      || keyId == Qt::Key_Alt);
}
