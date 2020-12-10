import QtQuick 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3
import QtQuick.Dialogs 1.3

import GLScene 1.0
//import Helper 1.0

ApplicationWindow {
    width: 400
    height: 400
    visible: true
    color: "white"

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action {
                text: qsTr("&Open...")
                shortcut: StandardKey.Open
                onTriggered: fileDialog.open()
            }
            MenuSeparator { }
            Action {
                text: qsTr("&Quit")
                shortcut: StandardKey.Quit
                onTriggered: Qt.quit()
            }
        }
        Menu {
            title: qsTr("&Labs")
            Action {
                text: qsTr("Lab 1")
                onTriggered: {
                    glbox.viewMode = GLScene.FH_Model
                }
            }
            Action {
                text: qsTr("Lab 2")
                onTriggered: {
                    glbox.viewMode = GLScene.Scene
                }
            }
            Action {
                text: qsTr("RGZ")
                onTriggered: {
                    glbox.viewMode = GLScene.Model
                }
            }
        }
    }

    GLScene {
        id: glbox
        anchors.fill: parent
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
            visible: glbox.viewMode === GLScene.Model
        CheckBox{
            id: checkbox
            checked: true
            onCheckStateChanged:
            {
                glbox.perspective = checkbox.checked;
            }
        }
        Label {
            Layout.fillWidth: true
            Layout.maximumWidth: 60
            text: "perspective"
            font.pixelSize: 30
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
                value: glbox.pos
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
        }
    }
}
