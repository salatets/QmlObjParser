import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15 as QQ
import QtQuick.Dialogs 1.3

import GLScene 1.0

QQ.ApplicationWindow {
    id: root
    minimumWidth: 400
    minimumHeight: 400
    visible: true

    menuBar: QQ.MenuBar {
        QQ.Menu {
            title: qsTr("&File")
            QQ.Action {
                text: qsTr("&Open...")
                shortcut: StandardKey.Open
                onTriggered: fileDialog.open()
            }
            QQ.MenuSeparator { }
            QQ.Action {
                text: qsTr("&Quit")
                shortcut: StandardKey.Quit
                onTriggered: Qt.quit()
            }
        }
        QQ.Menu {
            title: qsTr("&Labs")
            QQ.Action {
                text: qsTr("Lab 1")
                onTriggered: {
                    glbox.viewMode = GLScene.FH_Model
                }
            }
            QQ.Action {
                text: qsTr("Lab 2")
                onTriggered: {
                    glbox.viewMode = GLScene.Scene
                }
            }
            QQ.Action {
                text: qsTr("RGZ")
                onTriggered: {
                    glbox.viewMode = GLScene.Model
                }
            }
        }

        QQ.Menu  {
            title: qsTr("&Help")

            QQ.Action {
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
        visible: controls_layout.visible
        anchors.fill: controls_layout
        anchors.margins: -10
        color: "#e0e0e0"
        border.width: 1
        opacity: 0.8
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
            QQ.CheckBox{
                id: checkbox
                checked: true
                onCheckStateChanged:
                {
                    glbox.perspective = checkbox.checked;
                }
            }
            QQ.Label {
                Layout.fillWidth: true
                text: qsTr("Perspective")
            }
        }

        RowLayout{
            QQ.Label {
                Layout.fillWidth: true
                text: qsTr("light position")
            }

            QQ.Slider{
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
        title: qsTr("Please choose a .obj file")
        folder: shortcuts.home
        onAccepted: {
            glbox.path = fileDialog.fileUrl
        }
    }

    QQ.Dialog {
        id: helpDialog
        title: qsTr("help")
        modal: false

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        standardButtons: Dialog.Close

        QQ.ScrollView {
            anchors.fill: parent
            clip:true
            QQ.ScrollBar.horizontal.policy: QQ.ScrollBar.AlwaysOff
            Text {
                width: parent.width
                textFormat: Text.StyledText
                wrapMode: Text.WordWrap
                text: qsTr('<h1>Controls</h1>
<p>Hold and move mouse to rotate object<br>
Use mouse wheel for zoom object<br>
In different modes may be additional control panel, dont shy to use it<br>
<h1>Supported formats</h1>
<p> in this moment project support .obj and .scene formats<br>
<h1>Scene format</h1>
<p>.scene files constain(separator - new line):
<ul>
<li> mesh sx sy sz tx ty tz name.obj<br>
there are first coordinats for object scale in scene, second for translation
')
            }
        }

    }
}
