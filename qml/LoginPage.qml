import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow{
    id:loginpage
    width: 320
    height:420
    visible:true

    // flags: Qt.Window|Qt.FramelessWindowHint

    title:qsTr("ChatVision")
    Loader{
        id:loginloader
        anchors.fill: parent
        sourceComponent: logincomponent
        source: ""
    }

    Component{
        id:logincomponent

        Rectangle{
            id:loginrec
            anchors.fill: parent
            Image{
                id:backg
                anchors.fill: parent
                source: "qrc:/image/bg3.png"
            }

            Rectangle{
                id:rec
                anchors.horizontalCenter: parent.horizontalCenter
                y:parent.height/7*4
                Column{
                    id:logincolumn

                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    Rectangle{
                        id:textrec
                        width: loginrec.width/8*6
                        height:loginrec.height/5
                        color:"transparent"
                        Text{
                            id:logintext
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            text:qsTr("ChatVision")
                            font.pixelSize: 35
                            color: "Black"
                        }

                    }
                    spacing: 10
                    Rectangle{
                        id:inputrec
                        width:loginrec.width/8*6
                        height: loginrec.height/9
                        opacity: 0.6
                        radius: 5
                        // color: "transparent"
                        Row{
                            id:inputrow
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            Rectangle{
                                width: inputrec.width/5
                                height: inputrec.height/2
                                color:"transparent"
                                Text {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                    id: myidtext
                                    text: qsTr("账号:")
                                    font.pixelSize: 20
                                }
                            }
                            spacing: 10
                            Rectangle{
                                width:inputrec.width/5*3
                                height:inputrec.height/2
                                color:"transparent"
                                border.color: "black"
                                TextInput{
                                    id:id_textinput
                                    anchors.fill: parent
                                    autoScroll: false
                                    echoMode: TextInput.Normal
                                    font.pixelSize: 18
                                    horizontalAlignment: TextInput.AlignLeft
                                    verticalAlignment: TextInput.AlignVCenter
                                    leftPadding: 10
                                    validator: RegularExpressionValidator{
                                        regularExpression: /\b[1-9]\d{7}\b/
                                    }
                                    onAccepted: {
                                        personalPageControler.netNumber=text
                                    }
                                }
                            }
                        }
                    }

                    Button{
                       id:loginbutton
                       width: inputrec.width
                       height:inputrec.height/3*2
                       background: Rectangle{
                           id:bgRec
                           anchors.fill: parent
                           color:"#3399FF"
                           radius: 5
                       }
                       onPressed: {
                           bgRec.color="#66B2FF"
                       }
                       onReleased: {
                           bgRec.color="#3399FF"
                       }
                       Text{
                           anchors.horizontalCenter: parent.horizontalCenter
                           anchors.verticalCenter: parent.verticalCenter
                           id:buttontext
                           text:qsTr("登 录")
                           font.pixelSize: 25
                           color: "white"
                       }
                       onClicked: {
                           personalPageControler.netNumber=id_textinput.text
                           communicationPageControler.myId=id_textinput.text
                           personalPageControler.init();

                           loginpage.width=900
                           loginpage.height=600

                           loginloader.sourceComponent=null
                           loginloader.source="HomePage.qml"

                           communicationPageControler.initChattedList()
                           followingPageController.initRelationData()

                       }
                   }
                }
            }
        }
    }


}
