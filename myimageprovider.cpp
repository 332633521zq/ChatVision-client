#include "myimageprovider.h"
#include "mediathread.h"
MyImageProvider *MyImageProvider::m_myImageProvider=nullptr;
MyImageProvider::MyImageProvider():QQuickImageProvider(QQuickImageProvider::Pixmap) {}


QPixmap MyImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(requestedSize)
    Q_UNUSED(size)
    QPixmap pixmap;
    if (id.at(0) == 'a') {
        pixmap = QPixmap::fromImage(PiplineBuild::image);
    } else {
        pixmap = QPixmap::fromImage(PiplineBuild::myimage);
    }
    return pixmap;
    // QPixmap p(
    //     "/run/media/root/study/tempWechat/WeChat-Imitate-Client/assets/Picture/avatar/avater1.jpg");
}

void MyImageProvider::setAvater(QPixmap avater)
{
    m_avater=avater;
}

MyImageProvider *MyImageProvider::getInstance()
{
    if(m_myImageProvider == nullptr){
        m_myImageProvider = new MyImageProvider();
    }
    return m_myImageProvider;
}
