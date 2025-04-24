import QtQuick
import Qt5Compat.GraphicalEffects

Rectangle{
    id: followlist_page
    // anchors.fill: parent
    width: 240
    height:parent.height
    property var selection:"关注"
    property var homepage
    signal loadfollowinginfo()

    Rectangle {
        id: select_part

        z: 0
        width: parent.width * 0.9
        height: 40

        y: parent.width * 0.05
        color: "#ccc"  // 底层灰色背景
        anchors.horizontalCenter: parent.horizontalCenter

        radius: 10  // 圆角

        // 白色滑块
        Rectangle {
            id: highlight_selected

            width: parent.width * 0.425
            height: parent.height * 0.8
            x:parent.width * 0.04
            anchors.verticalCenter: parent.verticalCenter
            color: "#fff"
            radius: select_part.radius
            z: 1

            // 滑块的平移效果
            transform: Translate {
                id: translate
                x: 0
            }

            // 定义动画
            PropertyAnimation {
                id: highlight_fans
                target: translate
                properties: "x"
                to: select_part.width * 0.5
                duration: 500
            }

            PropertyAnimation {
                id: highlight_follow
                target: translate
                properties: "x"
                to: 0
                duration: 500
            }
        }

        // 左侧文字区域
        Rectangle {
            id: follow_text_rec

            width: parent.width / 2
            height: parent.height
            anchors.left: parent.left
            color: "transparent"  // 背景透明
            z: 2

            Text {
                text: "关注"
                anchors.centerIn: parent
                font.pixelSize: 14  // 字体大小
                color: translate.x === 0 ? "#000" : "#888"  // 滑块在左侧时文字为黑色，否则为灰色
            }

            // 点击事件
            TapHandler {
                onTapped: {
                    selection="关注";
                    if (translate.x !== 0) {
                        highlight_follow.start();  // 滑动到左侧
                        user_list.model = followingPageController.followings;  // 更新模型
                    }
                }
            }
        }

        // 右侧文字区域
        Rectangle {
            id: fans_text_rec

            width: parent.width / 2
            height: parent.height
            anchors.right: parent.right
            color: "transparent"
            z: 2

            Text {
                text: "粉丝"
                anchors.centerIn: parent
                font.pixelSize: 14
                color: translate.x !== 0 ? "#000" : "#888"
            }

            // 点击事件
            TapHandler {
                onTapped: {
                    selection="粉丝";
                    if (translate.x === 0) {
                        highlight_fans.start();
                        user_list.model = followingPageController.followers;
                    }
                }
            }
        }
    }

    Rectangle{
        id: user_list_rec
        width: parent.width
        height:parent.height * 0.95 - select_part.height
        anchors.horizontalCenter: parent.horizontalCenter
        y:select_part.height + parent.height * 0.03
        // color:"#060"
        opacity: 0.8
        ListView{
            id:user_list
            anchors.fill: parent
            clip:true
            model: followingPageController.followings
            delegate: Rectangle{
                id: model_rec

                width: user_list_rec.width
                height:60
                // color:"purple"
                Rectangle {
                    width: parent.width
                    height: 1
                    color: "#ccc"
                    anchors.bottom: parent.bottom
                }

                Row{
                    x:5
                    spacing:5
                    anchors.verticalCenter: parent.verticalCenter
                    Rectangle{
                        id:avaterrec
                        width: model_rec.width * 0.2
                        height: width
                        anchors.verticalCenter: parent.verticalCenter
                        Image{
                            id:avaterimage
                            anchors.fill: parent
                            source:modelData.avatar_path_
                            fillMode: Image.PreserveAspectCrop
                            sourceSize: Qt.size(parent.width,parent.height)
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
                        id:nickname_rec

                        width:user_list_rec.width*0.95 - avaterrec.width
                        height:avaterrec.height
                        anchors.verticalCenter: parent.verticalCenter

                        // {"area":"重庆","avatar_path_":"path","gender":"女","memo":"85","nickname":"85","signature":"罪恶没有假期，正义便无暇休憩","uid":"20000000"}

                        Column{
                            spacing:5
                            Text {
                                text: modelData.nickname // 显示昵称
                                font.pixelSize: 20
                            }
                            Text {
                                text: modelData.signature // 个性签名
                                font.pixelSize: 12
                            }
                        }


                    }
                }

                TapHandler{
                    onTapped: {
                        rightloader.visible=true
                        bips.following_uid = modelData.uid/* "2000000"*/
                        bips.nickname = modelData.nickname /*"85"*/
                        bips.memo = modelData.nickname /*"85"*/
                        bips.signal_text = modelData.signature/*"罪业的报偿"*/
                        bips.area = modelData.area/*"中国大陆 重庆"*/
                        bips.avatar_path = modelData.avatar_path_/*"../assets/Picture/avatar/cats.jpg"*/
                        bips.gender = modelData.gender

                        followingPageController.getOnlineState(modelData.uid)
                        if(selection==="关注"){
                            loadfollowinginfo();
                        }
                        else{
                            rightloader.source="qrc:/qml/Main.qml"
                        }
                    }
                }
            }
        }

    }
}
