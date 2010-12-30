#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <string>
#include <unistd.h>//for fork
#include <stdlib.h>//for fork
#include <errno.h>//for fork
#include <QTextCodec>
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
   FILE *fpipe;
   char line[256];
   QTextCodec* codec;
   QString display[24];
   /*multi process*/
   pid_t pid;
   
   codec = QTextCodec::codecForName("Big5");
   std::string addr_str= "telnet "+ ui->lineEdit->text().toStdString();
   const char *command = addr_str.c_str(); 
   pid = fork();
   if (pid == 0)
   {
    if ( !(fpipe = (FILE*)popen(command,"r")) )
    {  // If fpipe is NULL
       perror("Problems with pipe");
       return;
    }
    // int i = 0;
    while ( fgets( line, sizeof line, fpipe))
    {
     printf("%s", line);
      // display[i] = codec->toUnicode(const_cast<const char*>(line),
       // (int)sizeof line,0);
       //i++;
    }
    // ui->textEdit->setText(display[5]);
    pclose(fpipe);
    _exit(0);
   }
  }
