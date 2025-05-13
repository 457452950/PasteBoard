#ifndef SETTING_H
#define SETTING_H

#include <QSettings>

#define GetSetting Setting::GetInstance

struct DatabaseConfig {
    static QString DefaultName;
    static QString DefaultHostName;
    static QString DefaultPortName;
    static QString DefaultUserNameName;
    static QString DefaultPasswordName;

    static QString DefaultHost;
    static int16_t DefaultPort;
    static QString DefaultUserName;
    static QString DefaultPassword;

    QString Host;
    int16_t Port;

    QString UserName;
    QString Password;
};

class Setting {

private:
    Setting();

public:
    ~Setting();

    static Setting *GetInstance() {
        static Setting instance;
        return &instance;
    }

    bool LoadSetting(const QString &file) {
        setting_ = std::make_unique<QSettings>(file, QSettings::IniFormat);
        if(!setting_) {
            return false;
        }
        qDebug() << "load from " << file;

        setting_file_ = file;
        return true;
    }

    bool SaveSetting();

    DatabaseConfig GetDatabaseConfig();

private:
    QString                    setting_file_;
    std::unique_ptr<QSettings> setting_;
};

#endif // SETTING_H
