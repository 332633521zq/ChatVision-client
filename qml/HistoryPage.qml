import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects


Window{
    id:history_page

    width: 650
    height: 750
    color:"#eee"

    property var friend_nickname : communication_name;
    property var remind_text_pos;

    Column{
        spacing:15
        anchors.horizontalCenter: parent.horizontalCenter

        Label{
            id: title

            width: history_page.width * 0.95
            height: 20
            anchors.horizontalCenter: parent.horizontalCenter
            Text {
                id: friend_name
                text: friend_nickname
                font.pixelSize: 15
                anchors.centerIn: parent
            }
        }

        Rectangle{
            id: search_box

            width: title.width
            height: 30
            radius: 5
            color:"#ddd"
            anchors.horizontalCenter: parent.horizontalCenter

            Row{
                anchors.fill: parent
                spacing:10
                Image{
                    id:search_icon
                    source: "qrc:/image/Search.svg"
                    width: 20
                    height: width
                    anchors.verticalCenter: search_box.verticalCenter
                    x:20
                    opacity: 0.4
                }

                Text {
                    id: search_text
                    text: qsTr("搜索")
                    font.pixelSize: 20
                    opacity: 0.4
                }

            }
            TapHandler{
                onTapped: {
                    calendar_module.visible === true ? calendar_module.visible = false : calendar_module.visible = true;
                }
            }
        }

        Rectangle{
            id:filter

            width: title.width
            height: 30
            color:history_page.color

            Row{}

            Item{
                id:time_filter
                width:time_filter_icon.width+5
                height:parent.height
                anchors.right: parent.right

                Rectangle{
                    id:filter_back
                    anchors.fill: parent
                    color: calendar_module.visible === false ? history_page.color : "#ddd";

                }

                Image{
                    id:time_filter_icon
                    source: "qrc:/image/filter.svg"
                    width:20
                    height:width
                    anchors.verticalCenter: parent.verticalCenter
                }


                CalendarModule{
                     id:calendar_module
                }

                TapHandler{
                    onTapped: {
                        calendar_module.visible === true ? calendar_module.visible = false : calendar_module.visible = true;
                    }
                }
            }
        }

        Rectangle{
            id: msg_content
            width: title.width
            height: history_page.height * 0.85
            color:history_page.color

            ListView{
                id:msg_list
                model:communicationPageControler.history_msgs
                anchors.fill: parent
                clip:true
                delegate: Rectangle{
                    id:single_msg

                    width:msg_content.width
                    height:text_content.height + 50
                    color:history_page.color
                    Row{
                        spacing: 5
                        Rectangle{
                            width:55
                            height: single_msg.height
                            color:history_page.color

                            Rectangle{
                                id: avatar_rec
                                x:15
                                width:40
                                height:width
                                color:history_page.color
                                anchors.horizontalCenter: parent.horizontalCenter
                                Image{
                                    id:avatar_img
                                    source: modelData.sender_id === bips.following_uid ? bips.avatar_path : bips.my_avatar
                                    width:parent.width
                                    anchors.fill: parent
                                    fillMode: Image.PreserveAspectCrop
                                    visible: false
                                    height:width
                                }
                                Rectangle{
                                    id:mas
                                    radius: avatar_img.width
                                    anchors.fill: parent
                                    color:history_page.color
                                    visible: false
                                }
                                OpacityMask{
                                    anchors.fill: mas
                                    source:avatar_img
                                    maskSource: mas
                                }
                            }

                        }
                        Rectangle{
                            id:right_rec
                            width: single_msg.width - avatar_rec.width - 20
                            height:single_msg.height
                            opacity: 0.8
                            color:history_page.color

                            Column{
                                anchors.fill: parent
                                Rectangle{
                                    width:parent.width
                                    height:30
                                    opacity: 0.8
                                    color:history_page.color

                                    Row{
                                        spacing:10
                                        Text{
                                            text:modelData.sender_nickname
                                            font.pixelSize: 18
                                            opacity: 0.8
                                        }
                                        Text{
                                            text:modelData.datetime
                                            font.pixelSize: 18
                                            opacity: 0.8
                                        }
                                    }
                                }
                                Rectangle{
                                    width:parent.width
                                    height:text_content.height
                                    color:history_page.color

                                    Text{
                                        id:text_content
                                        width:parent.width
                                        text:modelData.data
                                        font.pixelSize: 20
                                        wrapMode: Text.Wrap
                                    }
                                }

                            }
                            Rectangle{
                                width: parent.width
                                height:1
                                color:"#eee"
                            }
                        }
                    }
                    Text{
                        id:remind_text
                        text: qsTr("已经到底了哦～")
                        visible: false
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                    }
                    Component.onCompleted: {
                        if(index === ListView.view.count - 1){
                            remind_text.visible = true;
                        }
                    }
                }
            }

            Text{
                id:rt
                text: qsTr("已经到底了哦～")
                visible: msg_list.count === 0
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
