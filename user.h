#pragma once

#include "filetools.h"
#include "singleton.h"
#include <nlohmann/json.hpp>

using nlohmann::json;

class User : public Singleton<User>
{
    friend class Singleton<User>;

public:
    User();
    void SetUid(unsigned int uid);
    unsigned int GetUid();

    void SetMyInfo(const json &user_info);
    const json &GetMyInfo();

    void InsertToFollowing(unsigned int uid, const json user_info);
    void RemoveFromFollowing(unsigned int uid);

    void InsertToFollower(unsigned int uid, const json user_info);
    void RemoveFromFollower(unsigned int uid);

    void InsertToBlacklist(unsigned int uid, const json user_info);
    void RemoveFromBlacklist(unsigned int uid);

    void InsertToChatted(unsigned int uid, const json user_info);
    void RemoveFromChatted(unsigned int uid);

private:
    unsigned int _uid;
    json _my_info;
    std::map<unsigned int, json> _following;
    std::map<unsigned int, json> _follower;
    std::map<unsigned int, json> _blacklist;
    std::map<unsigned int, json> _chatted;
};
