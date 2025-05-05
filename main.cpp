#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "client.h"
#include "communicationpagecontroller.h"
#include "findfriendpagecontroller.h"
#include "followlistpagecontroller.h"
#include "myimageprovider.h"
#include "personalpagecontroller.h"
#include "searchcontroller.h"
// #include "ConstValue.h"
// #include "mediathread.h"
// #include <boost/asio.hpp>
// #include <iostream>
// #include <nlohmann/json.hpp>
// #include <thread>

using namespace boost::asio::ip;

// #define IPADDRESS "192.168.253.186"
// #define IPADDRESS "127.0.0.1"
// #define PORT 10086

int main(int argc, char* argv[])
{
    Client::getInstance();

    QGuiApplication app(argc, argv);

    gst_init(&argc, &argv);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("followingPageController",
                                             &FollowListPageController::getInstance());
    engine.rootContext()->setContextProperty("personalPageControler",
                                             &PersonalPageController::getInstance());
    engine.rootContext()->setContextProperty("communicationPageControler",
                                             &CommunicationPageController::getInstance());

    engine.rootContext()->setContextProperty("findfriendPageController",
                                             &FindFriendPageController::getInstance());

    engine.rootContext()->setContextProperty("searchController", &SearchController::getInstance());

    MyImageProvider *myImageProvider = MyImageProvider::getInstance();
    engine.addImageProvider("pictures", myImageProvider);

    const QUrl url(QStringLiteral("qrc:/qml/LoginPage.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
