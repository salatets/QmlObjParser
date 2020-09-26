import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import GLScene 1.0

Item {
    width: 400
    height: 400


    GLScene {
        id: glbox
    }

    Rectangle {
        id:rect
        color: Qt.rgba(1, 1, 1, 0.7)
        radius: 10
        border.width: 1
        border.color: "white"
        opacity: 0.5
        anchors.fill: rotate
        anchors.margins: -10

        MouseArea {
               id: mouseArea
               anchors.fill: parent
               anchors.margins: -10
               hoverEnabled: true
               onHoveredChanged: {
                    if(mouseArea.containsMouse)
                        animateOpacityUP.start()
                   else
                        animateOpacityDOWN.start()
               }

           }

        NumberAnimation {
                id: animateOpacityUP
                target: rect
                properties: "opacity"
                from: 0.5
                to: 1.0
                duration: 250
                easing.type: Easing.InQuad
           }

        NumberAnimation {
                id: animateOpacityDOWN
                target: rect
                properties: "opacity"
                from: 1.0
                to: 0.5
                duration: 250
                easing.type: Easing.OutQuad
           }
    }

    ColumnLayout{
        id: rotate
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 20
        opacity: rect.opacity

        Slider{
            from: 0
            to: 359
            Layout.fillWidth: true
            onMoved:
            {
                glbox.pitch = value;
            }
        }
        Slider{
            from: 0
            to: 359
            Layout.fillWidth: true
            onMoved:
            {
                glbox.yaw = value;
            }
        }
        Slider{
            from: 0
            to: 359
            Layout.fillWidth: true
            onMoved:
            {
                glbox.roll = value;
            }
        }
    }
}
