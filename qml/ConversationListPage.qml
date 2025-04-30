import QtQuick
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects
//中间的会话列表
Rectangle{
    id:centerbar
    width:240
    height: parent.height
    color:"transparent"

    Component{
        id:conversationcomponent
        Rectangle{
            id:conversationrec
            width: centerbar.width
            height: centerbar.width/7*2
            color:"transparent"
            // border.color: "black"
            Row{
                leftPadding: 15
                anchors.verticalCenter: parent.verticalCenter
                Rectangle{
                    width: 40
                    height: 40
                    color:"transparent"
                    Image{
                        id:listavaterimage
                        source:avatar_path
                        sourceSize: Qt.size(parent.width,parent.height)
                        visible: false
                    }
                    Rectangle{
                        id:mask
                        anchors.fill: parent
                        radius: 20
                        visible: false
                    }
                    OpacityMask{
                        anchors.fill: mask
                        source:listavaterimage
                        maskSource: mask
                    }
                }

                Text {
                    id: nametext
                    text: memo
                }
            }
            Rectangle{
                id:unread_rec
                width: 16
                height: 16
                anchors.right:parent.right
                anchors.margins: 10
                anchors.bottom: parent.bottom
                radius:8
                color: "#DC0103"
                visible: unread_count !== 0
                Text {
                    id: unread_number
                    text: unread_count
                    font.pixelSize: 10
                    color: "white"
                    anchors.centerIn: parent
                }

            }
            Rectangle{
                id:conversationrecmask
                width: parent.width
                height: parent.height
                color:"black"
                opacity: 0.1
                visible: false
            }

            HoverHandler{
                onHoveredChanged: {
                    if(hovered){
                        conversationrecmask.visible=true
                    }
                    else{
                        conversationrecmask.visible=false
                    }
                }
            }
            TapHandler{
                onTapped: {
                    conversationlistview.currentIndex=index
                    rightloader.visible=true
                    rightloader.source="qrc:/qml/CommunicationPage.qml"
                    console.log("conversation was clicked")
                    if(msglistmodel===null){
                    }else{
                        msglistmodel.clear()
                    }
                    // console.log("msglistmodel cleared. Current count:", msglistmodel.count);
                    communicationPageControler.friendId=obid
                    bips.avatar_path=avatar_path
                    communication_name=memo
                    communicationPageControler.initCommunicationPage()
                    unread_count=0
                }
            }
        }
    }
    ListView{
        id:conversationlistview
        anchors.fill: parent
        model: conversationlist
        delegate: conversationcomponent
        clip: true
        highlight: Rectangle {
            color: "#5EB6E8"
        }
        highlightFollowsCurrentItem: true
    }

    Connections{
        target: communicationPageControler
        function onUnreadChanged(index){
            var item=conversationlist.get(index)
            var count=item.unread_count+1
            conversationlist.set(index,{"unread_count":count})
        }
    }

}
