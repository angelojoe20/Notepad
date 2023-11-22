#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QFontDialog>
#include <QFont>
#include <QColorDialog>
#include <QColor>
#include <QPrinter>
#include <QPrintDialog>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
    setWindowTitle("My Notes");
    lineColLabel = new QLabel (this);
    statusBar()->addWidget(lineColLabel);

    connect(ui->textEdit, &QTextEdit::cursorPositionChanged, this, &MainWindow::updateLineColLabel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateLineColLabel()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    int line = cursor.blockNumber() + 1;
    int column = cursor.columnNumber() + 1;
    lineColLabel->setText("Line: " + QString::number(line) + " | Column: " + QString::number(column));
}


void MainWindow::on_actionNew_triggered()
{
    QString about_text;
    QMessageBox::about(this,"New File",about_text);
    file_path_ = "";
    ui->textEdit->setText("");
}


void MainWindow::on_actionOpen_triggered()
{
    QString file_name = QFileDialog::getOpenFileName(this,"Open the file");
       QFile file(file_name);
       file_path_ = file_name;
       if(!file.open(QFile::ReadOnly | QFile::Text)) {
           QMessageBox::warning(this, "..", "File not opened");
           return;
       }

       QTextDocument *doc = ui->textEdit->document();
       QTextCursor cursor(doc);
       QString html = QString::fromUtf8(file.readAll());

       doc->setHtml(html);

       file.close();

       ui->statusbar->showMessage("Opening file..", 5000);
   }



void MainWindow::on_actionSave_triggered()
{
    if (file_path_.isEmpty()) {
        on_actionSave_as_triggered();
        return;
    }

    QFile file(file_path_);
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "..", "File not opened");
        return;
    }

    QTextDocument *doc = ui->textEdit->document();
    QTextCursor cursor(doc);
    QString html = doc->toHtml();

    QTextStream out(&file);
    out << html;
    file.flush();
    file.close();

    ui->statusbar->showMessage("Saving file..", 5000);
}

void MainWindow::on_actionSave_as_triggered()
{
    QString file_name = QFileDialog::getSaveFileName(this,"Open the file");
        QFile file(file_name);
        file_path_ = file_name;
        if(!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, "..", "File not opened");
            return;
        }

        QTextDocument *doc = ui->textEdit->document();
        QTextCursor cursor(doc);
        QString html = doc->toHtml();

        QTextStream out(&file);
        out << html;
        file.flush();
        file.close();
    }


void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}


void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}


void MainWindow::on_actionAbout_triggered()
{
    QString about_text;
    about_text = "Author : Angelo Joe O. Delos Santos\n";
    about_text = "School : Pamantasang Lungsod ng Maynila\n";
    about_text += "Date  : 04/07/2023\n";
    about_text += "Course  : CPE 0222.1-3\n";
    about_text += "(C) This Notepad is for Activity\n";

    QMessageBox::about(this,"About Notepad",about_text);
    ui->statusbar->showMessage("AJ Pogi", 5000);
}


void MainWindow::on_actionExit_triggered()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Closing the FILE?", "Do you want to SAVE the file?", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);

        if (reply == QMessageBox::Yes){
            QMessageBox::information(this, "Information", "SAVED FILE");

            QString filter = "All File (.) ;; Text File (.txt) ;; Data File (.dat)";
            QString filename = QFileDialog::getOpenFileName(this,"Open Existing File","QDir", filter);
            QFile file (filename);

            if (!file.open(QFile::WriteOnly | QFile::Text)){
                QMessageBox::warning(this,"Warning", "File Not Open");
                return;
            }
            QTextStream out (&file);
            QString text = ui->textEdit->toPlainText();
            out << text;
            QMessageBox::information(this,"Message Box", "File SAVED");
            file.flush();
            file.close();
        } else if (reply == QMessageBox::No){
            QMessageBox::information(this,"Information", "You chose not to SAVE, clearing the editor.");
            ui->textEdit->clear();
        } else if (reply == QMessageBox::Cancel){
            QMessageBox::information(this, "Information", "You chose to CANCEL to action taken");
            return;
        }
        ui->statusbar->showMessage("Closing File", 5000);
}


void MainWindow::on_actionFont_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this);
    if (ok) {

        QTextCursor cursor =ui->textEdit->textCursor();
        QTextCharFormat charFormat = cursor.charFormat();
        charFormat.setFont(font);
        cursor.setCharFormat(charFormat);
        cursor.mergeBlockCharFormat(charFormat);
    }

        ui->statusbar->showMessage("Changing Font Type", 5000);
}


void MainWindow::on_actionColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose color");
    if (color.isValid()) {
        ui->textEdit->setTextColor(color);


    }
}


void MainWindow::on_actionBackground_Color_triggered()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose color");
    if (color.isValid()) {
        ui->textEdit->setTextBackgroundColor(color);
    }
}


void MainWindow::on_actionBackground_Color_Edit_Text_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "Choose Font color");
    if (color.isValid()) {

        //Set the text color
        QTextCharFormat format;
        format.setForeground(color);
        ui->textEdit->mergeCurrentCharFormat(format);
    }
        ui->statusbar->showMessage("Changing Font Color :D", 5000);
}


void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    printer.setPrinterName("Printer name");
    QPrintDialog dialog(&printer, this);
    if(dialog.exec() == QDialog::Rejected) return;
    ui->textEdit->print(&printer);
}


void MainWindow::on_actionClose_triggered()
{
    QMessageBox::information(this,"Warning!", "Program Will Terminate ");
    QCoreApplication::quit();

}




void MainWindow::on_actiontimedate_triggered()
{

}

