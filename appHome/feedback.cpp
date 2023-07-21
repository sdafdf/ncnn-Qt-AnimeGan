#pragma execution_character_set("utf-8")
#include "feedback.h"

#include <QDebug>
#include <QPropertyAnimation>
#include <QTextBlockFormat>

#include "ui_feedback.h"
Feedback::Feedback(QWidget* parent) : QMainWindow(parent), ui(new Ui::Feedback)
{
    ui->setupUi(this);

    QTextBlockFormat blockFormat;
    blockFormat.setLineHeight(1.5, QTextBlockFormat::LineDistanceHeight);
    auto textCursor = ui->requestEdit->textCursor();
    textCursor.setBlockFormat(blockFormat);
    ui->requestEdit->setTextCursor(textCursor);
    ui->eamilEdit->setReadOnly(true);
    ui->requestEdit->setPlaceholderText(
        tr("此处可提交建议、想法、新增功能或者bug反馈等等。\n提示:此处消息不会有回复，但是作者能够收到您的建议"));
    connect(ui->requestEdit, &QTextEdit::textChanged, this, &Feedback::limitTextEditTextCount);

    QPropertyAnimation* animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(1000);
    animation->setStartValue(this->centralWidget()->geometry());
    animation->setEndValue(
        QRect(-this->centralWidget()->width(), 0, this->centralWidget()->width(), this->centralWidget()->height()));
}

Feedback::~Feedback()
{
    delete ui;
    delete sml;
    delete s1;
}

/*
 * 限制文本框的字符长度
 */
void Feedback::limitTextEditTextCount()
{
    QString text = ui->requestEdit->toPlainText();
    int count = text.count();

    if (count > 150)
    {
        int position = ui->requestEdit->textCursor().position();
        QTextCursor text_cursor = ui->requestEdit->textCursor();
        text.remove(position - (count - 150), count - 150);

        ui->requestEdit->setText(text);
        text_cursor.setPosition(position - (count - 150));
        ui->requestEdit->setTextCursor(text_cursor);
    }
}
