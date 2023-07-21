#include "histrioy.h"

#include "imagewin.h"
#include "setting.h"
#include "ui_histrioy.h"

Histrioy::Histrioy(QWidget* parent) : QWidget(parent), ui(new Ui::Histrioy)
{
    ui->setupUi(this);
    ui->listWidget->setIconSize(QSize(950, 300));
    ui->listWidget->setIconSize(QSize(950, 300));      //设置单个Icon大小
    ui->listWidget->setViewMode(QListView::IconMode);  //设置显示模式
    ui->listWidget->setFlow(QListView::LeftToRight);   //从左到右
    ui->listWidget->setResizeMode(QListView::Adjust);  //大小自适应
    ui->listWidget->setMovement(QListView::Static);    //设置列表每一项不可移动

    //样式表设计
    ui->listWidget->setStyleSheet("QListWidget::Item{padding-left:0px;padding-top:20px; padding-bottom:4px;color:black}"
                                  "QListWidget::Item:hover{background:lightgray; color:white;}"
                                  "QListWidget::item:selected{background:lightgray; color:black; }"
                                  "QListWidget{border: 1px solid white;}"
                                  "QListWidget{border-radius: 10px;}"
                                  "QListWidget{padding: 0 8px;}"
                                  "QlistWidget{background-color: rgb(255, 255, 255);}");
    //添加需要监视的目录
    QString path;
    utils.readJson(path);
    m_fileWatcher.addPath(path);

    //    //连接文件变化处理信号
    //    connect(&m_fileWatcher, &CustomFileWatcher::fileCreated, this, &Histrioy::handleDirectoryChanged);
    //    connect(&m_fileWatcher, &CustomFileWatcher::fileNameChanged, this, &Histrioy::handleDirectoryChanged);
    //    connect(&m_fileWatcher, &CustomFileWatcher::fileRemoved, this, &Histrioy::handleDirectoryChanged);

    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(enlargeImage(QListWidgetItem*)));

    //    Setting setting;
    //    connect(&setting, &Setting::ChangeSavePath, this, &Histrioy::changeSavePath);

    this->getAllFileFolder(path, floders);
    for (int i = 0; i < floders.size(); ++i)
    {
        ui->comboBox->addItem(floders[i]);
    }
}

Histrioy::~Histrioy()
{
    delete ui;
}

void Histrioy::showImages(QListWidget* imglist, QString floder)
{
    dir = new QDir(floder);

    // namefiles和files为QStringList类型
    namefiles << "*.png"
              << "*.jpg";

    //查找本地图片路径下的png后缀和jpg后缀的图片名  xxx.png  xxx.jpg
    files = (*dir).entryList(namefiles, QDir::Files | QDir::Readable, QDir::Name);

    //动态添加item到QListWidget里
    for (int i = 0; i < files.size(); i++)
    {
        QListWidgetItem* newitem =
            new QListWidgetItem(QIcon(QPixmap(floder + "/" + files.at(i)).scaled(320, 339)), files.at(i));
        QFileInfo fi = files.at(i);
        newitem->setText(fi.baseName());              // Returns the base name of the file without the path
        newitem->setTextAlignment(Qt::AlignHCenter);  //设置文字对齐方式：水平居中
        newitem->setData(Qt::UserRole, floder + "/" + files.at(i));
        imglist->addItem(newitem);
    }
}

void Histrioy::getAllFileFolder(QString dirPath, QVector<QString>& folder)
{
    QDir dir(dirPath);

    dir.setFilter(QDir::Dirs);

    foreach (QFileInfo fullDir, dir.entryInfoList())

    {
        if (fullDir.fileName() == "." || fullDir.fileName() == "..")
            continue;
        folder.push_back(fullDir.absoluteFilePath());

        this->getAllFileFolder(fullDir.absoluteFilePath(), folder);
    }
}

void Histrioy::handleDirectoryChanged()
{
    ui->listWidget->clear();
    QString path = ui->comboBox->currentText();
    showImages(ui->listWidget, path);
}

void Histrioy::on_pushButton_clicked()
{
    ui->listWidget->clear();
    QString path = ui->comboBox->currentText();
    showImages(ui->listWidget, path);
}

void Histrioy::enlargeImage(QListWidgetItem* item)
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

void Histrioy::changeSavePath(QString path)
{
    qDebug() << "sss" << path;
    this->getAllFileFolder(path, floders);
    for (int i = 0; i < floders.size(); ++i)
    {
        ui->comboBox->addItem(floders[i]);
    }
}

void Histrioy::on_comboBox_currentTextChanged(const QString& arg1)
{
    ui->listWidget->clear();
    showImages(ui->listWidget, arg1);
}
