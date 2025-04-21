import QtQuick
import Qt5Compat.GraphicalEffects


Rectangle{
    id:fuipage

    width:parent.width
    height:parent.height

    signal loadchatpage()
    signal currentuid(var uid)
    signal loadchatlistpage()

    Column{
        anchors.horizontalCenter: parent.horizontalCenter

        Rectangle{
            id:up_rec
            width:fuipage.width * 0.9
            height:fuipage.height * 0.27
            // color:"pink"
            Rectangle {
                width: parent.width
                height: 1
                color: "#ccc"
                anchors.bottom: parent.bottom
            }
            Row{
                spacing: 10
                Rectangle{
                    id:avaterrec
                    width: 80
                    height: width
                    // color:"orange"
                    y:parent.height*0.7
                    Image{
                        id:avaterimage
                        anchors.fill: parent
                        source:bips.avatar_path
                        fillMode: Image.PreserveAspectCrop
                        sourceSize: Qt.size(parent.width,parent.height)
                        visible: false
                    }
                    Rectangle{
                        id:maskrec
                        radius: avaterrec.width * 0.5
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
                    id: idrec

                    width: up_rec.width-avaterrec.width-10
                    height:avaterrec.height - 20
                    color: "#928"
                    anchors.verticalCenter: avaterrec.verticalCenter

                    Rectangle{
                        width:idrec.width
                        height:avaterrec.height
                        anchors.verticalCenter: parent.verticalCenter
                        Column{
                            spacing:5
                            Text{
                                text:bips.nickname
                                font.pixelSize: 22
                            }
                            Text{
                                text:"uid:"+bips.following_uid
                                font.pixelSize: 12
                            }
                            Text{
                                text:"OnlineState:" + isonline
                                font.pixelSize: 12
                            }
                        }
                    }
                }
            }
        }


        Rectangle{
            id:center_rec

            width: up_rec.width
            height:(fuipage.height - up_rec.height)*0.45
            // color:"gray"

            Column{

                spacing:15
                Rectangle{
                    id:baseinfo_rec
                    y:15
                    width: center_rec.width
                    height: 30
                    Row{
                        spacing:15
                        anchors.verticalCenter: parent.verticalCenter

                        Text{
                            text:"性别："+bips.gender
                            x:15
                            font.pixelSize: 16
                            color:"gray"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Rectangle {
                            width: 1
                            height: parent.height
                            color: "#ccc"
                            // anchors.bottom: parent.bottom
                        }
                        Text{
                            text:"现居："+bips.area
                            font.pixelSize: 16
                            color:"gray"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }
                Rectangle{
                    id:sig_rec

                    width: center_rec.width
                    height:40
                    // color: "#91e"
                    Rectangle{
                        id:sig_img
                        width: 20
                        height: width
                        // color:"orange"
                        anchors.verticalCenter: parent.verticalCenter
                        Image{
                            id:sig_icon
                            anchors.fill: parent
                            sourceSize: Qt.size(parent.width,parent.height)
                            source:"qrc:/image/edit_24.svg"
                        }
                    }
                    Text{
                        text:"个性签名"
                        font.pixelSize: 17
                        x: sig_img.width + 5
                        anchors.verticalCenter: parent.verticalCenter

                    }
                    Text{
                        text:bips.signal_text
                        font.pixelSize: 17
                        anchors.verticalCenter: parent.verticalCenter
                        // horizontalAlignment: Text.AlignRight
                        anchors.right: parent.right
                    }
                }
                Rectangle{
                    id:memo_rec

                    width: center_rec.width
                    height:40
                    // color: "#91e"
                    Rectangle{
                        id:memo_img
                        width: 20
                        height: width
                        // color:"orange"
                        anchors.verticalCenter: parent.verticalCenter
                        Image{
                            id:memo_icon
                            anchors.fill: parent
                            sourceSize: Qt.size(parent.width,parent.height)
                            source:"qrc:/image/remark_24.svg"
                        }
                    }
                    Text{
                        text:"备注"
                        font.pixelSize: 17
                        x: sig_img.width + 5
                        anchors.verticalCenter: parent.verticalCenter

                    }
                    Text{
                        text:bips.memo
                        font.pixelSize: 17
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                    }
                }
            }
            Rectangle {
                width: parent.width
                height: 1
                color: "#ccc"
                anchors.bottom: parent.bottom
            }
        }
        Rectangle{
            id:bt_rec

            width: up_rec.width
            height:(fuipage.height - up_rec.height)*0.55
            // color:"red"
            Row{
                anchors.fill: parent
                spacing: 20
                Rectangle{
                    id:space
                    width:120
                    height:40
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 10
                    // color:"#ccc"
                }
                Rectangle{
                    id:cancel_focus
                    width:120
                    height:40
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 10
                    color:"#ccc"
                    Text{
                        text:"取关"
                        font.pixelSize: 17
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    TapHandler{
                        onTapped: {
                            var uid = bips.following_uid;
                            console.log(uid)
                            followingPageController.cancelFocus(uid);

                        }
                    }
                }


                Rectangle{
                    id:chat_botton
                    width:120
                    height:40
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 10
                    color:"#ccc"
                    Text{
                        text:"发消息"
                        font.pixelSize: 17
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    TapHandler{
                        onTapped: {
                            var uid = bips.following_uid;
                            currentuid(uid);
                            loadchatlistpage();
                            loadchatpage();
                            communicationPageControler.friendId=uid
                            communication_name=bips.memo
                            if(msglistmodel===null){
                            }else{
                                msglistmodel.clear()
                            }
                            communicationPageControler.initCommunicationPage()
                            communicationPageControler.addListElement(bips.following_uid, bips.memo, bips.nickname, bips.area, bips.gender, bips.signal_text, bips.avatar_path)
                        }
                    }
                }
            }
        }
    }

}
