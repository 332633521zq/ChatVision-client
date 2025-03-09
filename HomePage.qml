import QtQuick
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects
Rectangle{
    id:homepagerec
    property string communication_name:"";
    property ListModel msglistmodel:rightloader.item.msglistmodel
    Row{
        id:homepagerow
        anchors.fill: parent
        Rectangle{
            id:leftbar
            width: 60
            height: parent.height
            color: "#6BABD1"
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
                            centerloader.source="FindFriendPage.qml"
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
                            settingpopup.open()
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
            color:"#4C82A1"
            Loader{
                id:centerloader
                anchors.fill: parent
                source: "qrc:/ConversationListPage.qml"
            }
        }

    //右边的矩形
        Rectangle{
            id:rightbar
            width: 600
            height: parent.height
            color: "#1D5B7F"
            Loader{
                id:rightloader
                anchors.fill: parent
                source: "CommunicationPage.qml"
                visible: false
            }
        }
    }
    Popup{
        id:settingpopup
        width: 200
        height: 120
        x:leftbar.width+5
        y:homepagerec.height-settingpopup.height-20
        background: Rectangle{
            color: "#368EB0"
            radius: 5
            clip: true
        }
        modal: true
        closePolicy: Popup.CloseOnPressOutside
        onClosed: console.log("popup was closed")
        Rectangle{
            id:poprec
            anchors.fill: parent
            radius: 5
            clip: true
            color:"transparent"
            ListModel{
                id:exitlist
                ListElement{
                    name:"退出"
                }
                ListElement{
                    name:"关闭"
                }
                ListElement{
                    name:"取消"
                }
            }
            Component{
                id:exitcomponent
                Rectangle{
                    id:listrec
                    width: poprec.width
                    height: poprec.height/3
                    radius:5
                    color:"transparent"
                    Text {
                        id: textpoprec
                        text: name
                        anchors.centerIn: parent
                        font.pixelSize: 15
                    }
                    Rectangle{
                        id:masklistrec
                        anchors.fill: parent
                        color:"black"
                        opacity: 0.2
                        visible: false
                    }
                    HoverHandler{
                        onHoveredChanged: {
                            if(hovered){
                                masklistrec.visible=true
                            }
                            else{
                                masklistrec.visible=false
                            }
                        }
                    }

                    TapHandler{
                        onTapped: {
                            if(index===0){
                                console.log("1 was taped")
                                loginloader.source=""
                                loginpage.width=320
                                loginpage.height=420
                                sourceComponent=logincomponent
                            }
                            else if(index===1){
                                console.log("2 was taped")
                                Qt.quit()
                            }
                            else{
                                console.log("3 was taped")
                                settingpopup.close()
                            }
                        }
                    }
                }
            }
            ListView{
                anchors.fill: parent
                model: exitlist
                delegate: exitcomponent
            }
        }
    }

    // StackView{
    //     anchors.fill: parent
    //     id:stackview
    //     initialItem: ""
    // }
}

