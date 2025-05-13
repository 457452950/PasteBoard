#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QObject>
#include <QGlobalStatic>
#include <QIcon>
#include <QSystemTrayIcon>
#include <QMenu>

#define GetSystemTray SystemTray::GetInstance

class SystemTray : public QObject {
    Q_OBJECT

    friend class QGlobalStatic<SystemTray>;

    explicit SystemTray(QObject *parent = nullptr);

public:
    ~SystemTray();

    static SystemTray *GetInstance() {
        static SystemTray instance_;
        return &instance_;
    }

    bool Init(const QIcon &icon, const QString &tips);

    void SetAction(const QString &name, int index);

    void SetTip(const QString &text) { system_tray_->setToolTip(text); }

signals:
    void OnActionTriggered(int index);
    void onTrayActivated(QSystemTrayIcon::ActivationReason reason);

private:
    void RebuildMenuAction();

private:
    QSystemTrayIcon         *system_tray_{nullptr};
    std::unique_ptr<QMenu>   menu_{nullptr};
    std::map<int, QAction *> actions_;
};

#endif // SYSTEMTRAY_H
