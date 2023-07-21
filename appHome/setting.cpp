#include "setting.h"

#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QPalette>

#include "animeGan/animegan.h"
#include "histrioy.h"
#include "ui_setting.h"
#include "utils/utils.h"

Setting::Setting(QWidget* parent) : QMainWindow(parent), ui(new Ui::Setting)
{
    ui->setupUi(this);
    //设置默认保存位置
    Utils utils;
    QString savePath;
    bool ret = utils.readJson(savePath);
    if (ret)
    {
        ui->lineEdit->setText(savePath);
    }
    ui->lineEdit->setReadOnly(true);
    //    connect(ui->lineEdit, &QLineEdit::textChanged, this, &Setting::lineEditResize);
    ui->lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->lineEdit->setAlignment(Qt::AlignCenter);

    ui->toolButton->setEnabled(false);
    ui->toolButton_2->setEnabled(false);
    ui->toolButton_3->setEnabled(false);

    getModelName();

    AnimeGan* an = new AnimeGan();
    connect(this, &Setting::ModelName, an, &AnimeGan::getModelName);
}

Setting::~Setting()
{
    delete ui;
}

void Setting::getModelName()
{
    QDir* dir = new QDir(QDir::currentPath() + "/models");

    filesList << "*.param";
    files = (*dir).entryList(filesList, QDir::Files | QDir::Readable, QDir::Name);
    qDebug() << "sss";
    for (int i = 0; i < files.size(); ++i)
    {
        QFileInfo info(files.at(i));
        ui->comboBox->addItem(info.baseName());
    }
}

void Setting::on_pushButton_black_clicked()
{
    setAutoFillBackground(true);
    QPalette palette = qApp->palette();
    palette.setColor(QPalette::Window, Qt::black);
    setPalette(palette);
}

void Setting::on_pushButtonSave_clicked()
{
    QString dirpath = QFileDialog::getExistingDirectory(this, "选择目录", "./", QFileDialog::ShowDirsOnly);
    if (dirpath.isEmpty())
    {
        QMessageBox* m_box = new QMessageBox(QMessageBox::Information, QString("错误"), QString("请选择有效的路径"));
        m_box->setStyleSheet("QMessageBox {background-color: #F2F2F2; }");
        m_box->show();
    }
    else
    {
        qDebug() << dirpath;
        Utils utils;
        bool ret = utils.saveJson(dirpath);
        if (ret)
        {
            ui->lineEdit->setText(dirpath);
            emit ChangeSavePath(dirpath);
        }
    }
}

void Setting::lineEditResize()
{
    int minSize = 0;
    int height = ui->lineEdit->height();
    int changeSize = ui->lineEdit->text().size();
    qDebug() << "changesize:" << changeSize;
    qDebug() << "size:" << ui->lineEdit->width();
    if (changeSize > minSize)
    {
        ui->lineEdit->setGeometry(ui->lineEdit->x(), ui->lineEdit->y(), changeSize, height);
    }
}

void Setting::on_comboBox_currentTextChanged(const QString& arg1)
{
    emit ModelName(arg1);
}
