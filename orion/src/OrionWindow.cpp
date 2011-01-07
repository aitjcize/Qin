/****************************************************************************
 **
 ** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:LGPL$
 ** Commercial Usage
 ** Licensees holding valid Qt Commercial licenses may use this file in
 ** accordance with the Qt Commercial License Agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and Nokia.
 **
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Nokia gives you certain additional
 ** rights.  These rights are described in the Nokia Qt LGPL Exception
 ** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
 **
 ** GNU General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU
 ** General Public License version 3.0 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU General Public License version 3.0 requirements will be
 ** met: http://www.gnu.org/copyleft/gpl.html.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Nokia at qt-info@nokia.com.
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#include "OrionWindow.h"

#include <QtGui>

OrionWindow::OrionWindow(const QString defaultUrl) {
  progress = 0;

  location = new QLineEdit(this);
  location->setSizePolicy(QSizePolicy::Expanding, location->sizePolicy().verticalPolicy());
  connect(location, SIGNAL(returnPressed()), SLOT(changeLocation()));

  view = new QWebView(this);
  view->load(QUrl(defaultUrl));
  connect(view, SIGNAL(loadFinished(bool)), SLOT(onLoadFinished(bool)));
  connect(view, SIGNAL(loadFinished(bool)), SLOT(syncLocation()));
  connect(view, SIGNAL(titleChanged(QString)), SLOT(updateTitle()));
  connect(view, SIGNAL(loadProgress(int)), SLOT(updateProgress(int)));

  QToolBar *toolBar = addToolBar("Navigation");
  toolBar->addAction(view->pageAction(QWebPage::Back));
  toolBar->addAction(view->pageAction(QWebPage::Forward));
  toolBar->addAction(view->pageAction(QWebPage::Reload));
  toolBar->addAction(view->pageAction(QWebPage::Stop));
  toolBar->addWidget(location);

  tabbar = new QTabBar;
  addNewTab();

  setCentralWidget(tabbar);
}

void OrionWindow::addNewTab(void) {
  QPushButton* button = new QPushButton("button");
  tabbar->addTab("test");
}

void OrionWindow::changeLocation(void) {
  QString url = location->text();
  if (!url.startsWith("http://")) {
    if (!url.contains("."))
      url = "http://www.google.com.tw/search?q=" + url;
    else
      url = "http://" + url;
  }
  location->setText(url);
  view->load(QUrl(url));
  view->setFocus();
}

void OrionWindow::syncLocation(void) {
  location->setText(view->url().toString());
}

void OrionWindow::updateTitle(void) {
  if (progress <= 0 || progress >= 100)
    setWindowTitle(view->title());
  else
    setWindowTitle(QString("%1 (%2%)").arg(view->title()).arg(progress));
}

void OrionWindow::onLoadFinished(bool) {
  progress = 100;
  updateTitle();
}

void OrionWindow::updateProgress(int prog) {
  progress = prog;
  updateTitle();
}
