import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.0
import GLScene 1.0

Item {
    width: 400
    height: 400


    GLScene {
        id: glbox
    }



    Rectangle{
        id:rect_load
        color: Qt.rgba(1, 1, 1, 0.7)
        radius: 10
        border.width: 1
        border.color: "white"
        opacity: 0.5
        anchors.fill: load_button
        anchors.margins: -10
    }

    Button{
        id: load_button
        text: "load"
        onPressed:fileDialog.open()
        anchors.top: parent.top
        anchors.left:  parent.left
        anchors.margins: 10

    }


    Rectangle{
        id:rect_controls
        color: Qt.rgba(1, 1, 1, 0.7)
        radius: 10
        border.width: 1
        border.color: "white"
        opacity: 0.5
        anchors.fill: controls_layout
        anchors.margins: -10

        MouseArea {
               anchors.fill: parent
               anchors.margins: -10
               hoverEnabled: true
               onEntered: animateOpacityUP.start()
               onExited: animateOpacityDOWN.start()
           }

        NumberAnimation {
                id: animateOpacityUP
                target: rect_controls
                properties: "opacity"
                from: 0.5
                to: 1.0
                duration: 250
                easing.type: Easing.InQuad
           }

        NumberAnimation {
                id: animateOpacityDOWN
                target: rect_controls
                properties: "opacity"
                from: 1.0
                to: 0.5
                duration: 250
                easing.type: Easing.OutQuad
           }
    }

    ColumnLayout{
        id: controls_layout
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom

        opacity: rect_controls.opacity
        anchors.margins: 20
        RowLayout{
                Label {
                    Layout.fillWidth: true
                    Layout.maximumWidth: 60
                    text: "pitch"
                    font.pixelSize: 30
                }

                Slider{
                    from: 0
                    to: 360
                    Layout.fillWidth: true
                    onMoved:
                    {
                        glbox.pitch = value;
                    }

                }
        }

        RowLayout{
                Label {
                    Layout.fillWidth: true
                    Layout.maximumWidth: 60
                    text: "yaw"
                    font.pixelSize: 30
                }

                Slider{
                    from: 0
                    to: 360
                    Layout.fillWidth: true
                    onMoved:
                    {
                        glbox.yaw = value;
                    }

                }
        }
        RowLayout{
                Label {
                    Layout.fillWidth: true
                    Layout.maximumWidth: 60
                    text: "roll"
                    font.pixelSize: 30
                }

                Slider{
                    from: 0
                    to: 360
                    Layout.fillWidth: true
                    onMoved:
                    {
                        glbox.roll = value;
                    }

                }
        }
        RowLayout{
                Label {
                    Layout.fillWidth: true
                    Layout.maximumWidth: 60
                    text: "light"
                    font.pixelSize: 30
                }
                Slider{
                    from: 0
                    to: 6.283
                    value: 3.14
                    Layout.fillWidth: true
                    onMoved:
                    {
                        glbox.pos = value;
                    }
                }

        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a .obj file"
        folder: shortcuts.home
        onAccepted: {
            glbox.path = fileDialog.fileUrl
            Qt.quit()
        }
    }
}
