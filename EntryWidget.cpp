#include "EntryWidget.h"

#include <QHBoxLayout>
#include <QSpacerItem>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QFileDialog>
#include <QDate>

#include "MainWidget.h"

EntryWidget::EntryWidget(QWidget *parent) : QWidget{parent} {
    this->main_layout_ = new QHBoxLayout(this);
    this->setContentsMargins(0, 0, 0, 0);
}

HtmlWidget::HtmlWidget(QWidget *parent) : EntryWidget(parent) {
    {
        this->html_ = new QTextEdit(this);
        html_->setAcceptRichText(true);
        html_->setReadOnly(true);
        html_->setContentsMargins(0, 0, 0, 0);
    }
    main_layout_->addWidget(html_);
    {
        auto wid = new QWidget(this);
        { wid->setFixedWidth(100); }
        auto layout = new QVBoxLayout(wid);
        wid->setLayout(layout);
        this->main_layout_->addWidget(wid);

        {
            this->type_ = new QLabel(this);
            this->type_->setText("Html");
        }
        layout->addWidget(type_);

        {
            auto spacer = new QSpacerItem(200, 10, QSizePolicy::Fixed, QSizePolicy::Expanding);
            layout->addItem(spacer);
        }

        {
            this->button_copy_html_ = new QPushButton(this);
            this->button_copy_html_->setText("copy html");
        }
        layout->addWidget(this->button_copy_html_);

        {
            this->button_copy_text_ = new QPushButton(this);
            this->button_copy_text_->setText("copy text");
        }
        layout->addWidget(this->button_copy_text_);
    }

    // connect
    QObject::connect(this->button_copy_html_, &QPushButton::clicked, [this](bool) {
        GetMainWidget()->SkipOnce();

        QMimeData  *data  = new QMimeData();
        QClipboard *board = QApplication::clipboard();

        data->setHtml(this->html_->toHtml());
        board->setMimeData(data);
    });

    QObject::connect(this->button_copy_text_, &QPushButton::clicked, [this](bool) {
        QClipboard *board = QApplication::clipboard();
        board->setText(this->html_->toPlainText());
    });
}

bool HtmlWidget::Init(const QString &html) {
    html_->setHtml(html);
    return true;
}

TextWidget::TextWidget(QWidget *parent) : EntryWidget(parent) {
    {
        this->text_ = new QTextEdit(this);
        text_->setReadOnly(true);
        text_->setContentsMargins(0, 0, 0, 0);
    }
    main_layout_->addWidget(text_);
    {
        auto wid = new QWidget(this);
        { wid->setFixedWidth(100); }
        auto layout = new QVBoxLayout(wid);
        wid->setLayout(layout);
        this->main_layout_->addWidget(wid);

        {
            this->type_ = new QLabel(this);
            this->type_->setText("Text");
        }
        layout->addWidget(type_);

        {
            auto spacer = new QSpacerItem(200, 10, QSizePolicy::Fixed, QSizePolicy::Expanding);
            layout->addItem(spacer);
        }

        {
            this->button_copy_text_ = new QPushButton(this);
            this->button_copy_text_->setText("copy text");
        }
        layout->addWidget(this->button_copy_text_);
    }

    // connect
    QObject::connect(this->button_copy_text_, &QPushButton::clicked, [this](bool) {
        GetMainWidget()->SkipOnce();
        QClipboard *board = QApplication::clipboard();
        board->setText(this->text_->toPlainText());
    });
}

bool TextWidget::Init(const QString &text) {
    text_->setPlainText(text);

    return true;
}

ImageWidget::ImageWidget(QWidget *parent) : EntryWidget(parent) {
    {
        this->image_ = new ImageLabel(this);
        // image_->setScaledContents(true);
        image_->setContentsMargins(0, 0, 0, 0);
    }
    main_layout_->addWidget(image_);
    {
        auto wid = new QWidget(this);
        { wid->setFixedWidth(100); }
        auto layout = new QVBoxLayout(wid);
        wid->setLayout(layout);
        this->main_layout_->addWidget(wid);

        {
            this->type_ = new QLabel(this);
            this->type_->setText("Image");
        }
        layout->addWidget(type_);

        { this->size_ = new QLabel(this); }
        layout->addWidget(this->size_);

        {
            auto spacer = new QSpacerItem(200, 10, QSizePolicy::Fixed, QSizePolicy::Expanding);
            layout->addItem(spacer);
        }

        {
            this->button_copy_ = new QPushButton(this);
            this->button_copy_->setText("copy");
        }
        layout->addWidget(this->button_copy_);

        {
            this->button_save_ = new QPushButton(this);
            this->button_save_->setText("save");
        }
        layout->addWidget(this->button_save_);
    }

    QObject::connect(this->button_copy_, &QPushButton::clicked, [this](bool) {
        GetMainWidget()->SkipOnce();

        QClipboard *board = QApplication::clipboard();

        auto pixmap = this->image_->pixmap();
        qDebug() << pixmap.size();
        board->setPixmap(this->image_->pixmap());
    });

    QObject::connect(this->image_, &ImageLabel::OnDoubleClicked, [this]() {
        auto label = new ImageLabel();
        label->setPixmap(this->image_->pixmap());
        label->show();
    });
    QObject::connect(this->button_save_, &QPushButton::clicked, this, &ImageWidget::OnButtonSaveClicked);
}

bool ImageWidget::Init(const QPixmap &pixmap) {
    image_->setPixmap(pixmap);

    this->size_->setText(QString("%1x%2").arg(pixmap.width()).arg(pixmap.height()));

    return true;
}

void ImageWidget::OnButtonSaveClicked() {
    qDebug() << "on save";
    auto file_name = QFileDialog::getSaveFileName(
            this,
            tr("save image"),
            QDir::currentPath() + QString("/%1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss")),
            tr("PNG (*.png);;JPEG (*.jpg *.jpeg);;BMP (*.bmp)"));
    if(file_name.isEmpty()) {
        qDebug() << "file name empty";
        return;
    }
    qDebug() << "file name " << file_name;
    this->image_->pixmap().save(file_name);
}


EntryWidget *CreateHtmlWidget(const QString &html, QWidget *parent) {
    auto widget = new HtmlWidget(parent);

    if(!widget->Init(html)) {
        delete widget;
        return nullptr;
    }

    return widget;
}

EntryWidget *CreateTextWidget(const QString &text, QWidget *parent) {
    auto widget = new TextWidget(parent);

    if(!widget->Init(text)) {
        delete widget;
        return nullptr;
    }

    return widget;
}

EntryWidget *CreateImageWidget(const QPixmap &pixmap, QWidget *parent) {
    auto widget = new ImageWidget(parent);

    if(!widget->Init(pixmap)) {
        delete widget;
        return nullptr;
    }

    return widget;
}
