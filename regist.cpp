#pragma execution_character_set("utf-8")
#include "regist.h"

#include "ui_regist.h"

Regist::Regist(QWidget* parent) : QMainWindow(parent), ui(new Ui::Regist)
{
    ui->setupUi(this);
}

Regist::~Regist()
{
    delete ui;
}

QList<QString> Regist::getEidUsrAndPwd()
{
    QString name = ui->usrname->text();       //用户昵称
    QString firstpwd = ui->pwd->text();       //第一次密码
    QString scendpwd = ui->true_pwd->text();  //确认密码

    if (name == nullptr)
    {
        ui->usrtips->setText(tr("昵称不能为空"));

        ui->usrtips->setStyleSheet("color:rgb(255,78,25)");
        QTimer::singleShot(1500, ui->usrtips, [&] { ui->usrtips->setText(""); });
    }
    else if (firstpwd == nullptr)
    {
        ui->usrtips->setText(tr("密码不能为空"));
        ui->usrtips->setStyleSheet("color:rgb(255,78,25)");
        QTimer::singleShot(1500, ui->usrtips, [&] { ui->usrtips->setText(""); });
    }

    else if (firstpwd != scendpwd)
    {
        ui->pwdtips->setText(tr("两次密码不一致"));
        ui->pwdtips->setStyleSheet("color:rgb(255,78,25)");
        QTimer::singleShot(1500, ui->pwdtips, [&] { ui->pwdtips->setText(""); });
    }
    else
    {
        QList<QString> PD;
        PD.append(name);
        PD.append(firstpwd);
        return PD;
    }
    return {};
}

void Regist::mousePressEvent(QMouseEvent* event)
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

void Regist::mouseMoveEvent(QMouseEvent* event)
{
    if (m_bDrag)
    {
        //获取鼠标移动的距离
        QPoint distance = event->globalPos() - mouseStartPoint;
        //改变窗口的位置
        this->move(WindowTopLeftPoint + distance);
    }
}

void Regist::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bDrag = false;
    }
}

void Regist::on_pushButton_regist_clicked()
{
    int id = data->getTableSize();
    QList<QString> PD = getEidUsrAndPwd();
    if (!PD.empty())
    {
        QString sql = QString("insert into usr(id,name,pwd) values(%1,'%2','%3')").arg(id + 1).arg(PD[0]).arg(PD[1]);
        qDebug() << sql;
        bool flag = data->insert(sql);
        if (flag)
        {
            QMessageBox* m_box = new QMessageBox(QMessageBox::Information, tr("通知"), tr("注册成功"));
            m_box->setStyleSheet("QMessageBox {background-color: #F2F2F2; }");
            QTimer::singleShot(1000, m_box, SLOT(accept()));
            m_box->show();
            this->close();
            LoginWindow* login = new LoginWindow();
            login->show();
        }
        else
        {
            QMessageBox* m_box = new QMessageBox(QMessageBox::Information, tr("通知"), tr("注册失败"));
            m_box->setStyleSheet("QMessageBox {background-color: #F2F2F2; }");
            QTimer::singleShot(1000, m_box, SLOT(accept()));
            m_box->show();
        }
    }
}

void Regist::on_pushButton_2_clicked()
{
    LoginWindow* login = new LoginWindow();
    this->close();
    login->show();
}
