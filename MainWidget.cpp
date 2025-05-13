#include "MainWidget.h"

#include <QGuiApplication>
#include <QClipboard>
#include <QMimeData>
#include <QListWidgetItem>
#include <QTimer>

#include "SystemTray.h"
#include "EntryWidget.h"
#include "Setting.h"


MainWidget::MainWidget(QWidget *parent) : QWidget(parent) {
    this->setWindowIcon(QIcon(":/image/resource/tray.ico"));
    this->setMinimumSize({600, 800});

    this->layout_ = new QVBoxLayout(this);

    this->list_widget_ = new QListWidget(this);
    this->layout_->addWidget(list_widget_);
    this->list_widget_->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);

    // connect
    auto board = QGuiApplication::clipboard();
    QObject::connect(board, &QClipboard::dataChanged, this, &MainWidget::OnClipBoardDataChanged);
}

MainWidget::~MainWidget() {
    qDebug() << "~MainWidget";
    Setting::GetInstance()->SaveSetting();
}

void MainWidget::Init() {
    bool ok;

    ok = QObject::connect(GetSystemTray(), &SystemTray::onTrayActivated, this, &MainWidget::OnSystemTrayActivated);
    if(!ok) {
        qDebug() << "connect fail";
    }

    GetSystemTray()->SetAction("Open", 0);
    QObject::connect(GetSystemTray(), &SystemTray::OnActionTriggered, [=](int index) {
        if(index == 0 && !this->isVisible()) {
            QSize originalSize = this->size();
            qDebug() << "show" << this->pos() << this->size();
            this->show();
            QTimer::singleShot(22, [=]() {
                qDebug() << "resize" << this->pos() << originalSize;
                this->resize(originalSize);
            });
        }
    });
}


void MainWidget::OnSystemTrayActivated(QSystemTrayIcon::ActivationReason reason) {
    qDebug() << "main window" << reason;
    if(reason == QSystemTrayIcon::DoubleClick) {
        QSize originalSize = this->size();
        if(!this->isVisible()) {
            qDebug() << "show" << this->pos() << this->size();
            this->show();
        }
        if(this->isMinimized()) {
            qDebug() << "active " << this->pos() << this->size();
            this->showNormal();
        }
        QTimer::singleShot(22, [=]() {
            qDebug() << "resize" << this->pos() << originalSize;
            this->resize(originalSize);
        });
    }
}

void MainWidget::OnClipBoardDataChanged() {
    if(skip_once_) {
        skip_once_ = false;
        qDebug() << "skip once";
        return;
    }

    auto             board    = QGuiApplication::clipboard();
    const QMimeData *mimeData = board->mimeData();
    if(!mimeData) {
        qDebug() << "mime data is nullptr, text : " << board->text();
        return;
    }

    EntryWidget *wid  = nullptr;
    auto         item = new QListWidgetItem();
    item->setSizeHint({300, 200});

    if(mimeData->hasImage()) {
        auto image = board->image();
        qDebug() << "image" << image.size();
        auto pixmap = qvariant_cast<QPixmap>(mimeData->imageData());
        if(pixmap.isNull()) {
            qDebug() << "trans to pixmap fail";
            return;
        }
        wid = CreateImageWidget(pixmap, this);
        item->setSizeHint({300, 300});
    } else if(mimeData->hasHtml()) {
        qDebug() << "html " << board->text() << mimeData->html();
        wid = CreateHtmlWidget(mimeData->html(), this);
    } else if(mimeData->hasText()) {
        qDebug() << "text " << board->text();
        wid = CreateTextWidget(board->text(), this);
    }

    this->list_widget_->insertItem(0, item);
    this->list_widget_->setItemWidget(item, wid);
}
