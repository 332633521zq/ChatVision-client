import QtQuick
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects
import QtMultimedia
Rectangle{
    id:homepagerec
    property string communication_name:"";
    property bool isfollowlistloadeed:false;

    property ListModel msglistmodel:rightloader.item.msglistmodel
    property real randomNumber:0

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
                    id:conversationrec
                    width:40
                    height:40
                    color: "transparent"
                    Image{
                        id:conversationrecimage
                        width: 25
                        height:25
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        source:"qrc:/image/message_24.svg"
                        fillMode: Image.PreserveAspectCrop
                    }
                    Rectangle{
                        id:maskconversation
                        anchors.fill: parent
                        color: "black"
                        opacity: 0.2
                        visible: false
                        radius: 5
                    }
                    HoverHandler{
                        onHoveredChanged: {
                            if(hovered){
                                maskconversation.visible=true
                            }
                            else{
                                maskconversation.visible=false
                            }
                        }
                    }
                    TapHandler{
                        onTapped: {
                            centerloader.source="qrc:/qml/ConversationListPage.qml"
                            isfollowlistloadeed == false

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
                            centerloader.source="qrc:/qml/FindFriendPage.qml"
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
                        source: "qrc:/image/channel_selection_16.svg"
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
                        source:"qrc:/image/administering_user_24.svg"
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
                    height: 160
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
                        source:"qrc:/image/setting_24.svg"
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
                id:centerloader
                anchors.fill: parent
                source: "qrc:/qml/ConversationListPage.qml"
            }
        }

    //右边的矩形
        Rectangle{
            id:rightbar
            width: 600
            height: parent.height
            color: "#FFFFFF"
            Loader{
                id:rightloader
                anchors.fill: parent
                source: "qrc:/qml/CommunicationPage.qml"
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

    Window{
        id:mediawindow
        width: 600
        height: 500
        color:"#368EB0"
        MediaPlayer{
            id:mediaplayer
            source: "qrc:/audio/callaudio.mp3"
            audioOutput: AudioOutput{}
        }
        Image {
            id: vedio
            anchors.fill: parent
            source: "image://pictures/avater"
            cache: false
        }
        Timer{
            id:t1
            interval: 1
            running: true
            repeat: true
            onTriggered: {
                randomNumber++
                vedio.source="image://pictures/"+randomNumber
            }
        }
        Rectangle{
            id:answerRequestRec
            anchors.fill: parent
            visible: false
            color:"transparent"
            Text{
                id:getcalltext
                text:qsTr("请求通话")
                font.pixelSize: 20
                anchors.centerIn: parent
            }

            Button{
                id:get_through_Button
                width: 100
                height: 30
                x:(parent.width-250)/2
                y: (parent.height-closebutton.height)-30
                Text{
                    text:qsTr("接通")
                    anchors.centerIn: parent
                }
                onClicked: {
                    //加载视频通话画面
                    communicationPageControler.getThrough()
                    answerRequestRec.visible=false
                    onThePhoneRec.visible=true
                }
            }
            Button{
                id:hang_up_Button
                width: 100
                height: 30
                x:(parent.width-250)/2+125
                y: (parent.height-closebutton.height)-30
                Text{
                    text:qsTr("挂断")
                    anchors.centerIn: parent

                }
                onClicked: {
                    //停止并释放管道，将answerruequestrec的visible置为false,关闭通话窗口
                    communicationPageControler.hangUp()
                    answerRequestRec.visible=false
                    mediaplayer.pause()
                    mediawindow.close()
                }
            }
        }

        Rectangle{
            id:requestCallRec
            anchors.fill: parent
            visible: false
            color:"transparent"
            Text {
                id:requestcalltext
                text: qsTr("等待对方接听...")
                font.pixelSize: 20
                anchors.centerIn: parent
            }
            Button{
                id:closebutton
                width: 100
                height: 30
                x:(parent.width-closebutton.width)/2
                y: (parent.height-closebutton.height)-30
                Text{
                    id:closetext
                    text: qsTr("结束通话")
                    anchors.centerIn: parent

                }
                onClicked: {
                    requestCallRec.visible=false
                    communicationPageControler.hangUp()
                    mediaplayer.pause()
                    mediawindow.close()
                }
            }
        }
        Rectangle{
            id:onThePhoneRec
            anchors.fill: parent
            color:"transparent"
            visible: false
            Button{
                id:hangUpButton
                width: 100
                height: 30
                x:(parent.width-closebutton.width)/2
                y: (parent.height-closebutton.height)-30
                Text{
                    text: qsTr("结束通话")
                    anchors.centerIn: parent
                }
                onClicked: {
                    //停止媒体线程关闭管道
                    communicationPageControler.hangUp()

                    onThePhoneRec.visible=false

                    mediawindow.close()
                }
            }
        }
    }
    Connections{
        target: communicationPageControler
        function onvideoCallRequest(){
            mediawindow.show()
            answerRequestRec.visible=true
            // mediaplayer.play()
        }
    }
    Connections{
        target: communicationPageControler
        function oncloseVideoWindow(){
            mediawindow.close()
            answerRequestRec.visible=false
            // mediaplayer.play()
        }
    }
}

