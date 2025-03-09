#include "user.h"

User::User() {}

void User::SetUid(unsigned int uid)
{
    _uid = uid;
}

unsigned int User::GetUid()
{
    return _uid;
}

void User::SetMyInfo(const json &user_info)
{
    _my_info = user_info;
}
const json &User::GetMyInfo()
{
    return _my_info;
}

void User::InsertToFollowing(unsigned int uid, const json user_info)
{
    _following[uid] = user_info;
}
void User::RemoveFromFollowing(unsigned int uid)
{
    _following.erase(uid);
}

void User::InsertToFollower(unsigned int uid, const json user_info)
{
    _follower[uid] = user_info;
}
void User::RemoveFromFollower(unsigned int uid)
{
    _follower.erase(uid);
}

void User::InsertToBlacklist(unsigned int uid, const json user_info)
{
    _blacklist[uid] = user_info;
}
void User::RemoveFromBlacklist(unsigned int uid)
{
    _blacklist.erase(uid);
}

void User::InsertToChatted(unsigned int uid, const json user_info)
{
    _chatted[uid] = user_info;
}
void User::RemoveFromChatted(unsigned int uid)
{
    _chatted.erase(uid);
}
