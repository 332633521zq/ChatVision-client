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
    std::map<unsigned int, json> &GetFollowing();

    void InsertToFollower(unsigned int uid, const json user_info);
    void RemoveFromFollower(unsigned int uid);
    std::map<unsigned int, json> &GetFollower();

    void InsertToBlacklist(unsigned int uid, const json user_info);
    void RemoveFromBlacklist(unsigned int uid);
    std::map<unsigned int, json> &GetBlackList();

    void InsertToChatted(unsigned int uid, const json user_info);
    void RemoveFromChatted(unsigned int uid);
    std::map<unsigned int, json> &GetChatted();

private:
    unsigned int _uid;
    json _my_info;
    std::map<unsigned int, json> _following;
    std::map<unsigned int, json> _follower;
    std::map<unsigned int, json> _blacklist;
    std::map<unsigned int, json> _chatted;
};
