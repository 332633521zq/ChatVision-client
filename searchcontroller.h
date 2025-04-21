#pragma once
#include <QObject>
#include "msgsender.h"
class SearchController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString searchID READ searchID WRITE setSearchID NOTIFY searchIDChanged FINAL)
public:
    static SearchController &getInstance();
    QString searchID() const;
    void setSearchID(const QString sID);
    Q_INVOKABLE void searchUser();

private:
    explicit SearchController(QObject *parent = nullptr);
    QString m_searchID;
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
