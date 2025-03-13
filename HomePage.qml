import QtQuick
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects
Rectangle{
    id:homepagerec
    // property alias rightloader: rightloader
    property string communication_name:"";
    property bool isfollowlistloadeed:false;

    BaseInfoProperties{
        id:bips
    }

    Connections{
        target:centerloader.item
        function onLoadfollowinginfo() {
            rightloader.source = "qrc:/FollowUserInfoPage.qml";
        }
    }

    Connections{
        target: rightloader.item
        function onLoadchatpage(){
            rightloader.source = "CommunicationPage.qml";
        }
    }

    Row{
        id:homepagerow
        anchors.fill: parent
        Rectangle{
            id:leftbar
            width: 60
            height: parent.height
            color: "#74E6C4"
            Column{
                id:leftbarcolumn
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 25
                topPadding: 25

                Rectangle{
                    id:avaterrec
                    width: 40
                    height: 40
                    color:"transparent"
                    Image{
                        id:avaterimage
                        anchors.fill: parent
                        source:"qrc:/image/bg2.jpg"
                        fillMode: Image.PreserveAspectCrop
                        visible: false
                    }
                    Rectangle{
                        id:maskrec
                        radius: 30
                        anchors.fill: parent
                        visible: false
                    }
                    OpacityMask{
                        anchors.fill: maskrec
                        source:avaterimage
                        maskSource: maskrec
                    }
                    TapHandler{
                        id:avaterth
                        onTapped: {
                            var myinfo = followingPageController.myinfo
                            bips.following_uid = myinfo.uid/* "2000000"*/
                            bips.nickname = myinfo.nickname /*"85"*/
                            bips.memo = myinfo.nickname /*"85"*/
                            bips.signal_text = myinfo.signature/*"罪业的报偿"*/
                            bips.area = myinfo.area/*"中国大陆 重庆"*/
                            // bips.avatar_path = myinfo.avatar_path/*"../assets/Picture/avatar/cats.jpg"*/
                            bips.gender = myinfo.gender

                            if(isfollowlistloadeed == true){
                                console.log("tapped ")
                                rightloader.source = "qrc:/FollowUserInfoPage.qml";
                            }
                        }
                    }
                }

                Rectangle{
                    id:findrec
                    width:40
                    height:40
                    color: "transparent"
                    Image{
                        id:findimage
                        width: 25
                        height:25
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        source:"qrc:/image/Anonymous.svg"
                        fillMode: Image.PreserveAspectCrop
                    }
                    Rectangle{
                        id:maskfind
                        anchors.fill: parent
                        color: "black"
                        opacity: 0.2
                        visible: false
                        radius: 5
                    }
                    HoverHandler{
                        onHoveredChanged: {
                            if(hovered){
                                maskfind.visible=true
                            }
                            else{
                                maskfind.visible=false
                            }
                        }
                    }
                    TapHandler{
                        onTapped: {
                            isfollowlistloadeed == false
                        }
                    }
                }

                Rectangle{
                    id:focusrec
                    width:40
                    height:40
                    color: "transparent"
                    Image{
                        width:25
                        height:25
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:/image/Essence.svg"
                    }
                    Rectangle{
                        id:maskfocus
                        anchors.fill: parent
                        color: "black"
                        opacity: 0.2
                        visible: false
                        radius: 5
                    }
                    HoverHandler{
                        onHoveredChanged: {
                            if(hovered){
                                maskfocus.visible=true
                            }
                            else{
                                maskfocus.visible=false
                            }
                        }
                    }
                    TapHandler{
                        onTapped: {
                            centerloader.source = "qrc:/FollowListPage.qml"
                            isfollowlistloadeed = true;
                            rightloader.source = ""
                        }
                    }
                }

                Rectangle{
                    id:minerec
                    width:40
                    height:40
                    color: "transparent"
                    Image{
                        id:mineimage
                        width:25
                        height:25
                        source:"qrc:/image/administering_user_16"
                        fillMode: Image.PreserveAspectCrop
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Rectangle{
                        id:maskmine
                        anchors.fill: parent
                        color: "black"
                        opacity: 0.2
                        visible: false
                        radius: 5
                    }
                    HoverHandler{
                        onHoveredChanged: {
                            if(hovered){
                                maskmine.visible=true
                            }
                            else{
                                maskmine.visible=false
                            }
                        }
                    }
                    TapHandler{
                        onTapped: {
                            isfollowlistloadeed == false
                        }
                    }
                }

                Rectangle{
                    id:space
                    width: parent.width
                    height: 210
                    color:"transparent"
                }
                Rectangle{
                    id:settingrec
                    width:40
                    height:40
                    color:"transparent"
                    Image{
                        id:settingimage
                        width:25
                        height:25
                        source:"qrc:/image/Setting.svg"
                        fillMode: Image.PreserveAspectCrop
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Rectangle{
                        id:masksetting
                        anchors.fill: parent
                        color: "black"
                        opacity: 0.2
                        visible: false
                        radius: 5
                    }
                    HoverHandler{
                        onHoveredChanged: {
                            if(hovered){
                                masksetting.visible=true
                            }
                            else{
                                masksetting.visible=false
                            }
                        }
                    }
                    TapHandler{
                        onTapped: {
                            isfollowlistloadeed == false
                        }
                    }
                }
            }
        }

       //中间的会话列表
        Rectangle{
            id:centerbar
            width:240
            height: parent.height
            color:"#74E6C4"
            Loader{
                anchors.fill: parent
                id:centerloader
                source: "qrc:/ConversationListPage.qml"
            }
        }

    //右边的矩形
        Rectangle{
            id:rightbar
            width: 600
            height: parent.height
            color: "#74E6C4"
            Loader{
                anchors.fill: parent
                id:rightloader
                source: ""
                // visible: false
            }
        }
    }


    // StackView{
    //     anchors.fill: parent
    //     id:stackview
    //     initialItem: ""
    // }
}

