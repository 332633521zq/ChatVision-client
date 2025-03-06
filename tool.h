#pragma once
#include <QObject>
class Tool
{
public:
    static Tool &getInstance();
    void getJsonFromFile(QVector<QJsonObject> &msgs, const QString &filePath);
    bool saveJsonObjectToFile(QJsonObject &jo, const QString &filePath);

private:
    Tool();
};
