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

        MenuSeparator { }
        Menu  {
            title: qsTr("&Help")

            Action {
                text: qsTr("&Help")
                shortcut: StandardKey.HelpContents
                onTriggered: helpDialog.open()
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
        visible: controls_layout.visible
        anchors.fill: controls_layout
        anchors.margins: -10
    }

    ColumnLayout{
        id: controls_layout
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        visible: glbox.viewMode === GLScene.Model

        opacity: rect_controls.opacity
        anchors.margins: 20

        RowLayout{
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
                Component.onCompleted:
                {
                    value = glbox.pos;
                }
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

    Dialog {
        id: helpDialog
        title: "help"
        Text {
            textFormat: Text.StyledText
            text: '
<h1>Controls</h1>
<p>Hold and move mouse to rotate object<br>
Use mouse wheel for zoom object<br>
In different modes may be additional control panel, dont shy to use it<br>
<h1>Supported formats</h1>
<p> in this moment project support .obj and .scene formats<br>
<h1>Scene format</h1>
.scene files constais(separator - new line):
<ul>
<li> mesh sx sy sz tx ty tz name.obj<br>
there are first coordinats for object scale in scene, second for translation
'
        }

        standardButtons: Dialog.Ok
    }
}
