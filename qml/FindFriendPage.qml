import QtQuick
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects
//找新朋友的列表
Rectangle{
    id:findfriendbar
    width:240
    height: parent.height
    color:"transparent"
    ListModel{
        id:findfriendlist
        // ListElement{
        //     name:"黄昏时风起"
        //     avater:"qrc:/image/avater.jpg"
        //     friendId:"20000001"
        //     memo:""
        //     signature:""
        //     area:""
        //     gender:""
        // }
        // ListElement{
        //     name:"坐看云起时"
        //     avater:"qrc:/image/avater.png"
        //     friendId:"20000000"
        // }
    }
    Component{
        id:findfriendcomponent
        Rectangle{
            id:findfriendrec
            width: findfriendbar.width
            height: findfriendbar.width/7*2
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
                    text: memo
                }
            }
            Rectangle{
                id:findfriendmask
                width: parent.width
                height: parent.height
                color:"black"
                opacity: 0.1
                visible: false
            }

            HoverHandler{
                onHoveredChanged: {
                    if(hovered){
                        findfriendmask.visible=true
                    }
                    else{
                        findfriendmask.visible=false
                    }
                }
            }
            TapHandler{
                onTapped: {
                    rightloader.visible=true
                    rightloader.source="qrc:/qml/Main.qml"
                    bips.nickname=nickname
                    bips.memo=memo
                    bips.area=area
                    bips.following_uid=uid
                    bips.signal_text=signature
                    bips.avatar_path=avater
                    bips.relation=relation
                    // rightloader.sourceComponent=findfriendInfoComponent

                }
            }
        }
    }
    ListView{
        anchors.fill: parent
        model: findfriendlist
        delegate: findfriendcomponent
        clip: true
    }
    Connections{
        target: findfriendPageController
        function onInitFindPage(area, gender, memo, nickname, signature, uid,avatar,relation){
            var addlist={};
            addlist.area=area;
            addlist.avater=avatar;
            addlist.gender=gender;
            addlist.memo=memo;
            addlist.nickname=nickname;
            addlist.signature=signature;
            addlist.uid=uid;
            addlist.relation=relation;
            findfriendlist.append(addlist);
        }
    }
    Component{
        id:findfriendInfoComponent
        FollowUserInfoPage{
            id:findInfoPage
        }
    }

}
