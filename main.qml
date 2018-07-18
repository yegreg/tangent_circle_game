import Charts 1.0
import QtQuick 2.0



Item {
    width: 900; height: 700

    CircleBoard {
        id: aCircleBoard
        anchors.centerIn: parent
        width: 600; height: 600
        name: "This board"
        color: "black"
        Component.onCompleted: initialize();
    }

    Rectangle {
        id: restartButton
        x: 8
        y: 642
        color: "grey"
        width: 100; height: 50

        Text {
            id: buttonLabel
            anchors.centerIn: parent
            text: "Restart Game"
        }

        MouseArea {
            id: buttonMouseArea
            // Anchor all sides of the mouse area to the rectangle's anchors
                anchors.fill: parent
                // onClicked handles valid mouse button clicks
                onClicked: aCircleBoard.restartGame()
        }
    }

}
