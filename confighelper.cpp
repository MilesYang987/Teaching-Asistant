#include "confighelper.h"
#include <fstream>
#include <map>


enum LineType{
    SECTION=1,
    DICT=2,
    COMMENT=3,
    NOTVALID=0
};

std::string trim(const std::string& s){
    size_t first = s.find_first_not_of(" \t\n\r"); // 找到第一个非空白字符
    if (first == std::string::npos) return ""; // 如果字符串全是空白，返回空字符串
    size_t last = s.find_last_not_of(" \t\n\r"); // 找到最后一个非空白字符
    return s.substr(first, last - first + 1); // 返回修整后的字符串
}


LineType checkLine(const std::string& line){
    if (line.empty() || line == "\n") return LineType::NOTVALID;
    if (line[0] == '[' && line[line.size()-1] == ']') return LineType::SECTION;
    if (line[0] == '#') return LineType::COMMENT;
    if (line.find('=')!=line.npos && !trim(line.substr(0,line.find('='))).empty()) return LineType::DICT;
    return LineType::NOTVALID;
}


struct INI
{
    std::map<std::string, std::map<std::string, std::string>> data;
    std::string file;

    std::string extractSection(const std::string& line);
    std::pair<std::string,std::string> extractDict(const std::string& line);

    bool hasSection(const std::string& section);
    void addSection(const std::string& section);
    bool Parse();
    bool Serialize();

    INI(const std::string& f):file(f){};
};


std::string INI::extractSection(const std::string& line){
    return line.substr(1,line.size()-2);
}

std::pair<std::string,std::string> INI::extractDict(const std::string& line){
    size_t pos = line.find('=');
    return std::make_pair(trim(line.substr(0,pos)),trim(line.substr(pos+1,line.size()-pos+2)));
}

bool INI::hasSection(const std::string &section)
{
    if(section.empty()) return false;
    if (!Parse()) return false;
    return data.find(section) != data.end();
}

void INI::addSection(const std::string &section)
{
    std::fstream fs;
    fs.open(file,std::ios::out | std::ios::app);
    if (!fs.is_open()){
        fs.close();
        return;
    }
    fs << "[" + section + "]\n";
    fs.close();
}

bool INI::Parse(){
    if (file.empty()) return false;
    std::fstream fs(file);
    if(!fs) return false;
    else{
        data.clear();
        std::string temp_section = "";
        auto temp_dict = std::map<std::string,std::string>();

        std::string line;
        while(std::getline(fs,line)){

            switch (checkLine(line)) {
            case LineType::SECTION:
                if (extractSection(line) != temp_section){
                    if (!temp_section.empty()) data.emplace(std::make_pair(temp_section,temp_dict));
                    temp_dict.clear();
                    temp_section = extractSection(line);
                }
                break;
            case LineType::DICT:
                if (temp_section.empty()) break;
                temp_dict.emplace(extractDict(line));
                break;
            default:
                break;
            }
        }
        data.emplace(std::make_pair(temp_section,temp_dict));

        fs.close();
        if (data.size() == 0) return false;
        return true;
    }

}

bool INI::Serialize()
{
    if (file.empty()) return false;
    std::fstream fs;
    fs.open(file, std::ios::out | std::ios::trunc);
    if (!fs.is_open()) {
        fs.close();
        return false;
    }

    for (auto &it : data){
        if (it.second.empty()) continue;
        fs  << "[" + it.first + "]\n";
        for (auto &t : it.second){
            fs << t.first + " = " + t.second + "\n";
        }
        fs << "\n";
    }
    fs.close();
    return true;
}


std::string ConfigHelper::ReadConfig(const std::string& section,const std::string& key)
{
    INI reader(this->file);

    if (reader.Parse()){
        auto it = reader.data.find(section);
        if (it == reader.data.end()) return "";
        auto t = it->second.find(key);
        if (t == it->second.end()) return "";
        else return t->second;
    }
    return "";
}



void ConfigHelper::WriteConfig(const std::string& section,const std::string& key,const std::string& value)
{
    if (section.empty() || key.empty()){
        return;
    }

    INI reader(this->file);
    if (!reader.Parse()) return;

    if (!HasSection(section)){
        reader.addSection(section);
        reader.Parse();
    }

    for (auto &it : reader.data){
        if (it.first == section)
        {
            if (it.second.empty()){
                auto temp_dict = std::map<std::string, std::string>();
                temp_dict.emplace(std::make_pair(key,value));
                it.second = temp_dict;
            }
            else{
                it.second[key] = value;
            }
            break;
        }
    }

    reader.Serialize();

}

void ConfigHelper::RemoveConfig(const std::string &section, const std::string &key)
{

    if (section.empty() || key.empty()){
        return;
    }

    if(!this->HasSection(section)) return;

    INI reader(this->file);
    if (!reader.Parse()) return;

    auto it = reader.data.find(section);
    if (it == reader.data.end()) return;
    else it->second.erase(key);

    reader.Serialize();

}

ConfigHelper::ConfigHelper() : file("config.ini")
{
    std::fstream fs(file);
    if (!fs) {
        fs.open(file, std::ios::out | std::ios::trunc);
        WriteConfig("sys","version","0.1");
        WriteConfig("sys","first","1");
        WriteConfig("mysql","ip","localhost");
        WriteConfig("mysql","port","3306");
    }
    fs.close();
}

bool ConfigHelper::HasSection(const std::string &section)
{
    if (section.empty()) return false;
    INI reader(this->file);
    return reader.hasSection(section);
}



