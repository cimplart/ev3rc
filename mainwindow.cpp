// MIT License

// Copyright (c) 2023 Artur Wisz

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    try {
        publisher = new RcCommandPublisher();
    }  catch (const std::exception& e) {
        delete ui;
        throw;
    }
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete publisher;
    delete ui;
}


void MainWindow::on_forwardButton_pressed()
{
    qDebug("forward button pressed");
    publisher->send_command(Ev3MoveType::Forward, true);
}


void MainWindow::on_forwardButton_released()
{
    qDebug("forward button released");
    publisher->send_command(Ev3MoveType::Forward, false);
}


void MainWindow::on_turnLeftButton_pressed()
{
    qDebug("forward-left button pressed");
    publisher->send_command(Ev3MoveType::ForwardLeft, true);
}


void MainWindow::on_turnLeftButton_released()
{
    qDebug("forward-left button released");
    publisher->send_command(Ev3MoveType::ForwardLeft, false);
}


void MainWindow::on_turnRightButton_pressed()
{
    qDebug("forward-right button pressed");
    publisher->send_command(Ev3MoveType::ForwardRight, true);
}


void MainWindow::on_turnRightButton_released()
{
    qDebug("forward-right button released");
    publisher->send_command(Ev3MoveType::ForwardRight, false);
}


void MainWindow::on_backwardButton_pressed()
{
    qDebug("backward button pressed");
    publisher->send_command(Ev3MoveType::Backward, true);
}

void MainWindow::on_backwardButton_released()
{
    qDebug("backward button released");
    publisher->send_command(Ev3MoveType::Backward, false);
}

void MainWindow::on_spinLeftButton_pressed()
{
    qDebug("spin-left button pressed");
    publisher->send_command(Ev3MoveType::SpinLeft, true);
}

void MainWindow::on_spinLeftButton_released()
{
    qDebug("spin-left button released");
    publisher->send_command(Ev3MoveType::SpinLeft, false);
}

void MainWindow::on_spinRightButton_pressed()
{
    qDebug("spin-right button pressed");
    publisher->send_command(Ev3MoveType::SpinRight, true);
}

void MainWindow::on_spinRightButton_released()
{
    qDebug("spin-right button released");
    publisher->send_command(Ev3MoveType::SpinRight, false);
}

