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
 */

#ifndef __USEKEYBOARDDIALOG_H_
#define __USEKEYBOARDDIALOG_H_

#include <QDialog>
#include "ui_UseKeyBoardDialog.h"
class QVirtualKeyboard;

class UseKeyBoardDialog : public QDialog , public Ui::UseKeyBoardDialog
{
    Q_OBJECT
    
    public:
        UseKeyBoardDialog(QWidget *parent = 0);
        ~UseKeyBoardDialog();

    private:
        QVirtualKeyboard *virtualKeyBoard;
        
    private slots:
        void on_btnShow_clicked();
        void on_btnHide_clicked();
};
#endif  /*__USEKEYBOARDDIALOG_H_*/
