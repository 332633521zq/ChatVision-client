import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

Item{
    width: personalpop.width
    height: personalpop.height

    Popup{
        id:personalpop
        width: 250
        height: 300
        Column{
            anchors.horizontalCenter: parent.horizontalCenter

            Rectangle{
                id:up_rec
                width:personalpop.width * 0.9
                height:personalpop.height * 0.27
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
                            source:"qrc:/image/bg2.jpg"
                            fillMode: Image.PreserveAspectCrop
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
                                    text:"OnlineState:"
                                    font.pixelSize: 12
                                }
                            }
                        }
                    }
                }
            }
        }
    }

}


