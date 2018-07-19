import Charts 1.0
import QtQuick 2.0



Item {
    width: 900; height: 700

    CircleBoardGUI {
        id: aCircleBoard
        anchors.centerIn: parent
        width: 600; height: 600
        name: "This board"
        color: "black"
        Component.onCompleted: initialize()
        onRefreshScoreBoard: {
            scoreText.text = getScoreString()
        }
    }

    Rectangle {
        id: statusBar
        anchors.bottom: parent.bottom
        width: 100; height: 50
        Rectangle {
            id: restartButton
            anchors.left: parent.left
            color: "#50A0BB00"
            width: 100; height: 50

            Text {
                id: buttonLabel
                anchors.centerIn: parent
                text: "Restart Game"
                font.bold: true
                font.pointSize: 10
            }

            MouseArea {
                id: buttonMouseArea
                    anchors.fill: parent
                    onClicked: {
                        aCircleBoard.restartGame()
                    }
            }
        }
        Rectangle {
            id: scoreBoard
            anchors.left: restartButton.right
            width: 400
            height: 50

            Text {
                id: scoreText
                anchors.centerIn: parent
                text: ""
                font.bold: true
                font.pointSize: 10
            }

            MouseArea {
                id: testButton
                    anchors.fill: parent
                    onClicked: scoreText.text = "Woooo"
            }

        }
    }

}
