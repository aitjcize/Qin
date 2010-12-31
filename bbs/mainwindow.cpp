#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <pty.h>
#include <unistd.h>
#include <fcntl.h>
#include <QTextCodec>

void close_XD(int & fd){
  close(fd);
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);

        break;
    default:
        break;
    }
}
void MainWindow::on_pushButton_clicked(){
   
   int fd;
   pid_t pid = forkpty(&fd, NULL, NULL, NULL);
   
   std::string addr_str=ui->lineEdit->text().toStdString();
   const char *addr = addr_str.c_str(); 

   if (pid == 0) { // child process
     close_XD(fd);
     printf("Execl\n");
     execlp("telnet", "telnet", "-8", addr, NULL, NULL);
     printf("Shit\n");
     exit(-1);
   } else {
     // Parent process
     sleep(2);
     int flags = fcntl(fd, F_GETFD);
     fcntl(fd, F_SETFD, flags | FD_CLOEXEC);
   }
   printf("Start\n");
   char buf[BUFSIZ];
   int len = 0;
   while (1) {
     len = read(fd, buf, BUFSIZ);
     write(1, buf, len);
   }
}
