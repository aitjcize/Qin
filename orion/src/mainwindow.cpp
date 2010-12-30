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

#include <QtGui>
#include <QtWebKit>
#include <QString>
#include "mainwindow.h"
#include <iostream>
using namespace std;
MainWindow::MainWindow()
{
  progress = 0;
  QNetworkProxyFactory::setUseSystemConfiguration(true);

    view = new QWebView(this);
    view->load(QUrl("http://www.google.com/"));
    connect(view, SIGNAL(loadFinished(bool)), SLOT(adjustLocation()));
    connect(view, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
    connect(view, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
    connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));

  locationEdit = new QLineEdit(this);
  locationEdit->setSizePolicy(QSizePolicy::Expanding, locationEdit->sizePolicy().verticalPolicy());
  connect(locationEdit, SIGNAL(returnPressed()), SLOT(changeLocation()));

  QToolBar *toolBar = addToolBar(tr("Navigation"));
  toolBar->addAction(view->pageAction(QWebPage::Back));
  toolBar->addAction(view->pageAction(QWebPage::Forward));
  toolBar->addAction(view->pageAction(QWebPage::Reload));
  toolBar->addAction(view->pageAction(QWebPage::Stop));
  toolBar->addWidget(locationEdit);

  setCentralWidget(view);
    //gesture
    grabGesture(Qt::SwipeGesture);    
}
bool MainWindow::touchEvent(QTouchEvent *event)
{
//    if (event->type() == QEvent::Gesture)
   cout << "here";
 //     return gestureEvent(static_cast<QGestureEvent*>(event));}
    return QWidget::event(event);
}
bool MainWindow::gestureEvent(QGestureEvent *event)
{
  if (QGesture *swipe = event->gesture(Qt::SwipeGesture))
   swipeTriggered(static_cast<QSwipeGesture *>(swipe));
  return true;
}

void MainWindow::swipeTriggered(QSwipeGesture *gesture)
{
//  if (gesture->state() == Qt::GestureFinished)
//   if (gesture->horizontalDirection() == QSwipeGesture::Left
//             || gesture->verticalDirection() == QSwipeGesture::Up)
}

void MainWindow::adjustLocation()
{
  locationEdit->setText(view->url().toString());
}

void MainWindow::changeLocation()
{
  QString url = locationEdit->text();
  if (!url.startsWith("http://")) {
    if (!url.contains("."))
      url = "http://www.google.com.tw/search?q=" + url;
    else
      url = "http://" + url;
  }
  locationEdit->setText(url);
  view->load(QUrl(url));
  view->setFocus();
}

void MainWindow::adjustTitle()
{
  if (progress <= 0 || progress >= 100)
    setWindowTitle(view->title());
  else
    setWindowTitle(QString("%1 (%2%)").arg(view->title()).arg(progress));
}

void MainWindow::setProgress(int p)
{
  progress = p;
  adjustTitle();
}

void MainWindow::finishLoading(bool)
{
  progress = 100;
  adjustTitle();
}
