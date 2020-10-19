#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QPushButton>

#include "Controller.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:

    void setupLayout();

    QWidget* connectButton();

    QWidget* ipEdit();

    QWidget* portEdit();

    QWidget* requestArea();

    QWidget* responseArea();

    QWidget* changePointPanel();

    QWidget* changeCoefficientPanel();

    QWidget* changeCenterPanel();

    QWidget* attackButton();

private slots:

    void connectToServer();

    void activateAttack();

private:

    Controller m_controller;

    QString m_ip;

    int m_port;

    bool m_attacking, m_looped;

    int m_x, m_y;

    int m_coefficient;

    int m_center;

    QPushButton* m_sendPoint;
};
#endif // MAINWINDOW_H
