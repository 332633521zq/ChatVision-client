#include "filetools.h"
#include <QStandardPaths>
#include "ConstValue.h"
#include "user.h"
#include <chrono>
#include <iostream>

FileTools::FileTools()
{
    _relation[RELATION_BLOCK] = "blacklist";
    _relation[RELATION_FOLLOWER] = "followers";
    _relation[RELATION_FOLLOWING] = "following";
    _relation[RELATION_INTERACT] = "interact";

    InitUserDirectory();
}

FileTools::~FileTools() {}

bool FileTools::CreateDir(std::filesystem::path dir_path)
{
    // 创建目录
    if (!std::filesystem::exists(dir_path)) {
        if (!std::filesystem::create_directories(dir_path)) {
            std::cerr << "Failed to create directory: " << dir_path << std::endl;
            return false;
        }
    }
    return true;
}

bool FileTools::CreateFile(std::filesystem::path file_path)
{
    // 创建或打开文件
    std::ofstream file(file_path, std::ios::app); // 以追加模式打开文件
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << file_path << std::endl;
        return false;
    }

    file.close();
    return true;
}

void FileTools::InitUserDirectory()
{
    unsigned int my_uid = User::GetInstance()->GetUid();

    std::filesystem::path root_path
        = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString();
    root_path = root_path / "ChatVisionUserInfo" / std::to_string(my_uid);

    CreateDir(root_path / "chatmsgs");
    CreateFile(root_path / "followers.txt");
    CreateFile(root_path / "following.txt");
    CreateFile(root_path / "interact.txt");
    CreateFile(root_path / "blacklist.txt");
}

void FileTools::InitChatMsgFiles(unsigned int uid)
{
    unsigned int my_uid = User::GetInstance()->GetUid();

    std::filesystem::path root_path
        = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString();
    root_path = root_path / "ChatVisionUserInfo" / std::to_string(my_uid);

    std::filesystem::path dir_name = root_path / "chatmsgs" / std::to_string(uid);

    CreateDir(dir_name / "textmsg");
    CreateDir(dir_name / "picture");
    CreateDir(dir_name / "video");
    CreateDir(dir_name / "audio");
}

bool FileTools::SaveUserInfo(const json& msg_data)
{
    unsigned int my_uid = User::GetInstance()->GetUid();

    std::filesystem::path root_path
        = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString();
    std::cout << "root_path:" << root_path;
    root_path = root_path / "ChatVisionUserInfo" / std::to_string(my_uid);

    std::filesystem::path file_path = root_path / "userinfo.txt";
    std::ofstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << file_path << std::endl;
        return false;
    }

    file << msg_data << std::endl;
    return true;
}

std::time_t FileTools::GetFileLatestModifyTime(std::filesystem::path file_path)
{
    auto writeTime = std::filesystem::last_write_time(file_path);
    auto systemTime = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        writeTime - std::filesystem::file_time_type::clock::now()
        + std::chrono::system_clock::now());
    auto writeTimeT = std::chrono::system_clock::to_time_t(systemTime);

    return writeTimeT;
}

// 获取目录中最后更改的文件路径
std::filesystem::path FileTools::GetLatestModifiedFile(const std::string& directory)
{
    std::filesystem::path latest_file;
    std::time_t latestTime = 0;

    // 遍历目录中的所有文件
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (std::filesystem::is_regular_file(entry)) {
            // 获取文件的最后修改时间
            auto writeTimeT = GetFileLatestModifyTime(entry);
            // 比较并更新最新文件
            if (writeTimeT > latestTime) {
                latestTime = writeTimeT;
                latest_file = entry.path();
            }
        }
    }

    return latest_file;
}

bool FileTools::SaveTextMsg(unsigned int& uid, json& msg_data)
{
    unsigned int my_uid = User::GetInstance()->GetUid();

    std::filesystem::path root_path
        = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString();
    std::cout << "root_path:" << root_path;
    root_path = root_path / "ChatVisionUserInfo" / std::to_string(my_uid);

    // 判断是否两位用户是否有聊天记录
    std::filesystem::path msg_dir = root_path / "chatmsgs" / std::to_string(uid);

    if (!std::filesystem::exists(msg_dir)) {
        InitChatMsgFiles(uid);
    }
    // 根据日期创建存储当天聊天消息的文件
    std::time_t now = std::time(nullptr);
    auto currentDate = std::localtime(&now);
    std::stringstream ss;
    ss << std::put_time(currentDate, "%Y-%m-%d");
    std::string curdate_str = ss.str();

    std::filesystem::path file_path = msg_dir / "textmsg" / (curdate_str + ".txt");
    // CreateFile(file_path);

    // 打开文件
    std::ofstream file(file_path, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << file_path << std::endl;
        return false;
    }

    // 获取当前时间
    auto nowtime = std::chrono::system_clock::now();
    auto now_seconds = std::chrono::time_point_cast<std::chrono::seconds>(nowtime);
    std::string timestamp = std::format("{:%Y-%m-%d %H:%M:%S}", now_seconds);

    // json json_msg = json::parse(msg_data);
    msg_data["datetime"] = timestamp;
    // 写入消息
    file << msg_data << std::endl;
    file.close();

    return true;
}

json FileTools::GetTextMsg(unsigned int& uid, std::string date_time)
{
    unsigned int my_uid = User::GetInstance()->GetUid();

    std::filesystem::path root_path
        = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString();
    std::cout << "root_path:" << root_path;
    root_path = root_path / "ChatVisionUserInfo" / std::to_string(my_uid);

    std::filesystem::path msg_dir = root_path / "chatmsgs" / std::to_string(uid) / "textmsg";

    std::filesystem::path object_file = msg_dir / (date_time + ".txt");
    std::ifstream file(object_file);
    std::string line;
    json res;
    while (std::getline(file, line)) {
        json json_line = json::parse(line);
        res.push_back(json_line);
    }

    file.close();
    return res;
}

json FileTools::GetLatestMsg(unsigned int& uid)
{
    unsigned int my_uid = User::GetInstance()->GetUid();

    std::filesystem::path root_path
        = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString();
    std::cout << "root_path:" << root_path;
    root_path = root_path / "ChatVisionUserInfo" / std::to_string(my_uid);

    std::filesystem::path msg_dir = root_path / "chatmsgs" / std::to_string(uid) / "textmsg";

    CreateDir(msg_dir);
    std::filesystem::path latest_file = msg_dir / GetLatestModifiedFile(msg_dir);
    std::ifstream file(latest_file);
    std::string line;
    json res;

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << latest_file << std::endl;
        return false;
    }
    while (std::getline(file, line)) {
        json json_line = json::parse(line);
        res.push_back(json_line);
    }
    file.close();
    return res;
}

bool FileTools::SaveRelation(unsigned int relation, unsigned int uid, json baseinfo)
{
    unsigned int my_uid = User::GetInstance()->GetUid();

    std::filesystem::path root_path
        = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString();
    std::cout << "root_path:" << root_path;
    root_path = root_path / "ChatVisionUserInfo" / std::to_string(my_uid);

    auto file_path = root_path / (_relation[relation] + ".txt");

    json relation_json;
    relation_json[std::to_string(uid)] = uid;
    relation_json["baseinfo"] = baseinfo;

    // 打开文件
    std::ofstream file(file_path, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << file_path << std::endl;
        return false;
    }

    file << relation_json << std::endl;
    file.close();
    return true;
}

json FileTools::GetRelations(unsigned int relation)
{
    unsigned int my_uid = User::GetInstance()->GetUid();

    std::filesystem::path root_path
        = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString();
    std::cout << "root_path:" << root_path;
    root_path = root_path / "ChatVisionUserInfo" / std::to_string(my_uid);

    auto file_path = root_path / (_relation[relation] + ".txt");

    // 打开文件
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << file_path << std::endl;
        return false;
    }

    json res;
    std::string line;
    while (std::getline(file, line)) {
        json json_line = json::parse(line);

        res.push_back(json_line);
    }

    file.close();
    return res;
}

void FileTools::RemoveRelation(unsigned int relation, unsigned int uid)
{
    unsigned int my_uid = User::GetInstance()->GetUid();

    std::filesystem::path root_path
        = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString();
    std::cout << "root_path:" << root_path;
    root_path = root_path / "ChatVisionUserInfo" / std::to_string(my_uid);

    auto file_path = root_path / (_relation[relation] + ".txt");

    // 打开文件
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << file_path << std::endl;
        return;
    }

    json res;
    std::string line;
    while (std::getline(file, line)) {
        auto json_line = json::parse(line);
        if (json_line[uid] == uid)
            continue;
        res.push_back(json_line);
    }
    file.close();

    std::ofstream outFile(file_path, std::ios::out | std::ios::trunc);
    if (!outFile.is_open()) {
        std::cerr << "无法打开文件！" << std::endl;
    }
    // 写入修改后的内容
    outFile << res;
    outFile.close();
}

void FileTools::SaveChattedUsers(json baseinfo)
{
    unsigned int my_uid = User::GetInstance()->GetUid();

    std::filesystem::path root_path
        = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString();
    std::cout << "root_path:" << root_path;
    root_path = root_path / "ChatVisionUserInfo" / std::to_string(my_uid);

    auto file_path = root_path / "chatted.txt";

    // 打开文件
    std::ofstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << file_path << std::endl;
        return;
    }

    file << baseinfo << std::endl;
    file.close();
}
