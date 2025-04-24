#include "filetools.h"
#include <QFileInfo>
#include <QStandardPaths>
#include "ConstValue.h"
#include "user.h"
#include <chrono>
#include <iostream>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

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

bool compare(std::string datetime1, std::string datetime2)
{
    return datetime1.compare(datetime2) < 0;
}

// 查找目录中所有更改时间在datetime之后的文件
void FileTools::GetFiles(const std::string& directory,
                         std::vector<std::string>& files,
                         const std::string& datetime)
{
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        // std::cout << entry.path() << std::endl;
        auto last_modify = std::format("{:%Y-%m-%d %H:%M:%S}",
                                       std::chrono::system_clock::from_time_t(
                                           GetFileLatestModifyTime(entry.path())));
        if (last_modify.compare(datetime) > 0) { // 检查是否为子目录
            files.push_back(entry.path().c_str());
        }
    }
}

std::vector<unsigned char> Base64Decode(const std::string& input) {
    // 创建Base64解码的BIO链
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); // 不处理换行符

    // 将输入字符串放入内存BIO
    BIO* mem = BIO_new_mem_buf(input.data(), static_cast<int>(input.length()));
    mem = BIO_push(b64, mem);

    // 准备输出缓冲区
    std::vector<unsigned char> output(input.length()); // 解码后数据不会比输入长
    int decoded_length = BIO_read(mem, output.data(), static_cast<int>(input.length()));

    // 清理资源
    BIO_free_all(mem);

    if(decoded_length < 0) {
        throw std::runtime_error("Base64解码失败");
    }

    output.resize(decoded_length);
    return output;
}

void FileTools::SaveFileMsg(unsigned int& uid,
                            std::filesystem::path filename,
                            std::string data,
                            size_t length,
                            short filetype)
{
    unsigned int my_uid = User::GetInstance()->GetUid();

    std::filesystem::path root_path
        = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString();
    root_path = root_path / "ChatVisionUserInfo" / std::to_string(my_uid);

    std::string file_dir = filetype == MSG_FILE ? "file" : "picture";
    std::filesystem::path dir_name = root_path / "chatmsgs" / std::to_string(uid) / file_dir;

    std::filesystem::path file_name = std::filesystem::path(filename).filename().string();
    file_name = dir_name / file_name;

    if (!std::filesystem::exists(dir_name)) {
        InitChatMsgFiles(uid);
    }

    std::ofstream file(file_name, std::ios::app | std::ios::binary);
    if (!file) {
        std::cerr << "Failed to create file: " << file_name << std::endl;
        return;
    }

    auto decode = Base64Decode(data);
    file.write(std::string(decode.begin(),decode.end()).c_str(), length);
}

std::filesystem::path FileTools::GetRootPath()
{
    unsigned int my_uid = User::GetInstance()->GetUid();

    std::filesystem::path root_path
        = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString();
    root_path = root_path / "ChatVisionUserInfo" / std::to_string(my_uid);

    return root_path;
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
    CreateDir(dir_name / "file");
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

    CreateDir(msg_dir);

    std::filesystem::path object_file = msg_dir / (date_time + ".txt");

    json res;

    QFileInfo fileInfo(object_file);
    if (!fileInfo.exists())
        return res;

    std::ifstream file(object_file);
    std::string line;
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

    // 打开文件
    std::ofstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << file_path << std::endl;
        return false;
    }

    file << baseinfo << std::endl;
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

    for (json info : baseinfo) {
        file << info << std::endl;
    }
    file.close();
}

// std::vector<std::string> FileTools::GetCommunicationDir(std::string pattern)
// {
//     std::filesystem::path root_path
//         = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString();
//     std::cout << "root_path:" << root_path;
//     root_path = root_path / "ChatVisionUserInfo";
//     std::vector<std::string> communicatonDir;
//     bool ret;
//     for (const auto& entry : std::filesystem::directory_iterator(root_path)) {
//         const auto path = entry.path().filename().string();
//         std::cout << "File: " << path << std::endl;
//         ret = (path.find(pattern) != std::string::npos);
//         if (ret) {
//             communicatonDir.push_back(path);
//         }
//     }
//     return communicatonDir;
// }
