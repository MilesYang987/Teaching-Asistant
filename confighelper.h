#ifndef CONFIGHELPER_H
#define CONFIGHELPER_H

#include <string>

class ConfigHelper
{
public:
    static ConfigHelper& GetInstance(){
        static ConfigHelper instance;
        return instance;
    }

    std::string ReadConfig(const std::string& section,const std::string& key);

    void WriteConfig(const std::string& section,const std::string& key,const std::string& value);
    void RemoveConfig(const std::string& section,const std::string& key);

private:
    ConfigHelper();
    ConfigHelper(const ConfigHelper&) = delete;
    ConfigHelper& operator=(const ConfigHelper&) = delete;
    ~ConfigHelper() = default;

    bool HasSection(const std::string &section);

private:
    std::string file;

};

#endif // CONFIGHELPER_H
