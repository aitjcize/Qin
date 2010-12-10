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

#include "qvirtualkeyboard.h"
#include <QKeyEvent>
#include <QSignalMapper>
#include <QSound>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QClipboard>
#include <QTextCodec>
#include <QList>
#include <QDebug>
#include <cstdio>

QVirtualKeyboard::QVirtualKeyboard(QWidget *parent) : QWidget(0)
{
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

  setupUi(this);
  resize(0,0);
  this->setWindowFlags(Qt::Tool);
  m_pParent = parent;
  Capsed = false;
  Shifted = false;
  Ctrled = false;
  Alted = false;
  Chinesed = false;
  changeTextCaps(false);
  allButtons = findChildren<QToolButton*>();
  signalMapper = new QSignalMapper(this);

  for (int i = 0; i < allButtons.count(); i++) {
    connect(allButtons.at(i), SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(allButtons.at(i), i);
  }

  connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(s_on_btn_clicked(int)));

  chewing2ascii["ㄅ"] = "1";
  chewing2ascii["ㄉ"] = "2";
  chewing2ascii["ˇ"] = "3";
  chewing2ascii["ˋ"] = "4";
  chewing2ascii["ㄓ"] = "5";
  chewing2ascii["ˊ"] = "6";
  chewing2ascii["˙"] = "7";
  chewing2ascii["ㄚ"] = "8";
  chewing2ascii["ㄞ"] = "9";
  chewing2ascii["ㄢ"] = "0";

  chewing2ascii["ㄆ"] = "q";
  chewing2ascii["ㄊ"] = "w";
  chewing2ascii["ㄍ"] = "e";
  chewing2ascii["ㄐ"] = "r";
  chewing2ascii["ㄔ"] = "t";
  chewing2ascii["ㄗ"] = "y";
  chewing2ascii["ㄧ"] = "u";
  chewing2ascii["ㄛ"] = "i";
  chewing2ascii["ㄟ"] = "o";
  chewing2ascii["ㄣ"] = "p";

  chewing2ascii["ㄇ"] = "a";
  chewing2ascii["ㄋ"] = "s";
  chewing2ascii["ㄎ"] = "d";
  chewing2ascii["ㄑ"] = "f";
  chewing2ascii["ㄕ"] = "g";
  chewing2ascii["ㄘ"] = "h";
  chewing2ascii["ㄨ"] = "j";
  chewing2ascii["ㄜ"] = "k";
  chewing2ascii["ㄠ"] = "l";
  chewing2ascii["ㄠ"] = ";";

  chewing2ascii["ㄈ"] = "z";
  chewing2ascii["ㄌ"] = "x";
  chewing2ascii["ㄏ"] = "c";
  chewing2ascii["ㄒ"] = "v";
  chewing2ascii["ㄖ"] = "b";
  chewing2ascii["ㄙ"] = "n";
  chewing2ascii["ㄩ"] = "m";        
  chewing2ascii["ㄝ"] = ",";
  chewing2ascii["ㄡ"] = ".";
  chewing2ascii["ㄥ"] = "/";
}

QVirtualKeyboard::~QVirtualKeyboard() {
  delete signalMapper;
}

void QVirtualKeyboard::s_on_btn_clicked(int btn) {
  QString strKeyId = allButtons.at(btn)->accessibleName();
  bool isOk;
  int keyId = strKeyId.toInt(&isOk, 16);

  if (strKeyId.isEmpty() || !isOk) {
    QMessageBox::information(0, 0, "Key Not Found");
    return;
  }

  if (keyId == Qt::Key_Shift
      || keyId == Qt::Key_Control
      || keyId == Qt::Key_Alt
      || keyId == 0xff /* 0xff: ch button */
     )
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

  if (Chinesed && chewing2ascii.find(ch) != chewing2ascii.end())
    ch = chewing2ascii[ch];

  //if(isTextKey) {
  //  QKeyEvent keyEventIns(QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier);
  //  QApplication::sendEvent(m_pParent->focusWidget(), &keyEventIns);
  //}

  QKeyEvent keyEvent(QEvent::KeyPress, keyId, Modifier, ch, false,involvedKeys);
  QApplication::sendEvent(m_pParent->focusWidget(), &keyEvent);

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
  }
  else {
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

void QVirtualKeyboard::on_btnCh_toggled(bool checked) {
  Chinesed = checked;
  changeTextChinese(Chinesed);
}

void QVirtualKeyboard::changeTextShift(bool Shifted) {    
  if (Chinesed) return;

  changeTextCaps(!Shifted);
  if (Shifted) {
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
  }
  else {
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

    btnStartSquare->setText(QChar('['));
    btnCloseSquare->setText(QChar(']'));
    btnBckSlash->setText(QChar('\\'));

    btnSemiColon->setText(QChar(';'));
    btnSp->setText(QChar('\''));

    btnComma->setText(QChar(','));
    btnPeriod->setText(QChar('.'));
    btnSlash->setText(QChar('/'));
  }
}

void QVirtualKeyboard::changeTextCaps(bool Capsed)
{
  if (Chinesed) return;
  if (Capsed) {
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
  }
  else {
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

    btnA->setText(QChar('a'));
    btnS->setText(QChar('s'));
    btnD->setText(QChar('d'));
    btnF->setText(QChar('f'));
    btnG->setText(QChar('g'));
    btnH->setText(QChar('h'));
    btnJ->setText(QChar('j'));
    btnK->setText(QChar('k'));
    btnL->setText(QChar('l'));

    btnZ->setText(QChar('z'));
    btnX->setText(QChar('x'));
    btnC->setText(QChar('c'));
    btnV->setText(QChar('v'));
    btnB->setText(QChar('b'));
    btnN->setText(QChar('n'));
    btnM->setText(QChar('m'));
  }
}

void QVirtualKeyboard::changeTextChinese(bool Chinesed) {
  if (Chinesed) {
    btn1->setText(QString("ㄅ"));
    btn2->setText(QString("ㄉ"));
    btn3->setText(QString("ˇ"));
    btn4->setText(QString("ˋ"));
    btn5->setText(QString("ㄓ"));
    btn6->setText(QString("ˊ"));
    btn7->setText(QString("˙"));
    btn8->setText(QString("ㄚ"));
    btn9->setText(QString("ㄞ"));
    btn0->setText(QString("ㄢ"));

    btnQ->setText(QString("ㄆ"));
    btnW->setText(QString("ㄊ"));
    btnE->setText(QString("ㄍ"));
    btnR->setText(QString("ㄐ"));
    btnT->setText(QString("ㄔ"));
    btnY->setText(QString("ㄗ"));
    btnU->setText(QString("ㄧ"));
    btnI->setText(QString("ㄛ"));
    btnO->setText(QString("ㄟ"));
    btnP->setText(QString("ㄣ"));

    btnA->setText(QString("ㄇ"));
    btnS->setText(QString("ㄋ"));
    btnD->setText(QString("ㄎ"));
    btnF->setText(QString("ㄑ"));
    btnG->setText(QString("ㄕ"));
    btnH->setText(QString("ㄘ"));
    btnJ->setText(QString("ㄨ"));
    btnK->setText(QString("ㄜ"));
    btnL->setText(QString("ㄠ"));
    btnSemiColon->setText(QString("ㄠ"));

    btnZ->setText(QString("ㄈ"));
    btnX->setText(QString("ㄌ"));
    btnC->setText(QString("ㄏ"));
    btnV->setText(QString("ㄒ"));
    btnB->setText(QString("ㄖ"));
    btnN->setText(QString("ㄙ"));
    btnM->setText(QString("ㄩ"));        
    btnComma->setText(QString("ㄝ"));
    btnPeriod->setText(QString("ㄡ"));
    btnSlash->setText(QString("ㄥ"));
  } else {
    btn1->setText(QString("1"));
    btn2->setText(QString("2"));
    btn3->setText(QString("3"));
    btn4->setText(QString("4"));
    btn5->setText(QString("5"));
    btn6->setText(QString("6"));
    btn7->setText(QString("7"));
    btn8->setText(QString("8"));
    btn9->setText(QString("9"));
    btn0->setText(QString("0"));

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
}


bool QVirtualKeyboard::isTextKey(int keyId)
{
  return (keyId==Qt::Key_Shift
      || keyId == Qt::Key_Control
      || keyId == Qt::Key_Tab
      || keyId == Qt::Key_Return
      || keyId == Qt::Key_Enter
      || keyId == Qt::Key_CapsLock
      || keyId == Qt::Key_Backspace
      || keyId == Qt::Key_Alt);
}
