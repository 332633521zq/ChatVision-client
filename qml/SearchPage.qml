import QtQuick
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects
Rectangle{
    property ListModel searchmodel:searchListModel
    id:searchbar
    anchors.fill: parent

    ListModel{
        id:searchListModel
    }
    // ListModel{
    //     id:netSearchListModel
    // }


    Component{
        id:searchComponet
        Rectangle{
            width: searchbar.width
            height: searchbar.width/8*2
            color: "transparent"
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
                    text:memo
                }
            }
            Rectangle{
                id:searchrecmask
                width: parent.width
                height: parent.height
                color:"black"
                opacity: 0.1
                visible: false
            }

            HoverHandler{
                onHoveredChanged: {
                    if(hovered){
                        searchrecmask.visible=true
                    }
                    else{
                        searchrecmask.visible=false
                    }
                }
            }
            TapHandler{
                onTapped: {
                    if(!isNetSearch){
                        rightloader.visible=true
                        rightloader.source="qrc:/qml/CommunicationPage.qml"
                        console.log("conversation was clicked")
                        if(msglistmodel===null){
                        }else{
                            msglistmodel.clear()
                        }
                        // console.log("msglistmodel cleared. Current count:", msglistmodel.count);
                        communicationPageControler.friendId=obid
                        communication_name=memo
                        communicationPageControler.initCommunicationPage()
                    }
                    else{
                        isNetSearch=false;
                        console.log("this is net search");
                        rightloader.visible=true
                        rightloader.source="qrc:/qml/Main.qml"
                        bips.nickname=nickname
                        bips.memo=memo
                        bips.area=area
                        bips.signal_text=signature
                    }
                }
            }
        }
    }

    ListView{
        id:localsearchListView
        anchors.fill:parent
        model: searchListModel
        delegate: searchComponet
        clip: true
        visible: true
    }
    // ListView{
    //     id:netSearchListView
    //     anchors.fill: parent
    //     model:netSearchListModel
    //     delegate:
    // }

}
