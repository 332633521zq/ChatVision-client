#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QObject>
#include "msgsender.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class FollowListPageController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<QJsonObject> followings READ GetFollowings NOTIFY onFollowingChanged FINAL)
    Q_PROPERTY(QList<QJsonObject> followers READ GetFollowers NOTIFY onFollowerChanged FINAL)
    Q_PROPERTY(QJsonObject myinfo READ GetMyinfo NOTIFY onMyInfoChanged FINAL)
    Q_PROPERTY(bool isonline READ GetOnlineState NOTIFY onIsOnlineChanged FINAL)

public:
    ~FollowListPageController();
    static FollowListPageController& getInstance();

    void CancelFollowing(const QString& object_id);
    QList<QJsonObject> GetFollowings() const;
    void SetFollowings(const json& data);

    QList<QJsonObject> GetFollowers() const;
    void SetFollowers(const json& data);

    QJsonObject GetMyinfo() const;
    void SetMyInfo(const json& data);

    bool GetOnlineState() const;
    void SetOnlineState(const bool onlinestate);

    Q_INVOKABLE void getOnlineState(const unsigned int& uid);
    Q_INVOKABLE void initRelationData();
    Q_INVOKABLE void addFollowing(QString uid,
                                  QString avatar,
                                  QString gender,
                                  QString nickname,
                                  QString area,
                                  QString signature);
    Q_INVOKABLE void cancelFocus(QString uid);
    Q_INVOKABLE void changePersonalInfo(QString nickname,
                                        QString gender,
                                        QString area,
                                        QString signature,
                                        QString avatar_path,
                                        QString uid);
signals:
    void onFollowingChanged();
    void onFollowerChanged();
    void onMyInfoChanged();

    void initPersonalInfo(QJsonObject myInfo);
    void onIsOnlineChanged();

private:
    FollowListPageController(QObject* parent = nullptr);

    json m_followings;
    QList<QJsonObject> m_qmlFollowings;

    json m_followers;
    QList<QJsonObject> m_qmlFollowers;

    json m_myinfo;
    QJsonObject m_qmlMyinfo;

    bool m_isonline;
    void updateQmlFollowings();
    void updateQmlFollowers();
    void updateQmlMyinfo();
};
