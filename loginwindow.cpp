#pragma execution_character_set("utf-8")
#include "loginwindow.h"

#include "regist.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    QTimer* timer = new QTimer;
    connect(timer, &QTimer::timeout, this, &LoginWindow::timerUpdate);
    timer->start(1000);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bDrag = true;
        //获取鼠标初始位置
        mouseStartPoint = event->globalPos();
        //获取窗口初始位置
        WindowTopLeftPoint = this->frameGeometry().topLeft();
    }
}

void LoginWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (m_bDrag)
    {
        //获取鼠标移动的距离
        QPoint distance = event->globalPos() - mouseStartPoint;
        //改变窗口的位置
        this->move(WindowTopLeftPoint + distance);
    }
}

void LoginWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bDrag = false;
    }
}

void LoginWindow::timerUpdate()
{
    QString str;
    QDateTime time = QDateTime::currentDateTime();
    str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
    QFont font("Microsoft YaHei", 13, 75);
    ui->label_time->setFont(font);
    ui->label_time->setText("  " + str);
}

void LoginWindow::on_pushButton_close_clicked()
{
    this->close();
}

void LoginWindow::on_pushButton_full_clicked()
{
    if (this->isFullScreen())
    {
        this->showNormal();
    }
    else
    {
        this->showFullScreen();
    }
}

void LoginWindow::on_pushButton_min_clicked()
{
    this->showMinimized();
}

void LoginWindow::on_pushButton_login_clicked()
{
    QString name = ui->username->text();
    QString pwd = ui->pwd->text();
    if (name == nullptr || pwd == nullptr)
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入账号和密码"));
    }
    else
    {
        QString sql = QString("select name,pwd from usr where name='%1'and pwd='%2'").arg(name, pwd);
        QList<QString> res;

        data->select(sql, res);
        if (res.empty())
        {
            QMessageBox::warning(nullptr, tr("错误"), tr("未查询到账号，请先注册"));
        }
        else
        {
            if (name == res[0] && pwd == res[1])
            {
                close();
                mainwindow = new MainWindow;
                mainwindow->show();
            }
            else
            {
                QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("用户名或密码错误"));
                ui->username->clear();
                ui->pwd->clear();
                ui->username->setFocus();
            }
        }
    }
}

void LoginWindow::on_pushButton_regist_clicked()
{
    this->close();
    Regist* regitsWindow = new Regist();
    regitsWindow->show();
}
