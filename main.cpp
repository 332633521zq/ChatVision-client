#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "client.h"
#include "communicationpagecontroller.h"
#include "findfriendpagecontroller.h"
#include "myimageprovider.h"
#include "personalpagecontroller.h"
// #include "ConstValue.h"
// #include "mediathread.h"
// #include <boost/asio.hpp>
// #include <iostream>
// #include <nlohmann/json.hpp>
// #include <thread>

// // const int MAX_LENGTH = 1024 * 2;
// const int HEAD_LENGTH = 2;
// const int HEAD_TOTAL = 4;

// // unsigned int uid = 20000001;
// extern unsigned int uid;
// unsigned int object_id = 20000001;

// void SendRequest(tcp::socket& sock, char* data, unsigned int object_id, unsigned int msg_id);
// void SendRequest(tcp::socket& sock, std::string data, unsigned int object_id, unsigned int msg_id);

using namespace boost::asio::ip;

// #define IPADDRESS "192.168.253.186"
#define IPADDRESS "127.0.0.1"
#define PORT 10086

int main(int argc, char* argv[])
{
    Client client;

    QGuiApplication app(argc, argv);

    gst_init(&argc, &argv);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("personalPageControler",
                                             &PersonalPageController::getInstance());
    engine.rootContext()->setContextProperty("communicationPageControler",
                                             &CommunicationPageController::getInstance());

    engine.rootContext()->setContextProperty("findfriendPageController",
                                             &FindFriendPageController::getInstance());

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
