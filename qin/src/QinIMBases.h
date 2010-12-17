/**
 * @file   QinIMBase.h
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

#ifndef __QIN_SRC_QIN_IM_BASE_H__
#define __QIN_SRC_QIN_IM_BASE_H__

#include <cstring>

#include <QHash>
#include <QStringList>
#include <QSqlDatabase>

#define QIN_KEYMAP_REG(a, b) \
  toStdKB_hash[a] = b; \
  fromStdKB_hash[b] = a;

/**
 * @name  QinIMBase
 * @brief Base class for all input method for Qin. New input methods should
 * derive from QinIMBase. 
 */
class QinIMBase {
  private:
    bool useCustomKeyMap;
    bool preEditable;

  public:
    /** Public members **/
    QHash<QString, QString> toStdKB_hash;
    QHash<QString, QString> fromStdKB_hash;

    /** Public methods **/
    QinIMBase(bool ukey = false, bool pre = false);
    virtual ~QinIMBase();

    void setUseCustomKeyMap(bool s);
    bool getUseCustomKeyMap(void);
    void setPreEditable(bool s);
    bool getPreEditable(void);

    virtual void setupAll(void);
    virtual void setupKeyMap(void);
    virtual bool getDoPopUp(void);
    virtual QStringList getPopUpStrings(void);
    virtual bool isPreEditing(void);

    /** I/O related **/
    /* Caller must free it */
    virtual char* getPreEditString(void);
    virtual char* getCommitString(void);
    QString toStdKB(QString str);
    QString fromStdKB(QString str);
    virtual void reset(void);

    /** Key handling APIs **/
    virtual void handle_Default(int);
    virtual void handle_Space(void);
    virtual void handle_Esc(void);
    virtual void handle_Enter(void);
    virtual void handle_Del(void);
    virtual void handle_Backspace(void);
    virtual void handle_Tab(void);
    virtual void handle_ShiftLeft(void);
    virtual void handle_Left(void);
    virtual void handle_ShiftRight(void);
    virtual void handle_Right(void);
    virtual void handle_Up(void);
    virtual void handle_Home(void);
    virtual void handle_End(void);
    virtual void handle_PageUp(void);
    virtual void handle_PageDown(void);
    virtual void handle_Down(void);
    virtual void handle_Capslock(void);
};

/**
 * @name  QinTableIMBase
 * @brief Base class for table input methods.
 */
class QinTableIMBase: public QinIMBase {
  private:
    QStringList results;
    QString dbName;
    QSqlDatabase database;
    QString commitString;
    int* keyStrokes;
    int maxKeyStrokes;
    int keyIndex;

  public:
    /** Public methods **/
    QinTableIMBase(bool ukey = false, QString dbname = QString(),
        int maxKeys = 0);
    virtual ~QinTableIMBase();

    virtual bool isPreEditing(void);
    virtual void doQuery(void);
    virtual QString getQueryTemplate(void);
    virtual int doKeyTransform(int key);

    /** I/O related **/
    /* Caller must free it */
    virtual char* getPreEditString(void);
    virtual char* getCommitString(void);

    /** Key handling APIs **/
    virtual void handle_Default(int keyId);
    virtual void handle_Space(void);
    virtual void handle_Backspace(void);
};

#endif /* __QIN_SRC_QIN_IM_BASE_H__ */
