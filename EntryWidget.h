#ifndef ENTRYWIDGET_H
#define ENTRYWIDGET_H

#include <QLabel>
#include <QWidget>
#include <QPixmap>
#include <QTextEdit>
#include <QBoxLayout>
#include <QPushButton>

#include "ImageLabel.h"

class EntryWidget : public QWidget {
    Q_OBJECT
public:
    explicit EntryWidget(QWidget *parent = nullptr);

signals:

public:
protected:
    QHBoxLayout *main_layout_{nullptr};
};

class HtmlWidget : public EntryWidget {
    Q_OBJECT

public:
    explicit HtmlWidget(QWidget *parent = nullptr);

    bool Init(const QString &html);

private:
    QTextEdit *html_{nullptr};

    QLabel      *type_{nullptr};
    QPushButton *button_copy_text_{nullptr};
    QPushButton *button_copy_html_{nullptr};
};

class TextWidget : public EntryWidget {
    Q_OBJECT

public:
    explicit TextWidget(QWidget *parent = nullptr);

    bool Init(const QString &text);

private:
    QTextEdit *text_{nullptr};

    QLabel      *type_{nullptr};
    QPushButton *button_copy_text_{nullptr};
};

class ImageWidget : public EntryWidget {
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = nullptr);

    bool Init(const QPixmap &pixmap);

private:
    void OnButtonSaveClicked();

private:
    ImageLabel *image_{nullptr};

    QLabel *size_{nullptr};
    QLabel *type_{nullptr};

    QPushButton *button_copy_{nullptr};
    QPushButton *button_save_{nullptr};
};

class EntryWidget *CreateTextWidget(const QString &text, QWidget *parent);
class EntryWidget *CreateHtmlWidget(const QString &html, QWidget *parent);
class EntryWidget *CreateImageWidget(const QPixmap &pixmap, QWidget *parent);

#endif // ENTRYWIDGET_H
