import QtQuick
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects
//右边的矩形
Rectangle{
    property ListModel msglistmodel:msglistmodel
    id:rightbar
    width: 600
    height: parent.height
    // color: "#FFFFFF"

    color: "#F2F2F2"
    //后面应该分开成一个组建加载
    Rectangle{
        id:chatrec
        anchors.fill: parent
        color: "transparent"
        Column{
            id:chatcolumn
            anchors.fill: parent

            Rectangle{
                id:showNameRec
                width: parent.width
                height: parent.height/10
                color: "transparent"
                Text {
                    id: friendnametext
                    text:communication_name
                    anchors.centerIn: parent
                    font.pixelSize: 20
                }
            }

            Rectangle{
                id:communicationListRec
                width: parent.width
                height:parent.height/5*3
                color: "transparent"
                border.color: "#E6E6E7"
                ListModel{
                    id:msglistmodel
                    ListElement{
                        textmsg:"你好木薯"
                        sender:"20000000"
                    }
                    ListElement{
                        textmsg:"你好一一"
                        sender:"20000001"
                    }
                }
                Component{
                    id:msgcomponent
                    Rectangle{
                        id:msgrec
                        width: 600
                        // height: 40
                        height:Math.max(text_msg.implicitHeight+10,40)
                        color:"transparent"
                        Rectangle{
                            id:msgavaterRec
                            width: 40
                            height: 40
                            color:"transparent"
                            Image{
                                id:msg_avater
                                source:"qrc:/image/avater.jpg"
                                sourceSize: Qt.size(parent.width,parent.height)
                                visible: false
                            }
                            Rectangle{
                                id:mask_rec
                                anchors.fill: parent
                                radius: 20
                                visible: false
                            }
                            OpacityMask{
                                anchors.fill: mask_rec
                                source:msg_avater
                                maskSource: mask_rec
                            }
                        }
                        Rectangle{
                            id:textmsgrec
                            width: Math.min(text_msg.implicitWidth,400)
                            height:text_msg.implicitHeight
                            anchors.verticalCenter: parent.verticalCenter
                            color: "#5EB6E8"
                            radius: 5
                            Text {
                                id: text_msg
                                width: parent.width
                                anchors.centerIn: parent.Center
                                text: textmsg
                                padding: 5
                                wrapMode: Text.Wrap
                                font.pixelSize: 15
                            }
                        }
                        Component.onCompleted: {
                            if(sender === communicationPageControler.myId){
                                msgavaterRec.anchors.right=msgrec.right
                                msgavaterRec.anchors.rightMargin=25
                                textmsgrec.anchors.rightMargin=10
                                textmsgrec.anchors.right=msgavaterRec.left
                            }
                            else{
                                msgavaterRec.anchors.left=msgrec.left
                                msgavaterRec.anchors.leftMargin=25
                                textmsgrec.anchors.leftMargin=10
                                textmsgrec.anchors.left=msgavaterRec.right
                                textmsgrec.color="#FFFFFF"
                            }
                        }
                    }
                }
                ListView{
                    id:communicationlistview
                    anchors.fill: parent
                    model: msglistmodel
                    delegate: msgcomponent
                    clip: true
                    spacing: 15
                }
            }

            Rectangle{
                id:textEditRec
                width:parent.width
                height:parent.height/10*3
                color: "transparent"
                Column{
                    id:texteditcolumn
                    anchors.fill: parent
                    Rectangle{
                        id:toolrec
                        width:parent.width
                        height: parent.height/5
                        color: "transparent"
                        Row{
                            anchors.fill: parent
                            padding: 5
                            spacing: 20
                            Rectangle{
                                width: 30
                                height: 30
                                color:"transparent"
                                Image {
                                    anchors.centerIn: parent
                                    source: "qrc:/image/Photo.svg"
                                    sourceSize: Qt.size(parent.width-10,parent.height-10)
                                }
                                Rectangle{
                                    id:mk
                                    anchors.fill: parent
                                    color:"black"
                                    opacity: 0.2
                                    visible: false
                                    radius: 5
                                }
                                HoverHandler{
                                    onHoveredChanged: {
                                        if(hovered){
                                            mk.visible=true
                                        }
                                        else{
                                            mk.visible=false
                                        }
                                    }
                                }
                            }
                            Rectangle{
                                width: 30
                                height: 30
                                color:"transparent"
                                Image {
                                    anchors.centerIn: parent
                                    source: "qrc:/image/transmission_file_24.svg"
                                    sourceSize: Qt.size(parent.width-10,parent.height-10)
                                }
                                Rectangle{
                                    id:mk3
                                    anchors.fill: parent
                                    color:"black"
                                    opacity: 0.2
                                    visible: false
                                    radius: 5
                                }
                                HoverHandler{
                                    onHoveredChanged: {
                                        if(hovered){
                                            mk3.visible=true
                                        }
                                        else{
                                            mk3.visible=false
                                        }
                                    }
                                }
                            }
                            Rectangle{
                                width: 30
                                height: 30
                                color:"transparent"
                                Image {
                                    anchors.centerIn: parent
                                    source: "qrc:/image/Phone.svg"
                                    sourceSize: Qt.size(parent.width-10,parent.height-10)
                                }
                                Rectangle{
                                    id:mk2
                                    anchors.fill: parent
                                    color:"black"
                                    radius: 5
                                    opacity: 0.2
                                    visible: false
                                }
                                HoverHandler{
                                    onHoveredChanged: {
                                        if(hovered){
                                            mk2.visible=true
                                        }
                                        else{
                                            mk2.visible=false
                                        }
                                    }
                                }
                                TapHandler{
                                    onTapped: {
                                        communicationPageControler.callRequest();
                                        mediawindow.show()
                                        requestCallRec.visible=true
                                        mediaplayer.play()
                                    }
                                }
                            }
                        }
                    }

                    Rectangle{
                        id:msginputrec
                        width:parent.width
                        height: parent.height/5*3
                        color: "transparent"
                        TextArea{
                            id:msginput
                            anchors.fill: parent
                            wrapMode: TextArea.Wrap
                            color: "black"
                        }
                    }
                    Rectangle{
                        id:sendbuttonrec
                        width: parent.width
                        height: parent.height/5
                        color: "transparent"
                        Button{
                            id:sendbutton
                            width: 70
                            height: 25
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            background: Rectangle{
                                id:buttonrec
                                anchors.fill: parent
                                color:"#3399FF"
                                radius: 3
                            }
                            onPressed: {
                                buttonrec.color="#66B2FF"
                            }
                            onReleased: {
                                buttonrec.color="#3399FF"
                            }
                            onHoveredChanged: {
                                if(hovered){
                                    buttonrec.color="#61A9D2"
                                }
                                else{
                                    buttonrec.color="#3399FF"
                                }
                            }
                            onClicked: {
                                communicationPageControler.myMessage=msginput.text
                                communicationPageControler.sendMessage(msginput.text);
                                msginput.clear()
                                //调用发送消息函数将消息发送给对方
                                communicationPageControler.saveMessage();
                            }

                            Text{
                                id:sendbuttontext
                                text:qsTr("发 送")
                                font.pixelSize: 15
                                anchors.centerIn: parent
                            }
                        }
                    }
                }
            }
        }
        Component.onCompleted: {
            console.log("this is on component")
        }
        Connections{
            target:communicationPageControler
            function onMyMessageChanged(){
                var msghistory={}
                msghistory.sender=communicationPageControler.myId
                msghistory.textmsg=communicationPageControler.myMessage
                msglistmodel.append(msghistory)
                communicationlistview.positionViewAtEnd()
            }
        }
        Connections{
            target: communicationPageControler
            function onFriendMessageChanged(){
                var msghistory={}
                msghistory.sender=communicationPageControler.friendId
                msghistory.textmsg=communicationPageControler.friendMessage
                msglistmodel.append(msghistory)
                communicationlistview.positionViewAtEnd()

            }
        }
    }
}
