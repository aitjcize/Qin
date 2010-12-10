/*
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
 * The supplied test sound file is properitery of Windows XP Balloon.wav © 2009 Microsoft Corporation. All rights reserved.
 */

#include "UseKeyBoardDialog.h"
#include "qvirtualkeyboard.h"

UseKeyBoardDialog::UseKeyBoardDialog(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
	virtualKeyBoard = new QVirtualKeyboard(this);
}

UseKeyBoardDialog::~UseKeyBoardDialog()
{}

void UseKeyBoardDialog::on_btnShow_clicked()
{
	virtualKeyBoard->show();
	this->lineEdit_2->setFocus();
}

void UseKeyBoardDialog::on_btnHide_clicked()
{    
	virtualKeyBoard->hide();
}
