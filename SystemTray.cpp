#include "SystemTray.h"

SystemTray::SystemTray(QObject *parent) : QObject{parent} {}

SystemTray::~SystemTray() { qDebug() << "~SystemTray"; }


bool SystemTray::Init(const QIcon &icon, const QString &tips) {
    this->system_tray_ = new QSystemTrayIcon(this);
    this->system_tray_->setIcon(icon);

    this->menu_ = std::make_unique<QMenu>();
    this->system_tray_->setContextMenu(this->menu_.get());

    this->SetTip(tips);

    QObject::connect(this->system_tray_, &QSystemTrayIcon::activated, this, &SystemTray::onTrayActivated);

    this->system_tray_->show();

    return true;
}

void SystemTray::SetAction(const QString &name, int index) {
    Q_ASSERT(actions_.count(index) == 0);

    qDebug() << "add action " << index << " " << name;

    auto action = new QAction(name, this);
    QObject::connect(action, &QAction::triggered, this, [=]() { emit this->OnActionTriggered(index); });

    auto [it, ok] = this->actions_.insert(std::make_pair(index, action));
    Q_ASSERT(ok);

    if(it != this->actions_.begin()) {
        --it;
        menu_->insertAction(it->second, action);
    } else {
        if(this->actions_.size() == 1) {
            menu_->insertAction(nullptr, action);
        } else {
            RebuildMenuAction();
        }
    }
}

void SystemTray::RebuildMenuAction() {
    qDebug() << "rebuild menu";

    this->menu_->clear();
    for(auto &&it : actions_) {
        this->menu_->addAction(it.second);
    }
}
