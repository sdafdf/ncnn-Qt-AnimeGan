#ifndef SETTING_H
#define SETTING_H

#include <QMainWindow>

namespace Ui
{
    class Setting;
}

class Setting : public QMainWindow
{
    Q_OBJECT

  public:
    explicit Setting(QWidget* parent = nullptr);
    ~Setting();

    void getModelName();

  private slots:
    void on_pushButton_black_clicked();

    void on_pushButtonSave_clicked();
    void lineEditResize();
    void on_comboBox_currentTextChanged(const QString& arg1);

  signals:
    void ChangeSavePath(QString path);
    void ModelName(QString name);

  private:
    Ui::Setting* ui;
    QStringList filesList, files;
};

#endif  // SETTING_H
