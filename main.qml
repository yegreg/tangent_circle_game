import Charts 1.0
import QtQuick 2.0



Item {
    width: 800; height: 600

    CircleBoard {
        id: aCircleBoard
        anchors.centerIn: parent
        width: 600; height: 600
        name: "This board"
        color: "black"
        Component.onCompleted: initialize();
    }

//    Rectangle {
//        id: simpleButton
//        color: "grey"
//        width: 150; height: 75

//        Text {
//            id: buttonLabel
//            anchors.centerIn: parent
//            text: "button label"
//        }

//        MouseArea {
//                id: buttonMouseArea

//                // Anchor all sides of the mouse area to the rectangle's anchors
//                anchors.fill: parent
//                // onClicked handles valid mouse button clicks
//                onClicked: aCircleBoard.buttonClick()
//        }
//    }

}
