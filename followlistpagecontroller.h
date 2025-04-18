#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QObject>
#include "sendmsg.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class FollowListPageController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<QJsonObject> followings READ GetFollowings NOTIFY onFollowingChanged FINAL)
    Q_PROPERTY(QList<QJsonObject> followers READ GetFollowers NOTIFY onFollowerChanged FINAL)
    Q_PROPERTY(QJsonObject myinfo READ GetMyinfo NOTIFY onMyInfoChanged FINAL)

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

    Q_INVOKABLE void initRelationData();
    Q_INVOKABLE void addFollowing(QString uid,
                                  QString avatar,
                                  QString gender,
                                  QString nickname,
                                  QString area,
                                  QString signature);
    Q_INVOKABLE void cancelFocus(QString uid);
signals:
    void onFollowingChanged();
    void onFollowerChanged();
    void onMyInfoChanged();

private:
    FollowListPageController(QObject* parent = nullptr);

    json m_followings;
    QList<QJsonObject> m_qmlFollowings;

    json m_followers;
    QList<QJsonObject> m_qmlFollowers;

    json m_myinfo;
    QJsonObject m_qmlMyinfo;

    void updateQmlFollowings();
    void updateQmlFollowers();
    void updateQmlMyinfo();
};
