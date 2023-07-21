#include "home.h"

#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QIcon>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QStyledItemDelegate>

#include "imagewin.h"
#include "ui_home.h"

Home::Home(QWidget* parent) : QMainWindow(parent), ui(new Ui::Home)
{
    ui->setupUi(this);
    this->statusBar()->setSizeGripEnabled(false);

    initPageImg();
    initPage2Img();
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(enlargeImage(QListWidgetItem*)));
    connect(ui->listWidget_4, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(enlargeImage(QListWidgetItem*)));
    ui->lineEdit->setReadOnly(true);

    ui->lineEdit_4->setReadOnly(true);
    ui->imgPath->setReadOnly(true);
    ui->imgPath_4->setReadOnly(true);

    ui->progressBar_3->setValue(0);

    detection = new Detection();
    connect(this, &Home::dirPathSignal, detection, &Detection::scvPath);
    connect(detection, &Detection::fileSize, this, &Home::progressBarSize);
    connect(detection, &Detection::progressBarValue, this, &Home::progressBarUpdate);
}

void Home::initPageImg()
{
    auto* imgList = ui->listWidget;
    imgList->setIconSize(QSize(950, 300));      //设置单个Icon大小
    imgList->setViewMode(QListView::IconMode);  //设置显示模式
    imgList->setFlow(QListView::LeftToRight);   //从左到右
    imgList->setResizeMode(QListView::Adjust);  //大小自适应
    imgList->setMovement(QListView::Static);    //设置列表每一项不可移动
}

void Home::initPage2Img()
{
    auto* imgList = ui->listWidget_4;
    imgList->setIconSize(QSize(950, 300));      //设置单个Icon大小
    imgList->setViewMode(QListView::IconMode);  //设置显示模式
    imgList->setFlow(QListView::LeftToRight);   //从左到右
    imgList->setResizeMode(QListView::Adjust);  //大小自适应
    imgList->setMovement(QListView::Static);    //设置列表每一项不可移动
}

void Home::showImages(QListWidget* imglist, cv::Mat& bgr, QString path)
{
    // 将BGR图像转换为Qt支持的格式，例如RGB格式
    cv::Mat rgb;
    cv::cvtColor(bgr, rgb, cv::COLOR_BGR2RGB);

    // 创建Qt图像
    QImage img(rgb.data, rgb.cols, rgb.rows, static_cast<int>(rgb.step), QImage::Format_RGB888);

    // 缩放图像以适应列表项
    QSize thumbnailSize(300, 339);
    QImage thumbnail = img.scaled(thumbnailSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // 创建列表项
    QListWidgetItem* item = new QListWidgetItem(QIcon(QPixmap::fromImage(thumbnail)), "");
    item->setData(Qt::UserRole, path);
    item->setSizeHint(thumbnailSize);

    // 将列表项添加到QListWidget
    imglist->addItem(item);
}

void Home::progressBarSize(const int size)
{
    maxSize = size;
    ui->progressBar->setMaximum(maxSize - 1);
}

void Home::progressBarUpdate(const int i)
{
    ui->progressBar->setValue(i);
}

void Home::predict(const QString& fileName)
{
    cv::Mat bgr = cv::imread(fileName.toStdString(), 1);
    anime = new AnimeGan(bgr);
// process and save
#pragma omp parallel for num_threads(2)
    cv::Mat outbgr;
    //    anime.detecet(styletransfernet, bgr, outbgr);
    anime->detecet(bgr, outbgr);

    QString path;          //本地图片路径
    utils.readJson(path);  //获取config文件中的保存路径
    int pos = path.lastIndexOf("/");
    if (pos != -1)
    {
        QString result = path.mid(pos + 1);
        QDir dir(QDir::currentPath());
        if (!dir.exists(result))
        {
            dir.mkdir(result);
        }
    }
    QString saveName = utils.RandomFile();
    qDebug() << path + "/" + saveName + ".jpg";
    cv::imwrite(path.toStdString() + "/" + saveName.toStdString() + ".jpg", outbgr);
    ncnn::destroy_gpu_instance();
    ui->progressBar_3->setValue(0);
    ui->progressBar_3->setMaximum(100);
    for (int i = 0; i <= 100; i++)
    {
        ui->progressBar_3->setValue(i);
    }

    showImages(ui->listWidget, outbgr, path + "/" + saveName + ".jpg");
}

Home::~Home()
{
    delete ui;
    delete dir;
    detection->quit();
    detection->wait(0);
}

/*
 * 通过自定义的imageWin来显示item中的图片
 */
void Home::enlargeImage(QListWidgetItem* item)
{
    QRect rect = this->geometry();  //获取当前窗口坐标及大小 x、y、w、h

    //获取图片路径
    QString imagePath = item->data(Qt::UserRole).toString();
    QPixmap image(imagePath);
    //通过自定义的窗口显示图片
    imageWin* showImageWidget =
        new imageWin(item, QRect(rect.x(), rect.y() + rect.y() * 1 / 4, image.width(), image.height()));

    showImageWidget->show();
}

/*
 * 单张图片处理时选择图片的函数
 */
void Home::on_pushButtonSelect_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this, "选择单个文件", "./");
    ui->imgPath->setText(filepath);
}

/*
 * 多张图片处理时选择文件夹的函数
 */
void Home::on_pushButtonSelect_4_clicked()
{
    QString dirpath = QFileDialog::getExistingDirectory(this, "选择目录", "./", QFileDialog::ShowDirsOnly);
    ui->imgPath_4->setText(dirpath);
}

void Home::on_pushButtonStart_clicked()
{
    QString filepath = ui->imgPath->text();
    if (!filepath.isEmpty())
    {
        qDebug() << "filepath:" << filepath;
        predict(filepath);
    }
    else
    {
        QMessageBox* m_box = new QMessageBox(QMessageBox::Information, QString("错误"), QString("请选择图片"));
        m_box->setStyleSheet("QMessageBox {background-color: #F2F2F2; }");
        m_box->show();
    }
}

void Home::on_pushButtonStart_4_clicked()
{
    QString filesPath = ui->imgPath_4->text();

    if (!filesPath.isEmpty())
    {
        emit dirPathSignal(filesPath);
        detection->start();
        connect(detection, &Detection::imgdata, this, &Home::reciveImg);
        qDebug() << "id::" << QThread::currentThreadId();
    }
    else
    {
        QMessageBox* m_box = new QMessageBox(QMessageBox::Information, QString("错误"), QString("请选择图片"));
        m_box->setStyleSheet("QMessageBox {background-color: #F2F2F2; }");
        m_box->show();
    }
}

void Home::reciveImg(cv::Mat bgr, QString path)
{
    showImages(ui->listWidget_4, bgr, path);
}
