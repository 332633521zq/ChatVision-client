#include "tool.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>
Tool &Tool::getInstance()
{
    static Tool tool;
    return tool;
}

void Tool::getJsonFromFile(QVector<QJsonObject> &msgs, const QString &filePath)
{
    qDebug() << "file path is:" << filePath;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file for reading:" << file.errorString();
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(line.toUtf8());
        if (jsonDoc.isNull() || !jsonDoc.isObject()) {
            qWarning() << "Failed to parse JSON object from line.";
            continue;
        }
        // 提取 JSON 对象
        QJsonObject json = jsonDoc.object();
        msgs.append(json);
    }
    file.close();
}

bool Tool::saveJsonObjectToFile(QJsonObject &jo, const QString &filePath)
{
    QJsonDocument document(jo);
    QFile file(filePath);
    if (!file.open(QIODevice::Append)) {
        qWarning() << "Failed to open file for writint:" << file.errorString();
        return false;
    }
    if (file.size() > 0) {
        file.write("\n");
    }
    file.write(document.toJson(QJsonDocument::Compact));
    file.close();
    qDebug() << "写入文件成功";
    return true;
}

Tool::Tool() {}
