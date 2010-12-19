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

#include <cstdio>

#include <QSignalMapper>
#include <QDesktopWidget>
#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "QinEngine.h"
#include "QinIMBases.h"

QVirtualKeyboard::QVirtualKeyboard(QinEngine* im)
  :QWidget(0, Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
  setupUi(this);
  this->move((QApplication::desktop()->width() - 520)/2,
      QApplication::desktop()->height() - 170);

  imEngine = im;
  Capsed = false;
  Shifted = false;
  isStdKeyMap = true;
  IMIndex = 0;
  opacitySlide->setRange(20, 100);

  allButtons = findChildren<QToolButton*>();
  signalMapper = new QSignalMapper(this);

  /* set style sheet */
  QFile data(":/data/button.qss");
  QString style;
  if (data.open(QFile::ReadOnly)) {
    QTextStream styleIn(&data);
    style = styleIn.readAll();
    data.close();
  } else
    qDebug() << "Failed to set style sheet!";

  for (int i = 0; i < allButtons.count(); i++) {
    allButtons.at(i)->setStyleSheet(style);
    connect(allButtons.at(i), SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(allButtons.at(i), i);
  }
  connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(s_on_btn_clicked(int)));
}

QVirtualKeyboard::~QVirtualKeyboard() {
  delete signalMapper;
}

void QVirtualKeyboard::insertInputMethod(const QinIMBase* im) {
  regedIMs.push_back(im->name());
  btnIMToggle->setText(regedIMs[0]);
  imEngine->setCurrentIM(0);
}

void QVirtualKeyboard::on_opacitySlide_valueChanged(int value) {
  setWindowOpacity((120.0 - value) / 100.0);
}

void QVirtualKeyboard::s_on_btn_clicked(int btn) {
  QString strKeyId = allButtons.at(btn)->accessibleName();
  bool isOk;
  int keyId = strKeyId.toInt(&isOk, 16);
  int involvedKeys = 1;
  bool istextkey = isTextKey(keyId);

  if (strKeyId.isEmpty() || !isOk || keyId == 0x10000001)
    return;

  Qt::KeyboardModifiers Modifier = Qt::NoModifier;

  if (Shifted) {
    Modifier = Modifier | Qt::ShiftModifier;
    involvedKeys++;

    switch (keyId) {
      case 0x2c:
      case 0x2e:
      case 0x2f: keyId += 0x10; break;
      case 0x3b: keyId = 0x3a; break;
      case 0x27: keyId = 0x22; break;
      case 0x5b:
      case 0x5c:
      case 0x5d: keyId += 0x20; break;
      case 0x31: keyId = 0x21; break;
      case 0x32: keyId = 0x40; break;
      case 0x33: keyId = 0x23; break;
      case 0x34: keyId = 0x24; break;
      case 0x35: keyId = 0x25; break;
      case 0x36: keyId = 0x5e; break;
      case 0x37: keyId = 0x26; break;
      case 0x38: keyId = 0x2a; break;
      case 0x39: keyId = 0x28; break;
      case 0x30: keyId = 0x29; break;
      case 0x2d: keyId = 0x5f; break;
      default: keyId = tolower(keyId);
    }
  }

  QString ch = allButtons.at(btn)->text().trimmed();

  if (!istextkey)
    ch = QString();
  if (keyId == Qt::Key_Space)
    ch = QString(" ");

  QWSServer::sendKeyEvent(ch.unicode()[0].unicode(),keyId,Modifier,true,false);

  if (istextkey) {
    btnShiftLeft->setChecked(false);
    btnShiftRight->setChecked(false);
  }
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

void QVirtualKeyboard::on_btnIMToggle_clicked(void) {
  IMIndex = (IMIndex + 1) % regedIMs.size();
  imEngine->setCurrentIM(IMIndex);
  btnIMToggle->setText(regedIMs[IMIndex]);

  if (imEngine->inputMethods[IMIndex]->getUseCustomKeyMap()) {
    isStdKeyMap = false;
    changeKeyMap(imEngine->inputMethods[IMIndex]);
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
