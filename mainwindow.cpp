#include "mainwindow.h"

#include <QPropertyAnimation>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->statusBar()->setSizeGripEnabled(false);
    //禁止拖动窗口大小
    this->setFixedSize(this->width(), this->height());

    m_widget = this->findChildren<QMainWindow*>(QString(), Qt::FindDirectChildrenOnly);
    foreach (auto widhet, m_widget)
    {
        m_WidgetRect.insert(widhet, QRect(widhet->x(), widhet->y(), widhet->width(), widhet->height()));
    }
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    //将准备好的三个ui添加到stackedWidget中
    ui->stackedWidget->addWidget(&home);
    ui->stackedWidget->addWidget(&set);
    ui->stackedWidget->addWidget(&fd);
    ui->stackedWidget->addWidget(&ab);
    ui->stackedWidget->addWidget(&histrioy);

    //将tool按钮加入到按钮组中并编号
    btnGroup.addButton(ui->home, 0);
    btnGroup.addButton(ui->setting, 1);

    btnGroup.addButton(ui->feedback, 2);
    btnGroup.addButton(ui->about, 3);
    btnGroup.addButton(ui->histrioy, 4);
    connect(
        &btnGroup, QOverload<int>::of(&QButtonGroup::idClicked), ui->stackedWidget, &QStackedWidget::setCurrentIndex);

    //设置默认显示界面
    btnGroup.button(0)->setChecked(true);
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
    qDebug() << "ssss";
}

void MainWindow::receivelogin()
{
    qDebug() << "ssss";
    QPropertyAnimation* an = new QPropertyAnimation(this, "windowOpacity");
    an->setDuration(400);
    an->setStartValue(0);
    an->setEndValue(1);
    an->start();
    this->show();
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bDrag = true;
        //获取鼠标的初始位置
        mouseStartPoint = event->globalPos();
        //获取窗口的初始位置
        WindowTopLeftPoint = this->frameGeometry().topLeft();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (m_bDrag)
    {
        //获取鼠标移动的距离
        QPoint distance = event->globalPos() - mouseStartPoint;
        //改变窗口的位置
        this->move(WindowTopLeftPoint + distance);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bDrag = false;
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    float widht = this->width() * 1. / 600;
    float heiht = this->height() * 1. / 400;
    for (auto it = m_WidgetRect.begin(); it != m_WidgetRect.end(); it++)
    {
        it.key()->setGeometry(
            it.value().x() * widht, it.value().y() * heiht, it.value().width() * widht, it.value().height() * heiht);
    }
    QMainWindow::resizeEvent(event);
}

void MainWindow::on_pushButtonquit_clicked()
{
    this->close();
}

void MainWindow::on_pushButtonreduce_clicked()
{
    this->showMinimized();
}

void MainWindow::on_pushButtonbig_clicked()
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
