import QtQuick
import Qt5Compat.GraphicalEffects

Window{
    id:personalinfowindow
    width: 400
    height: 420
    title: "编辑资料"
    color: "#F2F2F2"
    Column{
        anchors.fill: parent
        spacing: 10
        Rectangle{
            width: parent.width
            height:100
            color: "pink"
        }
        Rectangle{
            width: parent.width
            height: 50
            color: "white"
            radius: 2
        }

    }
}
