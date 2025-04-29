import QtQuick
import QtQuick.Controls

Window {
    id:calendar_page

    width: 270
    height:250
    // flags: Qt.Window | Qt.FramelessWindowHint

    property date selectedDate: new Date()
    property date selectedDay: new Date()

    Column {
        anchors.centerIn: parent
        spacing: 10
        anchors.fill:parent

        Row{
            width:calendar_page.width
            height:35
            // anchors.horizontalCenter: parent.horizontalCenter

            Rectangle{
                id:pre_month
                width:30
                height:width
                x:20
                Image{
                    id:previous_icon

                    width:20
                    height:width
                    source: "qrc:/image/previous.svg"
                    anchors.centerIn:parent
                }

                TapHandler{
                    onTapped:selectedDate = new Date(selectedDate.getFullYear(), selectedDate.getMonth() - 1, 1);
                }
            }

            // 显示当前月份和年份
            Label {
                text: Qt.formatDate(selectedDate, "yyyy - M")
                font.pixelSize: 20
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: pre_month.verticalCenter
                color: "black"
            }

            Rectangle{
                id:next_month
                width:pre_month.width
                height:width
                x:parent.width-50
                Image{
                    width:previous_icon.width
                    height:width
                    source: "qrc:/image/next.svg"
                    anchors.centerIn:parent
                }

                TapHandler{
                    onTapped:selectedDate = new Date(selectedDate.getFullYear(), selectedDate.getMonth() + 1, 1);
                }
            }
        }

        // 显示星期几
        DayOfWeekRow {
            locale: Qt.locale("en_US")
            width:monthGrid.width
            anchors.horizontalCenter: parent.horizontalCenter
            delegate: Label {
                // width:27
                text: model.shortName.charAt(0).toUpperCase()
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                color: "black"
            }
        }

        // 显示月份网格
        MonthGrid {
            id: monthGrid
            month: selectedDate.getMonth() // 月份从 0 开始，需要加 1
            year: selectedDate.getFullYear()
            locale: Qt.locale("en_US")
            width:calendar_page.width * 0.9
            height:calendar_page.height * 0.45
            anchors.horizontalCenter: parent.horizontalCenter

            delegate: Rectangle {
                id:day_rec

                width: 27
                height: model.month === monthGrid.month ? 22 : 0.1

                color: {
                    // 如果当前日期是选中的日期，背景变灰
                    if (model.year === selectedDay.getFullYear() &&
                        model.month === selectedDay.getMonth() &&
                        model.day === selectedDay.getDate()) {
                        return "#eee";
                    }
                    // 如果是当前月份的日期，背景为白色
                    else if (model.month === monthGrid.month) {
                        return "white";
                    }
                }

                Label {
                    text: model.day
                    anchors.centerIn: parent
                    color:"black"
                    font.pixelSize: 15
                    visible: model.month === monthGrid.month ? true : false
                }

                TapHandler {
                    onTapped: {
                        selectedDay = new Date(model.year, model.month, model.day);
                        communicationPageControler.msgDate = Qt.formatDate(selectedDay,"yyyy-MM-dd");
                        calendar_page.visible = false;
                    }
                }
            }

        }
    }
}
