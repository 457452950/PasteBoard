#include "Setting.h"

QString DatabaseConfig::DefaultName         = "database";
QString DatabaseConfig::DefaultHostName     = DefaultName + "/host";
QString DatabaseConfig::DefaultPortName     = DefaultName + "/port";
QString DatabaseConfig::DefaultUserNameName = DefaultName + "/user";
QString DatabaseConfig::DefaultPasswordName = DefaultName + "/passwd";

QString DatabaseConfig::DefaultHost     = "127.0.0.1";
int16_t DatabaseConfig::DefaultPort     = 5432;
QString DatabaseConfig::DefaultUserName = "wlb";
QString DatabaseConfig::DefaultPassword = "postgres";

Setting::Setting() {}

Setting::~Setting() {}

bool Setting::SaveSetting() {
    if(this->setting_file_.isEmpty()) {
        qDebug() << "cant save setting, file name is empty";
        return false;
    }
    if(!this->setting_) {
        qDebug() << "cant save setting, setting nullptr";
        return false;
    }

    qDebug() << "save setting";
    this->setting_->sync();
    return true;
}

DatabaseConfig Setting::GetDatabaseConfig() {
    if(!this->setting_->contains(DatabaseConfig::DefaultName)) {
        this->setting_->setValue(DatabaseConfig::DefaultHostName, DatabaseConfig::DefaultHost);
        this->setting_->setValue(DatabaseConfig::DefaultPortName, DatabaseConfig::DefaultPort);
        this->setting_->setValue(DatabaseConfig::DefaultUserNameName, DatabaseConfig::DefaultUserName);
        this->setting_->setValue(DatabaseConfig::DefaultPasswordName, DatabaseConfig::DefaultPassword);
    }

    DatabaseConfig config;
    config.Host = this->setting_->value(DatabaseConfig::DefaultHostName, DatabaseConfig::DefaultHost).toString();
    config.Port = this->setting_->value(DatabaseConfig::DefaultPortName, DatabaseConfig::DefaultPort).toInt();
    config.UserName =
            this->setting_->value(DatabaseConfig::DefaultUserNameName, DatabaseConfig::DefaultUserName).toString();
    config.Password =
            this->setting_->value(DatabaseConfig::DefaultPasswordName, DatabaseConfig::DefaultPassword).toString();
    return config;
}
