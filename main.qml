import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15

import Model_qml 1.0

Window {
    visible: true
    width: 640
    height: 480
    minimumWidth: 640
    minimumHeight: 280

    Text {
        id: status_message_text
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width * 0.5
        height: 30
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        fontSizeMode: Text.Fit
        minimumPointSize: 1
        font.pointSize: 10
        elide: Text.ElideRight
        wrapMode: Text.WordWrap
        text: "Status: " + status
        property string status
    }

    ListView {
        id: list_view
        model: Model {
            id: data_model
            onShow_state: {
                status_message_text.status = state
            }
        }
        anchors {
            top: status_message_text.bottom
            topMargin: 10
            bottom: parent.bottom
            bottomMargin: anchors.topMargin
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width * 0.9
        clip: true
        currentIndex: -1
        ScrollBar.vertical: ScrollBar{}
        Rectangle {
            id: font_list_view_frame
            anchors.fill: parent
            border.width: 1
            border.color: "#000000"
            visible: list_view.count === 0
            Text {
                anchors.centerIn: parent
                text: "No data."
            }
        }
        header: Item {
            width: parent.width
            height: 30
            property real name_col_w: name_col.width
            property real latitude_col_w: latitude_col.width
            Rectangle {
                id: name_col
                width: parent.width / 2
                height: parent.height
                color: "gray"
                border.width: 1
                border.color: "#000000"
                Text {
                    anchors.centerIn: parent
                    text: "Name"
                }
            }
            Rectangle {
                id: latitude_col
                anchors {
                    left: name_col.right
                }
                width: parent.width / 2
                height: parent.height
                color: "gray"
                border.width: name_col.border.width
                border.color: name_col.border.color
                Text {
                    anchors.centerIn: parent
                    text: "Latitude"
                }
            }
        }
        delegate: Item {
            id: delegate
            width: list_view.width
            height: 40
            property int border_w: 1
            property color border_color: "#000000"
            Rectangle {
                id: name_frame
                width: list_view.headerItem.name_col_w
                height: parent.height
                border.width: delegate.border_w
                border.color: delegate.border_color
                Text {
                    id: name
                    width: parent.width
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    fontSizeMode: Text.Fit
                    minimumPointSize: 1
                    font.pointSize: 10
                    elide: Text.ElideRight
                    wrapMode: Text.WordWrap
                    text: model.name
                }
            }
            Rectangle {
                id: latitude_frame
                anchors {
                    left: name_frame.right
                }
                width: list_view.headerItem.latitude_col_w
                height: parent.height
                color: Number(latitude.text) < 0 ? "#ff0000" : "#00ff00"
                border.width: delegate.border_w
                border.color: delegate.border_color
                Text {
                    id: latitude
                    width: parent.width
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    fontSizeMode: Text.Fit
                    minimumPointSize: 1
                    font.pointSize: 10
                    elide: Text.ElideRight
                    wrapMode: Text.WordWrap
                    text: model.latitude
                }
            }
        }
    }

}
