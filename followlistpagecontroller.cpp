#include "followlistpagecontroller.h"
#include "msgsender.h"
#include <QJsonDocument>
#include "user.h"

FollowListPageController::FollowListPageController(QObject* parent)
    : QObject(parent)
{
    // InitRelationData();
}

FollowListPageController::~FollowListPageController() {}

FollowListPageController& FollowListPageController::getInstance()
{
    static FollowListPageController flc;
    return flc;
}

void FollowListPageController::CancelFollowing(const QString& object_id)
{
    // 实现取消关注的逻辑
}

QList<QJsonObject> FollowListPageController::GetFollowings() const
{
    return m_qmlFollowings;
}

void FollowListPageController::SetFollowings(const json& data)
{
    m_followings = data;
    updateQmlFollowings();     // 更新 QML 数据
    emit onFollowingChanged(); // 通知 QML 数据已更新
}

QList<QJsonObject> FollowListPageController::GetFollowers() const
{
    return m_qmlFollowers;
}

void FollowListPageController::SetFollowers(const json& data)
{
    m_followers = data;
    updateQmlFollowers();
    emit onFollowerChanged();
}

QJsonObject FollowListPageController::GetMyinfo() const
{
    return m_qmlMyinfo;
}

void FollowListPageController::SetMyInfo(const json& data)
{
    m_myinfo = data;
    updateQmlMyinfo();
    emit onMyInfoChanged();
}

bool FollowListPageController::GetOnlineState() const
{
    return m_isonline;
}

void FollowListPageController::SetOnlineState(const bool onlinestate)
{
    m_isonline = onlinestate;
}

void FollowListPageController::getOnlineState(const unsigned int& uid)
{
    std::cout<<"getOnlineState sendrequest--------"<<std::endl;
    MsgSender::GetInstance()->SendRequest("",uid,MSG_ONLINE_STATE);
}

void FollowListPageController::initRelationData()
{
    json res;
    const auto& followings = User::GetInstance()->GetFollowing();
    for (const auto& pair : followings) {
        const json& baseinfo = pair.second;
        res.push_back(baseinfo);
    }
    SetFollowings(res);

    json follower;
    const auto& followers = User::GetInstance()->GetFollower();
    for (const auto& pair : followers) {
        const json& baseinfo = pair.second;
        follower.push_back(baseinfo);
    }
    SetFollowers(follower);
    SetMyInfo(User::GetInstance()->GetMyInfo());
}

void FollowListPageController::updateQmlFollowings()
{
    m_qmlFollowings.clear();
    for (const json& item : m_followings) {
        // {"area":"重庆","avatar_path_":"path","gender":"女","memo":"85","nickname":"85","signature":"罪恶没有假期，正义便无暇休憩","uid":"20000000"}
        QJsonObject qjsonObj;
        std::string uid_str = item["uid"];
        qjsonObj["uid"] = QString::fromStdString(uid_str);
        qjsonObj["gender"] = QString::fromStdString(item["gender"]);
        qjsonObj["avatar_path_"] = QString::fromStdString(item["avatar_path_"]);
        qjsonObj["nickname"] = QString::fromStdString(item["nickname"]);
        qjsonObj["area"] = QString::fromStdString(item["area"]);
        qjsonObj["signature"] = QString::fromStdString(item["signature"]);

        m_qmlFollowings.append(qjsonObj);
    }
}

void FollowListPageController::updateQmlFollowers()
{
    m_qmlFollowers.clear();
    for (const json& item : m_followers) {
        // {"area":"重庆","avatar_path_":"path","gender":"女","memo":"85","nickname":"85","signature":"罪恶没有假期，正义便无暇休憩","uid":"20000000"}
        QJsonObject qjsonObj;
        std::string uid_str = item["uid"];
        qjsonObj["uid"] = QString::fromStdString(uid_str);
        qjsonObj["gender"] = QString::fromStdString(item["gender"]);
        qjsonObj["avatar_path_"] = QString::fromStdString(item["avatar_path_"]);
        qjsonObj["nickname"] = QString::fromStdString(item["nickname"]);
        qjsonObj["area"] = QString::fromStdString(item["area"]);
        qjsonObj["signature"] = QString::fromStdString(item["signature"]);

        m_qmlFollowers.append(qjsonObj);
    }
}

void FollowListPageController::updateQmlMyinfo()
{
    // {"area":"重庆","avatar_path_":"path","gender":"女","memo":"85","nickname":"85","signature":"罪恶没有假期，正义便无暇休憩","uid":"20000000"}
    if (m_myinfo.size() == 0)
        return;
    std::string uid_str = m_myinfo["uid"];
    m_qmlMyinfo["uid"] = QString::fromStdString(uid_str);
    m_qmlMyinfo["gender"] = QString::fromStdString(m_myinfo["gender"]);
    m_qmlMyinfo["avatar_path_"] = QString::fromStdString(m_myinfo["avatar_path_"]);
    m_qmlMyinfo["nickname"] = QString::fromStdString(m_myinfo["nickname"]);
    m_qmlMyinfo["area"] = QString::fromStdString(m_myinfo["area"]);
    m_qmlMyinfo["signature"] = QString::fromStdString(m_myinfo["signature"]);
    emit initPersonalInfo(GetMyinfo());
}
void FollowListPageController::addFollowing(QString uid,
                                            QString avatar_path,
                                            QString gender,
                                            QString nickname,
                                            QString area,
                                            QString signature)
{
    QJsonObject qjsonObj;
    qjsonObj["uid"] = uid;
    qjsonObj["gender"] = gender;
    qjsonObj["avatar_path_"] = avatar_path;
    qjsonObj["nickname"] = nickname;
    qjsonObj["area"] = area;
    qjsonObj["signature"] = signature;

    m_qmlFollowings.append(qjsonObj);
    emit onFollowingChanged();
}

void FollowListPageController::cancelFocus(QString uid)
{
    unsigned int id = uid.toUInt();
    User::GetInstance()->RemoveFromFollowing(id);
    json res;
    const auto& followings = User::GetInstance()->GetFollowing();
    for (const auto& pair : followings) {
        const json& baseinfo = pair.second;
        res.push_back(baseinfo);
    }
    SetFollowings(res);
    MsgSender::GetInstance()->SendRequest("", id, MSG_CANCEL_FOLLOW);
}

void FollowListPageController::changePersonalInfo(QString nickname,
                                                  QString gender,
                                                  QString area,
                                                  QString signature,
                                                  QString avatar_path,
                                                  QString uid)
{
    std::string s_nickname = nickname.toStdString();
    std::string s_gender = gender.toStdString();
    std::string s_area = area.toStdString();
    std::string s_signature = signature.toStdString();
    std::string s_uid = uid.toStdString();
    std::string s_avatar_path = avatar_path.toStdString();
    json myinfo;
    myinfo["nickname"] = s_nickname;
    myinfo["gender"] = s_gender;
    myinfo["area"] = s_area;
    myinfo["signature"] = s_signature;
    myinfo["uid"] = s_uid;
    myinfo["avatar_path_"] = s_avatar_path;
    User::GetInstance()->SetMyInfo(myinfo);
    SetMyInfo(User::GetInstance()->GetMyInfo());
}
