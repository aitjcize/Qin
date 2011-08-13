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

#include <QDomElement>
#include <QHash>
#include <QSqlDatabase>
#include <QStringList>

/* Number of selection keys */
#define SELKEY_COUNT 10
#define SELKEYS { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30 }

/**
 * @name  QinIMBase
 * @brief Base class for all input method for Qin. New input methods should
 * derive from QinIMBase. 
 */
class QinIMBase {
  protected:
    QString xmlPath;
    QString imName;
    bool useCustomKeyMap;
    bool preEditable;

  public:
    /** Public members **/
    QHash<QString, QString> fromStdKB_hash;
    QHash<QString, QString> fromShiftStdKB_hash;

    /** Public methods **/
    QinIMBase(QString xmlpath);
    virtual ~QinIMBase();

    QString name(void) const;
    void setUseCustomKeyMap(bool s);
    bool getUseCustomKeyMap(void);
    void setPreEditable(bool s);
    bool getPreEditable(void);

    void setupKeyMap(const QDomElement& keymap);
    virtual bool isPreEditing(void);
    virtual bool getDoPopUp(void);
    virtual QStringList getPopUpStrings(void);

    /** I/O related **/
    /* Caller must free it */
    virtual char* getPreEditString(void);
    virtual char* getCommitString(void);
    virtual int cursorCurrent(void);
    virtual void setCursor(int index);
    QString fromStdKB(QString str);
    QString fromShiftStdKB(QString str);
    virtual void reset(void);

    /** Key handling APIs **/
    virtual void handle_Alt(void);
    virtual void handle_Backspace(void);
    virtual void handle_Capslock(void);
    virtual void handle_Ctrl(void);
    virtual void handle_Default(int);
    virtual void handle_Del(void);
    virtual void handle_Down(void);
    virtual void handle_End(void);
    virtual void handle_Enter(void);
    virtual void handle_Esc(void);
    virtual void handle_Home(void);
    virtual void handle_Left(void);
    virtual void handle_PageDown(void);
    virtual void handle_PageUp(void);
    virtual void handle_Right(void);
    virtual void handle_Space(void);
    virtual void handle_Tab(void);
    virtual void handle_Up(void);
};

/**
 * @name  QinTableIMBase
 * @brief Base class for table input methods.
 */
class QinTableIMBase: public QinIMBase {
  protected:
    QString dbPath;
    QSqlDatabase database;
    QString commitString;
    QStringList results;
    QString queryTemplate;
    QHash<int, int> keyTransform;
    int* keyStrokes;
    int maxKeyStrokes;
    int keyIndex;

  public:
    /** Public methods **/
    QinTableIMBase(QString xmlpath);
    virtual ~QinTableIMBase();

    virtual bool isPreEditing(void);
    virtual bool getDoPopUp(void);
    virtual QStringList getPopUpStrings(void);
    virtual void doQuery(void);
    virtual QString getQueryTemplate(void);

    /** I/O related **/
    /* Caller must free it */
    virtual char* getPreEditString(void);
    virtual char* getCommitString(void);

    /** Key handling APIs **/
    virtual void handle_Default(int keyId);
    virtual void handle_Enter(void);
    virtual void handle_Space(void);
    virtual void handle_Backspace(void);
};

#endif /* __QIN_SRC_QIN_IM_BASE_H__ */
