#include "msgsender.h"
#include <QDebug>
#include "ConstValue.h"
#include "user.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <iomanip>

using namespace nlohmann;

MsgSender::MsgSender()
    : _token_bucket(std::make_shared<TokenBucket>(10, 10))
{}

MsgSender::~MsgSender() {}

void MsgSender::SetSocket(boost::asio::ip::tcp::socket* sock)
{
    _sock = sock;
}

void MsgSender::SendRequest(std::string msg, unsigned int object_id, short msgid)
{
    // std::lock_guard<std::mutex> lock(_mutex);

    json send_str;
    send_str["uid"] = User::GetInstance()->GetUid();
    send_str["object_id"] = object_id;
    send_str["data"] = msg;
    send_str["msgid"] = msgid;

    std::string temp_send_str = send_str.dump();

    std::shared_ptr<SendNode> new_node = std::make_shared<SendNode>(temp_send_str.c_str(),
                                                                    temp_send_str.length(),
                                                                    object_id,
                                                                    msgid);

    _send_que.push(new_node);

    while (_send_que.size() >= 1) {
        bool is_success = _token_bucket->acquire(1);

        if (!is_success) {
            sleep(1);
            continue;
        }

        auto& send_node = _send_que.front();
        boost::asio::write(*_sock, boost::asio::buffer(send_node->_data, send_node->_total_len));
        _send_que.pop();
    }
}

// Base64编码工具函数
std::string Base64Encode(const unsigned char* input, size_t length) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // 不换行
    BIO_write(bio, input, length);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);

    std::string result(bufferPtr->data, bufferPtr->length);
    BIO_free_all(bio);

    return result;
}

void MsgSender::SendFile(std::filesystem::path file_path,
                         unsigned int object_id,
                         const short& filetype)
{
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << file_path << std::endl;
        return;
    }
    // 发送文件大小
    file.seekg(0, std::ios::end);       // 将文件读指针位置置为文件末
    size_t file_size = file.tellg();    // 获取文件读指针当前的位置，即该文件的长度
    file.seekg(0, std::ios::beg);       // 将文件读指针重新置为文件头


    // "file_name": "example.zip",      // 文件名
    // "file_size": 102400,             // 文件总大小（字节）
    // "total_chunks": 10,              // 总数据块数
    // "chunk_index": 0,                // 当前块序号
    // "chunk_data": "Base64数据..."    // 文件数据块（Base64编码）

    const size_t MAX_CHUNK_SIZE = MAX_LENGTH / 2;       // Base64编码后会膨胀
    char buf[MAX_CHUNK_SIZE];
    size_t total_chunks = (file_size + MAX_CHUNK_SIZE - 1) / MAX_CHUNK_SIZE;
    int chunk_num = 1;
    while(!file.eof()){
        memset(buf,0,MAX_CHUNK_SIZE);
        file.read(buf,MAX_CHUNK_SIZE);
        size_t bytes_read = file.gcount();
        if(bytes_read == 0) break;

        json fileinfo;
        std::string path = std::filesystem::path(file_path).filename().string();
        fileinfo["filename"] = path;
        fileinfo["filesize"] = file_size;
        fileinfo["total_chunks"] = total_chunks;
        fileinfo["chunk_index"] = chunk_num++;
        fileinfo["file_type"] = filetype;
        fileinfo["data_size"] = bytes_read;

        fileinfo["chunk_data"] = Base64Encode(
                    reinterpret_cast<const unsigned char*>(buf),
                    bytes_read
                );

        std::string send_str = fileinfo.dump();
        SendRequest(send_str,object_id,MSG_FILE);
    }
}

void MsgSender::SendRequest(char* msg, size_t msg_len, unsigned int object_id, short msgid)
{
    json send_str;
    send_str["uid"] = User::GetInstance()->GetUid();
    send_str["object_id"] = object_id;
    send_str["data"] = std::string(msg, msg_len);
    std::string temp_send_str = send_str.dump();

    std::shared_ptr<SendNode> new_node = std::make_shared<SendNode>(temp_send_str.c_str(),
                                                                    temp_send_str.length(),
                                                                    object_id,
                                                                    msgid);

    _send_que.push(new_node);

    while (_send_que.size() >= 1) {
        bool is_success = _token_bucket->acquire(1);

        if (!is_success) {
            sleep(1);
            continue;
        }

        auto& send_node = _send_que.front();
        boost::asio::write(*_sock, boost::asio::buffer(send_node->_data, send_node->_total_len));
        _send_que.pop();
    }
}
