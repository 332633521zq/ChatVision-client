#ifndef MYIMAGEPROVIDER_H
#define MYIMAGEPROVIDER_H

#include <QQuickImageProvider>

class MyImageProvider : public QQuickImageProvider
{
    Q_OBJECT
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)override;

public:
    void setAvater(QPixmap avater);
    static MyImageProvider *getInstance();
private:
    QPixmap m_avater;
    MyImageProvider();
    static MyImageProvider *m_myImageProvider;
};

#endif // MYIMAGEPROVIDER_H
