#include <QApplication>
#include <QDebug>

#include "MainWidget.h"
#include "SystemTray.h"
#include "Setting.h"
#include "Client.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);

    auto setting_file = QApplication::applicationDirPath() + "/setting.ini";
    auto ok           = GetSetting()->LoadSetting(setting_file);
    if(!ok) {
        qDebug() << "load config fail";
        return -1;
    }

    GetSystemTray()->Init(QIcon(":/image/resource/tray.ico"), "tip");
    GetSystemTray()->SetAction("Quit", 1);
    QObject::connect(GetSystemTray(), &SystemTray::OnActionTriggered, [&](int index) {
        if(index == 1) {
            qDebug() << "quit";
            app.quit();
        }
    });

    Client client;
    client.Init(QUrl("wss://192.168.2.11:9001"));

    MainWidget *w = GetMainWidget();
    w->Init();
    w->show();

    return app.exec();
}
