#pragma once

#include "nlohmann/json.hpp"
#include "singleton.h"
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>

using nlohmann::json;

class FileTools : public Singleton<FileTools>
{
    friend class Singleton<FileTools>;

public:
    ~FileTools();

    bool CreateDir(std::filesystem::path dir_path);
    bool CreateFile(std::filesystem::path file_path);
    void InitUserDirectory();
    void InitChatMsgFiles(unsigned int uid);
    std::filesystem::path GetLatestModifiedFile(const std::string& directory);
    std::time_t GetFileLatestModifyTime(std::filesystem::path file_path);
    bool SaveUserInfo(const json& msg_data);
    bool SaveTextMsg(unsigned int& uid, json& msg_data);
    json GetTextMsg(unsigned int& uid, std::string date_time);
    json GetLatestMsg(unsigned int& uid);
    bool SaveRelation(unsigned int relation, unsigned int uid, json baseinfo);
    json GetRelations(unsigned int relation);
    void RemoveRelation(unsigned int relation, unsigned int uid);
    void SaveChattedUsers(json baseinfo);
    // std::vector<std::string> GetCommunicationDir(std::string pattern);

    void GetFiles(const std::string& directory,
                  std::vector<std::string>& files,
                  const std::string& datetime);
    void SaveFileMsg(unsigned int& uid,
                     std::filesystem::path filename,
                     std::string data,
                     size_t length,
                     short filetype);

    std::filesystem::path GetRootPath();

private:
    FileTools();
    std::map<unsigned int, std::string> _relation;
};
