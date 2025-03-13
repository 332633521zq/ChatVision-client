import QtQuick
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects
//中间的会话列表
Rectangle{
    id:centerbar
    width:240
    height: parent.height
    color:"transparent"
    ListModel{
        id:conversationlist
        ListElement{
            name:"沐树01"
            avater:"qrc:/image/avater.jpg"
            friendId:"20000001"
        }
        ListElement{
            name:"亦已00"
            avater:"qrc:/image/avater.png"
            friendId:"20000000"
        }
        ListElement{
            name:"幻蓝02"
            avater:"qrc:/image/avater.png"
            friendId:"20000002"
        }
    }
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
                        source:avater
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
                    text: name
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
                    rightloader.visible=true
                    console.log("conversation was clicked")
                    msglistmodel.clear()
                    console.log("msglistmodel cleared. Current count:", msglistmodel.count);
                    communicationPageControler.friendId=friendId
                    communication_name=name
                    communicationPageControler.initCommunicationPage()
                }
            }
        }
    }
    ListView{
        anchors.fill: parent
        model: conversationlist
        delegate: conversationcomponent
    }

}
