/**
 * @file   QinIMBases.cpp
 * @brief  
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

#include "QinIMBases.h"

#include <QCoreApplication>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include <cstring>

/* QinIMBase methods implementation */

QinIMBase::QinIMBase(QString name, bool ukey, bool pre):
  IMName(name), useCustomKeyMap(ukey), preEditable(pre) {}

QinIMBase::~QinIMBase() {}

QString QinIMBase::name(void) const {
  return IMName;
}

void QinIMBase::setUseCustomKeyMap(bool s) {
  useCustomKeyMap = s;
}

bool QinIMBase::getUseCustomKeyMap(void) {
  return useCustomKeyMap;
}

void QinIMBase::setPreEditable(bool s) {
  preEditable = s;
}

bool QinIMBase::getPreEditable(void) {
  return preEditable;
}

void QinIMBase::setupAll(void) {
  setupKeyMap();
}

void QinIMBase::setupKeyMap(void) {}

bool QinIMBase::getDoPopUp(void) {
  return false;
}

QStringList QinIMBase::getPopUpStrings(void) {
  return QStringList();
}

bool QinIMBase::isPreEditing(void) {
  return false;
}

char* QinIMBase::getPreEditString(void) {
  return NULL;
}

char* QinIMBase::getCommitString(void) {
  return NULL;
}

QString QinIMBase::toStdKB(QString str) {
  return (toStdKB_hash.find(str) != toStdKB_hash.end())?
    toStdKB_hash[str]: str;
}

QString QinIMBase::fromStdKB(QString str) {
  return (fromStdKB_hash.find(str) != fromStdKB_hash.end())?
    fromStdKB_hash[str]: str;
}

void QinIMBase::reset(void) {}
void QinIMBase::handle_Alt(void) {}
void QinIMBase::handle_Backspace(void) {}
void QinIMBase::handle_Capslock(void) {}
void QinIMBase::handle_Ctrl(void) {}
void QinIMBase::handle_Default(int) {}
void QinIMBase::handle_Del(void) {}
void QinIMBase::handle_Down(void) {}
void QinIMBase::handle_End(void) {}
void QinIMBase::handle_Enter(void) {}
void QinIMBase::handle_Esc(void) {}
void QinIMBase::handle_Home(void) {}
void QinIMBase::handle_Left(void) {}
void QinIMBase::handle_PageDown(void) {}
void QinIMBase::handle_PageUp(void) {}
void QinIMBase::handle_Right(void) {}
void QinIMBase::handle_Space(void) {}
void QinIMBase::handle_Tab(void) {}
void QinIMBase::handle_Up(void) {}


/* QinTableIMBase methods implementation */

QinTableIMBase::QinTableIMBase(QString name, bool ukey, QString dbname,
    int maxKeys): QinIMBase(name, ukey, true), dbName(dbname),
  maxKeyStrokes(maxKeys), keyIndex(0) {
  keyStrokes = new int[maxKeyStrokes + 1];

  database = QSqlDatabase::addDatabase("QSQLITE");
  database.setDatabaseName(dbName);
  if (!database.open()) {
    qDebug() << "Fatal error: can't find database `" << dbName << "' ..."
      << "abort.";
    QCoreApplication::exit(1);
  }
  database.setConnectOptions("QSQLITE_OPEN_READONLY");
}

QinTableIMBase::~QinTableIMBase() {
  database.close();
  delete [] keyStrokes;
}

QString QinTableIMBase::getQueryTemplate(void) {
  return QString();
}

bool QinTableIMBase::isPreEditing(void) {
  return keyIndex != 0;
}

void QinTableIMBase::doQuery(void) {
  QString queryTemplate = getQueryTemplate();
  QString query = queryTemplate;
  
  for (int i = 0; i < maxKeyStrokes; ++i) {
    if (i < keyIndex)
      query = query.arg("=%1").arg(doKeyTransform(keyStrokes[i]));
    else
      query = query.arg(" IS NULL");
  }

  results.clear();
  QSqlQuery queryResults = database.exec(query);
  
  while (queryResults.next())
    results += queryResults.value(0).toString();
}

int QinTableIMBase::doKeyTransform(int key) {
  return key;
}

char* QinTableIMBase::getPreEditString(void) {
  char* preEditStr = NULL;
  const char* cstr = NULL;

  if (!results.isEmpty()) {
    cstr = results[0].toStdString().c_str();
    preEditStr = new char[strlen(cstr) + 1];
    memcpy(preEditStr, cstr, strlen(cstr));
    preEditStr[strlen(cstr)] = 0;
  }

#ifdef DEBUG
  if (preEditStr) {
    qDebug() << "results[0]: " << results[0];
    qDebug("preEditStr: %s", preEditStr);
  }
#endif

  return preEditStr;
}

char* QinTableIMBase::getCommitString(void) {
  char* commitStr = NULL;
  const char* cstr = NULL;

  if (!commitString.isEmpty()) {
    cstr = commitString.toStdString().c_str();
    commitStr = new char[strlen(cstr) + 1];
    memcpy(commitStr, cstr, strlen(cstr));
    commitStr[strlen(cstr)] = 0;
    commitString.clear();
  }
  return commitStr;
}

void QinTableIMBase::handle_Default(int keyId) {
  if (keyIndex == maxKeyStrokes)
    return;
  keyStrokes[keyIndex++] = keyId;
  doQuery();
}

void QinTableIMBase::handle_Space(void) {
  doQuery();
  if (results.isEmpty())
    commitString.clear();
  else {
    commitString = results[0];
    results.clear();
  }

  /* reset keyStrokes */
  keyIndex = 0;
}

void QinTableIMBase::handle_Enter(void) {
  handle_Space();
}

void QinTableIMBase::handle_Backspace(void) {
  if (keyIndex > 0)
    --keyIndex;
  doQuery();
}
