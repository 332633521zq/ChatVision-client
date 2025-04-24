import QtQuick
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects
import QtMultimedia
Rectangle{
    id:homepagerec
    property string communication_name:"";
    property bool isfollowlistloadeed:false;

    property ListModel msglistmodel:rightloader.item.msglistmodel
    property ListModel searchmodel
    property ListModel conversationlist:chattedListModel
    property real randomNumber:0
    property bool isNetSearch: false
    property var startTime
    property bool isonline

    ListModel{
        id:chattedListModel
        // ListElement{
        //     obid:"20000001"
        //     memo:"ts"
        //     nickname:""
        //     area:""
        //     gender:""
        //     signature:""
        //     avatar_path:""
        // }
    }


    BaseInfoProperties{
        id:bips
    }
    Connections{
        target:centerloader.item
        function onLoadfollowinginfo() {
            rightloader.source = "qrc:/qml/FollowUserInfoPage.qml";
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
            color: "#F2F2F2"
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
                        source:bips.my_avatar
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
                        onTapped: {
                            var myinfo = followingPageController.myinfo
                            bips.following_uid = myinfo.uid/* "2000000"*/
                            bips.nickname = myinfo.nickname /*"85"*/
                            bips.memo = myinfo.nickname /*"85"*/
                            bips.signal_text = myinfo.signature/*"罪业的报偿"*/
                            bips.area = myinfo.area/*"中国大陆 重庆"*/
                            bips.avatar_path = myinfo.avatar_path_/*"../assets/Picture/avatar/cats.jpg"*/
                            bips.gender = myinfo.gender
                            personalpop.open()
                            // // if(isfollowlistloadeed == true){
                            // console.log("tapped ")
                            // rightloader.source = "qrc:/qml/PersonalInfo.qml";
                            // // }
                        }
                    }
                }

                Popup{
                    id:personalpop
                    width: 300
                    height: 300
                    x:leftbar.width-10
                    y:30
                    background: Rectangle{
                        color: "white"
                        radius: 5
                        clip: true
                        border.color: "black"
                    }
                    Column{
                        anchors.fill: parent
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 10
                        Rectangle{
                            id:up_rec
                            width:personalpop.width * 0.9
                            height:personalpop.height * 0.3

                            Row{
                                spacing: 10
                                Rectangle{
                                    id:avaterrec1
                                    width: 60
                                    height: width
                                    // color:"orange"
                                    y:10
                                    Image{
                                        id:avaterimage1
                                        anchors.fill: parent
                                        source:"file:///root/屏幕截图_20250422_132716.png"
                                        fillMode: Image.PreserveAspectCrop
                                        sourceSize: Qt.size(parent.width,parent.height)
                                        visible: false
                                    }
                                    Rectangle{
                                        id:maskrec1
                                        radius: avaterrec1.width * 0.5
                                        anchors.fill: parent
                                        visible: false
                                    }
                                    OpacityMask{
                                        anchors.fill: maskrec1
                                        source:avaterimage1
                                        maskSource: maskrec1
                                    }

                                }
                                Rectangle{
                                    id: idrec

                                    width: up_rec.width-avaterrec1.width-10
                                    height:avaterrec1.height - 20
                                    color: "red"
                                    anchors.verticalCenter: avaterrec1.verticalCenter

                                    Rectangle{
                                        width:idrec.width
                                        height:avaterrec1.height
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
                                                text:"OnlineState:"+isonline
                                                font.pixelSize: 12
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        Rectangle{
                            id:signatureRec
                            width:personalpop.width * 0.9
                            height:20
                            Row{
                                anchors.fill: parent
                                Text {
                                    width:70
                                    height:parent.height
                                    text: qsTr("签名")
                                }
                                Text {
                                    text: bips.signal_text
                                }
                            }
                        }
                        Rectangle{
                            id:addrRec
                            width:personalpop.width * 0.9
                            height:20
                            Row{
                                anchors.fill: parent
                                Text {
                                    width:70
                                    height:parent.height
                                    text: qsTr("所在地")
                                }
                                Text {
                                    text: bips.area
                                }
                            }
                        }
                        Rectangle{
                            width:parent.width
                            height: 30
                        }

                        Rectangle{
                            id:toolRec
                            width:personalpop.width * 0.9
                            height:20
                            Row{
                                anchors.fill: parent
                                spacing: 10
                                Rectangle{
                                    width: 20
                                    height: 30
                                }

                                Button{
                                    width: 110
                                    height: 30
                                    background: Rectangle{
                                        anchors.fill: parent
                                        radius: 3
                                        color: "white"
                                        border.color: "#ccc"
                                    }
                                    Text{
                                        anchors.centerIn: parent
                                        text:"编辑资料"
                                        font.pointSize: 10
                                    }
                                    onClicked: {
                                        personalinfowindow.show()
                                        personalpop.close()
                                    }
                                }
                                Button{
                                    width: 110
                                    height: 30
                                    background: Rectangle{
                                        anchors.fill: parent
                                        radius: 3
                                        color: "#3399FF"
                                    }
                                    Text{
                                        anchors.centerIn: parent
                                        text:"发送消息"
                                        font.pointSize: 10
                                    }
                                }

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
                    Rectangle{
                        id:tapmaskconversation
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
                            tapmaskconversation.visible=true
                            tapmaskfind.visible=false
                            tapmaskfocus.visible=false
                            tapmaskset.visible=false

                            rightloader.visible=false
                            centerloader.source="qrc:/qml/ConversationListPage.qml"
                            isfollowlistloadeed = false
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
                    Rectangle{
                        id:tapmaskfind
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
                            tapmaskconversation.visible=false
                            tapmaskfind.visible=true
                            tapmaskfocus.visible=false
                            tapmaskset.visible=false

                            rightloader.visible=false
                            findfriendPageController.sendRandowRequest()
                            centerloader.source="qrc:/qml/FindFriendPage.qml"
                            isfollowlistloadeed = false
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
                    Rectangle{
                        id:tapmaskfocus
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
                            tapmaskconversation.visible=false
                            tapmaskfind.visible=false
                            tapmaskfocus.visible=true
                            tapmaskset.visible=false

                            rightloader.source = ""
                            isfollowlistloadeed = true;
                            rightloader.visible=false
                            centerloader.source = "qrc:/qml/FollowListPage.qml"
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
                    Rectangle{
                        id:tapmaskset
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
                            tapmaskconversation.visible=false
                            tapmaskfind.visible=false
                            tapmaskfocus.visible=false
                            tapmaskset.visible=true

                            settingpopup.open()
                            isfollowlistloadeed = false
                        }
                    }
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
                color: "white"
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
        Column{
            height: parent.height
            Rectangle{
                id:searchbar
                width: 240
                height: centerbar.width/7*2-10
                color: "transparent"
                Row{
                    anchors.fill: parent
                    Rectangle{
                        id:spacer
                        width: 20
                        height: 20
                        // color:"red"
                    }

                    Rectangle{
                        id:iamgerec
                        width: 25
                        height: 25
                        color: "#F2F2F2"
                        anchors.verticalCenter: parent.verticalCenter
                        Image{
                            width: 15
                            height: 15
                            anchors.centerIn: parent
                            fillMode: Image.PreserveAspectCrop
                            source: "qrc:/image/Search.svg"
                        }
                    }

                    Rectangle{
                        id:searchrec
                        width: 150
                        height: 25
                        color: "#F2F2F2"
                        anchors.verticalCenter: parent.verticalCenter
                        opacity: 0.8
                        Text{
                            anchors.fill: parent
                            text: "搜索"
                            verticalAlignment: Text.AlignVCenter
                            visible: !searchInput.focus && searchInput.text === ""
                        }

                        TextInput{
                            id:searchInput
                            anchors.fill: parent
                            color: "green"
                            wrapMode: Text.WordWrap
                            autoScroll: false
                            verticalAlignment: TextInput.AlignVCenter
                            validator: RegularExpressionValidator{
                                regularExpression: /\b[1-9]\d{7}\b/
                            }
                            focus: false
                            // focusPolicy: Qt.StrongFocus
                            onTextEdited: {
                                searchController.searchID=text;
                                filterModel(text);
                                console.log("text is changed");
                            }
                            onAccepted: {
                                console.log("enter was pressed");
                                searchController.searchUser();
                            }

                            TapHandler{
                                onTapped: {
                                    centerloader.source="qrc:/qml/SearchPage.qml"
                                    searchmodel=centerloader.item.searchmodel

                                }
                            }
                        }
                    }
                }
////////////////////////////////////////////////////////////////////////

            }

            //中间的会话列表
             Rectangle{
                 id:centerbar
                 width:240
                 height: parent.height-searchrec.height
                 color:"#FFFFFF"
                 Loader{
                     id:centerloader
                     anchors.fill: parent
                     source: "qrc:/qml/ConversationListPage.qml"
                 }
             }

        }


    //右边的矩形
        Rectangle{
            id:rightbar
            width: 600
            height: parent.height
            color: "#F2F2F2"
            Loader{
                id:rightloader
                anchors.fill: parent
                source: "qrc:/qml/CommunicationPage.qml"
                visible: false
            }
        }
    }

    Window{
        id:mediawindow
        width: 600
        height: 500
        color:"white"
        MediaPlayer{
            id:mediaplayer
            source: "qrc:/audio/callaudio.mp3"
            audioOutput: AudioOutput{}
        }
        Image {
            id: videoimage
            anchors.fill: parent
            source: "image://pictures/avater"
            cache: false

        }
        Timer{
            id:t1
            interval: 5
            running: true
            repeat: true
            onTriggered: {
                randomNumber++
                videoimage.source="image://pictures/"+"a"+randomNumber
            }
        }
        Rectangle{
            id:myself_windowrec
            width: 200
            height: 160
            color: "white"
            Image{
                anchors.fill: parent
                id:myselfimage
                source:"image://pictures/b"
            }
        }
        Timer{
            id:t2
            interval: 1
            running: true
            repeat: true
            onTriggered: {
                randomNumber++
                myselfimage.source="image://pictures/"+"b"+randomNumber
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
                    t1.running=true
                    communicationPageControler.getThrough()
                    answerRequestRec.visible=false
                    onThePhoneRec.visible=true
                    mediaplayer.stop()
                    mediaplayer.pause()
                    startTime=new Date()
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
                    videoimage.source=""
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
                    videoimage.source=""
                    t1.running=false
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
                    videoimage.source=""
                    t1.running=false
                    onThePhoneRec.visible=false
                    mediawindow.close()
                    callTime(startTime)
                }
            }
        }
    }
    //对方发来通话请求
    Connections{
        target: communicationPageControler
        function onvideoCallRequest(){
            mediawindow.show()
            answerRequestRec.visible=true
            mediaplayer.play()
        }
    }
    //对面点击结束通话
    Connections{
        target: communicationPageControler
        function oncloseVideoWindow(){
            mediawindow.close()
            videoimage.source=""
            t1.running=false
            onThePhoneRec.visible=false
            requestCallRec.visible=false
            answerRequestRec.visible=false
            callTime(startTime)
            // mediaplayer.play()
        }
    }
    //向对方发去通话请求后，对方点击同意接听
    Connections{
        target: communicationPageControler
        function onagreeCall(){
            t1.running=true
            requestCallRec.visible=false;
            onThePhoneRec.visible=true;
            mediaplayer.stop()
            mediaplayer.pause()
        }
    }


    //鼠标点击搜索框外时搜索框的focus为false
    TapHandler{
        onTapped: {
            if(!searchInput.contains(point.scenePosition)){
                searchInput.focus=false;
            }
        }
    }
    function filterModel(searchID){
        searchmodel.clear();
        if(searchID!==""){
            for(var i=0;i<chattedListModel.count;i++){
                var item=chattedListModel.get(i);
                if(item.obid.includes(searchID)){
                    searchmodel.append(item);
                }
            }
        }
    }
    Connections{
        target: communicationPageControler
        function onAddListElement(id,memo,nickname, area, gender, signature, avatar_path){
            console.log("add was touched")
            var listelement={};
            listelement.obid=id;
            listelement.memo=memo;
            listelement.nickname=nickname;
            listelement.area=area;
            listelement.gender=gender;
            listelement.signature=signature;
            listelement.avatar_path=avatar_path;
            listelement.unread_count=0;
            chattedListModel.append(listelement)
        }
    }
    Connections{
        target: searchController
        function onAddSearchUserList(id,memo,nickname, area, gender, signature, avatar_path){
            var listelement={};
            listelement.obid=id;
            console.log("id is "+id)
            listelement.memo=memo;
            listelement.nickname=nickname;
            listelement.area=area;
            listelement.gender=gender;
            listelement.signature=signature;
            listelement.avatar_path=avatar_path;
            searchmodel.append(listelement);
            isNetSearch=true;
        }
    }
    function callTime(beginTime){
        var hour=beginTime.getHours();  var minute=beginTime.getMinutes();
        var second=beginTime.getSeconds();  var flagsecond=0;
        var flagminute=0; var now=new Date();
        var endhour=now.getHours(); var endminute=now.getMinutes();
        var endsecond=now.getSeconds(); var elapsedhour=endhour-hour;
        var elapsedminute;  var elapsedsecond;
        if(endsecond < second){
            flagsecond=1;
            elapsedsecond=endsecond+60-second;
        }else{
            elapsedsecond=endsecond-second;
        }
        if(endminute < minute){
            flagminute=1;
            if(flagsecond===1)
                elapsedminute=endminute+60-minute-1;
            else
                elapsedminute=endminute+60-minute;
        }
        else{
            if(flagsecond===1)
                elapsedminute=endminute-minute-1;
            else
                elapsedminute=endminute-minute;
        }
        console.log("hour:"+elapsedhour,"minute:"+elapsedminute,"seconds:"+elapsedsecond)
        var textmsg="通话时长"+elapsedhour+":"+elapsedminute+":"+elapsedsecond+" ☎️"

        communicationPageControler.myMessage=textmsg;
        communicationPageControler.saveMessage();
    }
    Connections{
        target: followingPageController
        function onInitPersonalInfo(myinfo){
            bips.following_uid = myinfo.uid/* "2000000"*/
            bips.nickname = myinfo.nickname /*"85"*/
            bips.memo = myinfo.nickname /*"85"*/
            bips.signal_text = myinfo.signature/*"罪业的报偿"*/
            bips.area = myinfo.area/*"中国大陆 重庆"*/
            bips.avatar_path = myinfo.avatar_path_/*"../assets/Picture/avatar/cats.jpg"*/
            bips.my_avatar=myinfo.avatar_path_
            bips.gender = myinfo.gender
        }
    }
    Connections{
        target: followingPageController
        function onIsOnlineChanged(is_online){
            isonline = is_online
        }
    }
    Window{
        id:personalinfowindow
        width: 400
        height: 420
        title: "编辑资料"
        color: "#F2F2F2"
        flags: Qt.Dialog // 设置为对话框样式
        modality: Qt.WindowModal // 窗口模态
        Column{
            anchors.fill: parent
            spacing: 20
            Rectangle{
                width: parent.width
                height:130
                color:"transparent"
                Rectangle{
                    width: 80
                    height: 80
                    anchors.centerIn: parent
                    color:"transparent"
                    Image{
                        id:avaterimage2
                        anchors.fill: parent
                        source:bips.my_avatar
                        fillMode: Image.PreserveAspectCrop
                        sourceSize: Qt.size(parent.width,parent.height)
                        visible: false
                    }
                    Rectangle{
                        id:maskrec2
                        radius: parent.width * 0.5
                        anchors.fill: parent
                        visible: false
                    }
                    OpacityMask{
                        anchors.fill: maskrec2
                        source:avaterimage2
                        maskSource: maskrec2
                    }
                }
            }
            Rectangle{
                width: parent.width-40
                height: 30
                color: "white"
                anchors.horizontalCenter: parent.horizontalCenter
                radius: 4
                Row{
                    anchors.fill: parent
                    spacing: 20
                    Rectangle{
                        width: 40
                        height: parent.height
                        Text {
                            width: 20
                            text: qsTr("昵称")
                            anchors.centerIn: parent
                            font.pixelSize: 15
                        }
                    }
                    TextInput{
                        id:nicknameinput
                        width: parent.width-20
                        text: bips.nickname
                        font.pixelSize: 15
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
            Rectangle{
                width: parent.width-40
                height: 30
                color: "white"
                anchors.horizontalCenter: parent.horizontalCenter
                radius: 4
                Row{
                    anchors.fill: parent
                    spacing: 20
                    Rectangle{
                        width: 40
                        height: parent.height
                        Text {
                            width: 20
                            text: qsTr("个签")
                            anchors.centerIn: parent
                            font.pixelSize: 15
                        }
                    }
                    TextInput{
                        id:siginput
                        width: parent.width-20
                        text: bips.signal_text
                        font.pixelSize: 15
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
            Rectangle{
                width: parent.width-40
                height: 30
                color: "white"
                anchors.horizontalCenter: parent.horizontalCenter
                radius: 4
                Row{
                    anchors.fill: parent
                    spacing: 20
                    Rectangle{
                        width: 40
                        height: parent.height
                        Text {
                            width: 20
                            text: qsTr("性别")
                            anchors.centerIn: parent
                            font.pixelSize: 15
                        }
                    }
                    TextInput{
                        id:genderinput
                        width: parent.width-20
                        text: bips.gender
                        font.pixelSize: 15
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
            Rectangle{
                width: parent.width-40
                height: 30
                color: "white"
                anchors.horizontalCenter: parent.horizontalCenter
                radius: 4
                Row{
                    anchors.fill: parent
                    spacing: 20
                    Rectangle{
                        width: 40
                        height: parent.height
                        Text {
                            width: 20
                            text: qsTr("地区")
                            anchors.centerIn: parent
                            font.pixelSize: 15
                        }
                    }
                    TextInput{
                        id:areainput
                        width: parent.width-20
                        text: bips.area
                        font.pixelSize: 15
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
            Rectangle{
                width: parent.width-40
                height: 30
                color: "transparent"
                anchors.horizontalCenter: parent.horizontalCenter
                radius: 4
                Row{
                    anchors.fill: parent
                    spacing: 20
                    Rectangle{
                        width: parent.width-150
                        height: parent.height
                        color:"transparent"
                    }
                    Button{
                        width: 55
                        height: 25
                        background: Rectangle{
                            anchors.fill: parent
                            radius: 3
                            color: "#3399FF"
                        }
                        Text{
                            text: "保存"
                            anchors.centerIn: parent
                        }
                        onClicked: {
                            newinfo.nickname=nicknameinput.text
                            newinfo.gender=genderinput.text
                            newinfo.area=areainput.text
                            newinfo.signal_text=siginput.text
                            newinfo.following_uid=bips.following_uid
                            newinfo.avatar_path=bips.avatar_path
                            followingPageController.changePersonalInfo(newinfo.nickname,newinfo.gender,newinfo.area,newinfo.signal_text,newinfo.avatar_path,newinfo.following_uid)
                            personalinfowindow.close()
                        }
                    }
                    Button{
                        width: 55
                        height: 25
                        background: Rectangle{
                            anchors.fill: parent
                            radius: 3
                            color: "#3399FF"
                        }
                        Text{
                            text: "取消"
                            anchors.centerIn: parent
                        }
                        onClicked: {
                            personalinfowindow.close()
                        }
                    }
                }
            }
        }
    }
    BaseInfoProperties{
        id:newinfo
    }
}

