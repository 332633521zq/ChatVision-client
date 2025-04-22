#pragma once
#include <QObject>
#include "msgsender.h"
#include <nlohmann/json.hpp>
using nlohmann::json;

class SearchController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString searchID READ searchID WRITE setSearchID NOTIFY searchIDChanged FINAL)
public:
    static SearchController &getInstance();
    QString searchID() const;
    void setSearchID(const QString sID);
    Q_INVOKABLE void searchUser();
    void setSearchInfo(nlohmann::json search_uinfo);
    nlohmann::json getSearchInfo();

private:
    explicit SearchController(QObject *parent = nullptr);
    QString m_searchID;
    nlohmann::json _search_uinfo;
    bool _isaddchatted = false;
signals:
    void searchIDChanged();
    void addSearchUserList(QString id,
                           QString memo,
                           QString nickname,
                           QString area,
                           QString gender,
                           QString signature,
                           QString avatar_path);
public slots:
    void onSearchIDChanged();
};
