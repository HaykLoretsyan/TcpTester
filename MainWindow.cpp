#include "MainWindow.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QIntValidator>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_attacking(false),
      m_looped(false),
      m_x(10),
      m_y(10),
      m_coefficient(0),
      m_center(0),
      m_sendPoint(new QPushButton("send", this))
{
    setupLayout();

    setMinimumSize(400, 500);

    connect(m_sendPoint, &QPushButton::pressed, this, [this]() { m_controller.sendPoint(m_x, m_y); });
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupLayout()
{
    QWidget* centralWidget = new QWidget(this);
    QGridLayout* layout = new QGridLayout(this);

    layout->addWidget(ipEdit(), 0, 0, 1, 3);
    layout->addWidget(connectButton(), 0, 3, 1, 1);

    layout->addWidget(changeCoefficientPanel(), 1, 0, 1, 1);
    layout->addWidget(changeCenterPanel(), 1, 1, 1, 1);
    layout->addWidget(attackButton(), 1, 3, 1, 1);

    layout->addWidget(changePointPanel(), 2, 0, 1, 4);

    layout->addWidget(requestArea(), 3, 0, 2, 4);
    layout->addWidget(responseArea(), 5, 0, 2, 4);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

QWidget *MainWindow::connectButton()
{
    auto button = new QPushButton("connect", this);
    connect(button, &QPushButton::pressed, this, &MainWindow::connectToServer);

    return button;
}

QWidget *MainWindow::ipEdit()
{
    QWidget* ipWidget = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(this);

    QLineEdit* ip_edit = new QLineEdit(this);
    QLineEdit* port_edit = new QLineEdit(this);
    ip_edit->setPlaceholderText("IP");
    port_edit->setPlaceholderText("Port");
    connect(ip_edit, &QLineEdit::textChanged, this, [this](const QString& new_ip) { m_ip = new_ip; });
    connect(port_edit, &QLineEdit::textChanged, this, [this](const QString& new_port) { m_port = new_port.toInt(); });

    layout->addWidget(ip_edit, 3);
    layout->addWidget(port_edit, 1);

    ipWidget->setLayout(layout);
    return ipWidget;
}

QWidget *MainWindow::requestArea()
{
    QGroupBox* groupBox = new QGroupBox("Request", this);
    QHBoxLayout* layout = new QHBoxLayout(this);

    QTextEdit* request_text = new QTextEdit(this);
    request_text->setReadOnly(true);
    layout->addWidget(request_text);

    connect(&m_controller, &Controller::dataSent, this, [request_text](QString data) { request_text->append("\n" + data); });

    groupBox->setLayout(layout);
    return groupBox;
}

QWidget *MainWindow::responseArea()
{
    QGroupBox* groupBox = new QGroupBox("Response", this);
    QHBoxLayout* layout = new QHBoxLayout(this);

    QTextEdit* response_text = new QTextEdit(this);
    response_text->setReadOnly(true);
    layout->addWidget(response_text);

    connect(&m_controller, &Controller::dataReceived, this, [response_text](QString data) { response_text->append(data + "\n"); });

    groupBox->setLayout(layout);
    return groupBox;
}

QWidget *MainWindow::changePointPanel()
{
    QWidget* widget = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(this);

    QCheckBox* checkBox = new QCheckBox("loop", this);
    checkBox->setCheckState(m_looped ? Qt::Checked : Qt::Unchecked);
    connect(checkBox, &QCheckBox::stateChanged, this, [this](int state) {
        m_looped = (state == Qt::Checked);
        m_sendPoint->setEnabled(!m_looped);
    });

    QLineEdit* x_edit = new QLineEdit(QString::number(m_x), this);
    QLineEdit* y_edit = new QLineEdit(QString::number(m_x), this);
    x_edit->setValidator(new QIntValidator(0, 1500, this));
    y_edit->setValidator(new QIntValidator(0, 1500, this));
    connect(x_edit, &QLineEdit::textChanged, this, [this](const QString& value) { m_x = value.toInt(); } );
    connect(y_edit, &QLineEdit::textChanged, this, [this](const QString& value) { m_y = value.toInt(); } );

    QLabel* x_label = new QLabel("X: ", this);
    QLabel* y_label = new QLabel("Y: ", this);

    layout->addWidget(checkBox);
    layout->addWidget(x_label);
    layout->addWidget(x_edit);
    layout->addWidget(y_label);
    layout->addWidget(y_edit);
    layout->addWidget(m_sendPoint);

    widget->setLayout(layout);
    return widget;
}

QWidget *MainWindow::changeCoefficientPanel()
{
    QWidget* widget = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(this);

    QLineEdit* coefEdit = new QLineEdit(QString::number(m_coefficient), this);
    coefEdit->setValidator(new QIntValidator(0, 100, this));
    connect(coefEdit, &QLineEdit::textChanged, this, [this](const QString& value) { m_coefficient = value.toInt(); });
    QPushButton* sendButton = new QPushButton("send", this);
    connect(sendButton, &QPushButton::pressed, this, [this]() { m_controller.changeCoef(m_coefficient); } );

    QLabel* label = new QLabel("Coefficient: ", this);

    layout->addWidget(label);
    layout->addWidget(coefEdit);
    layout->addWidget(sendButton);

    widget->setLayout(layout);
    return widget;
}

QWidget *MainWindow::changeCenterPanel()
{
    QWidget* widget = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(this);

    QLineEdit* centerEdit = new QLineEdit(QString::number(m_center), this);
    centerEdit->setValidator(new QIntValidator(0, 1500, this));
    connect(centerEdit, &QLineEdit::textChanged, this, [this](const QString& value) { m_center = value.toInt(); });
    QPushButton* sendButton = new QPushButton("send", this);
    connect(sendButton, &QPushButton::pressed, this, [this]() { m_controller.changeCenter(m_center); } );

    QLabel* label = new QLabel("Center: ", this);

    layout->addWidget(label);
    layout->addWidget(centerEdit);
    layout->addWidget(sendButton);

    widget->setLayout(layout);
    return widget;
}

QWidget *MainWindow::attackButton()
{
    auto button = new QPushButton("attack", this);
    connect(button, &QPushButton::pressed, this, &MainWindow::activateAttack);

    return button;
}

void MainWindow::connectToServer()
{
    QPushButton* button = dynamic_cast<QPushButton*>(sender());
    button->setEnabled(false);

    if (m_controller.isConnected())
        m_controller.disconnectFromServer();
    else
        m_controller.connectToServer(m_ip, m_port);

    connect(&m_controller, &Controller::connectionChanged, this, [button](bool connected) {

        button->setEnabled(true);
        button->setText(connected ? "disconnect" : "connect");
    });
}

void MainWindow::activateAttack()
{
    QPushButton* button = dynamic_cast<QPushButton*>(sender());

    m_attacking = !m_attacking;
    button->setText(m_attacking ? "stop" : "attack");

    m_controller.activateAttack(m_attacking);
};

