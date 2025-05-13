#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent>
#include <QBoxLayout>
#include <QListWidget>

#define GetMainWidget MainWidget::GetInstance

class MainWidget : public QWidget {
    Q_OBJECT

    MainWidget(QWidget *parent = nullptr);

public:
    ~MainWidget();

    static MainWidget *GetInstance() {
        static MainWidget widget;
        return &widget;
    }

private:
    // 重写关闭事件
    void closeEvent(QCloseEvent *event) override {
        qDebug() << "hide" << this->pos() << this->size();
        hide();          // 隐藏主窗口
        event->ignore(); // 忽略关闭事件，阻止程序退出
    }

public:
    void Init();

    void SkipOnce() { this->skip_once_ = true; }

public slots:
    void OnSystemTrayActivated(QSystemTrayIcon::ActivationReason reason);
    void OnClipBoardDataChanged();

private:
    QVBoxLayout *layout_{nullptr};
    QListWidget *list_widget_{nullptr};

    bool skip_once_{false};
};

#endif // MAINWIDGET_H
