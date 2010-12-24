/**
 * @file   QinChewing.h
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

#ifndef __QIN_SRC_PLUGIN_QIN_CHEWING_H__
#define __QIN_SRC_PLUGIN_QIN_CHEWING_H__

#include "QinIMBases.h"

#include <chewing.h>

#define datadir "/usr"
#define homedir "/home/aitjcize"

/* libchewing related definitions */
#define QIN_CHEWING_DATA_PATH datadir"/share/libchewing3/chewing"
#define QIN_CHEWING_HASH_PATH homedir"/.chewing"
#define QIN_CHEWING_CAND_PER_PAGE 10
#define QIN_CHEWING_MAX_CHINESE_SYMBOL_LEN 10

/* QinChewing class definition */
class QinChewing: public QinIMBase {
  private:
    ChewingContext* chewContext;
    QString preEditString;

  public:
    QinChewing(void);
    virtual ~QinChewing(void);

    virtual bool isPreEditing(void);
    virtual bool getDoPopUp(void);
    virtual QStringList getPopUpStrings(void);

    /** I/O related **/
    virtual char* getPreEditString(void);
    virtual char* getCommitString(void);
    virtual int cursorCurrent(void);
    virtual void setCursor(int index);
    virtual void reset(void);

    /** Key handling APIs **/
    virtual void handle_Default(int keyId);
    virtual void handle_Space(void);
    virtual void handle_Esc(void);
    virtual void handle_Enter(void);
    virtual void handle_Del(void);
    virtual void handle_Backspace(void);
    virtual void handle_Tab(void);
    virtual void handle_Left(void);
    virtual void handle_Right(void);
    virtual void handle_Up(void);
    virtual void handle_Home(void);
    virtual void handle_End(void);
    virtual void handle_PageUp(void);
    virtual void handle_PageDown(void);
    virtual void handle_Down(void);
    virtual void handle_Capslock(void);
};

#endif /* __QIN_SRC_PLUGIN_QIN_CHEWING_H__ */
