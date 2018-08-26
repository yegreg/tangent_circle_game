import Charts 1.0
import QtQuick 2.0
import QtQuick.Controls 1.4



Item {
    property int marginSize: 20
    property string defaultFont: "Didot"
    property int circleBoardSize: 600
    property int scoreBoardHeight: 100
    property int menuWidth: 200
    property int windowWidth: circleBoardSize + menuWidth
    property int windowHeight: circleBoardSize + scoreBoardHeight

    width: windowWidth
    height: windowHeight

    CircleBoardGUI {
        id: aCircleBoard
        anchors.left: parent.left
        anchors.top: parent.top
        width: circleBoardSize; height: circleBoardSize
        name: "This board"
        color: "black"
        Component.onCompleted: initialize()
        onRefreshScoreBoard: {
            scoreText.text = getScoreString()
            remainingRoundsText.text = getRemainingRoundString()
        }
        onGameOver: {
            endGameWindow.visible = true
            endGameLabel.text = getWinnerString()
        }
        onNewGame: {
            endGameWindow.visible = false
        }
    }

    Rectangle {
        id: endGameWindow
        width: 400
        height: 200
        anchors.centerIn: aCircleBoard;
        color: "#D09010FF"
        border.color: "#C0000000"
        border.width: 3
        visible: false

        Text {
            id: endGameLabel
            anchors.centerIn: parent
            text: "Game over"
            font.family: defaultFont
            font.pointSize: 16
            font.bold: true
            color: "#F0FFDDBB"
            style: Text.Outline
            horizontalAlignment: Text.AlignHCenter
        }

    }
    Rectangle {
        id: menuBar
        anchors.top: parent.top
        anchors.left: aCircleBoard.right
        width: menuWidth
        color: "#30BB10FF"
        height: parent.height

        property int textSize: 10
        property int comboBoxWidth: 150

        Text {
            id: numPlayersLabel
            anchors.top: parent.top
            anchors.topMargin: marginSize
            anchors.left: numPlayersDropDown.left
            text: "Number of players:"
            font.family: defaultFont
            font.pointSize: menuBar.textSize
        }

        ComboBox {
            id: numPlayersDropDown
            property bool botOn: false
            width: menuWidth - 2 * marginSize
            anchors.top: numPlayersLabel.bottom
            anchors.topMargin: marginSize
            anchors.horizontalCenter: parent.horizontalCenter
            model: ["1 (vs. bot)", "2", "3"]
            currentIndex: 1 // default two players
            onCurrentIndexChanged: {
                if (!botOptions) return;
                if (numPlayersDropDown.currentIndex == 0) {
                    botOn = true
                    botOptions.visible = true
                    botOptions.height = botOptions.botOptionHeight
                    botOptions.anchors.topMargin = marginSize
                } else {
                    botOn = false
                    botOptions.visible = false
                    botOptions.height = 0
                    botOptions.anchors.topMargin = 0
                }
            }
        }

        Rectangle {
            id: botOptions
            property int botPlayer: 1
            property int botOptionHeight: 140
            height: botOptionHeight
            anchors.top: numPlayersDropDown.bottom
            anchors.topMargin: marginSize
            anchors.left: numPlayersDropDown.left
            visible: true

            property int colorBoxSize: menuWidth / 5

            Text {
                id: botPlayerLabel
                anchors.top: parent.top
                anchors.left: parent.left
                text: "Player color:"
            }

            Rectangle {
                id: playerColorSwitch
                anchors.top: botPlayerLabel.bottom
                anchors.topMargin: marginSize
                height: colorSwitchHeight

                property int selectionWidth: 3
                property int colorSwitchHeight: 60


                Rectangle {
                    id: redPlayerChoice
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.leftMargin: marginSize
                    color: aCircleBoard.getPlayerColor(0);
                    width: botOptions.colorBoxSize
                    height: botOptions.colorBoxSize

                    Rectangle {
                        id: redPlayerSelectionMarker
                        anchors.fill: parent
                        color: "transparent"
                        border.color: "black"
                        border.width: playerColorSwitch.selectionWidth
                        visible: true
                    }

                    MouseArea {
                        id: redPlayerSelectButton
                        anchors.fill: parent
                        onClicked: {
                            botOptions.botPlayer = 1
                            redPlayerSelectionMarker.visible = true
                            bluePlayerSelectionMarker.visible = false
                        }
                    }
                }

                Rectangle {
                    id: bluePlayerChoice
                    anchors.top: parent.top
                    anchors.left: redPlayerChoice.right
                    anchors.leftMargin: marginSize
                    color: aCircleBoard.getPlayerColor(1);
                    width: botOptions.colorBoxSize
                    height: botOptions.colorBoxSize

                    Rectangle {
                        id: bluePlayerSelectionMarker
                        anchors.fill: parent
                        color: "transparent"
                        border.color: "black"
                        border.width: playerColorSwitch.selectionWidth
                        visible: false
                    }

                    MouseArea {
                        id: bluePlayerSelectButton
                        anchors.fill: parent
                        onClicked: {
                            botOptions.botPlayer = 0
                            bluePlayerSelectionMarker.visible = true
                            redPlayerSelectionMarker.visible = false
                        }
                    }

                }


            }

            Text {
                id: botDifficultyLabel
                text: "Bot difficulty:"
                anchors.top: playerColorSwitch.bottom
                anchors.left: parent.left
            }

            ComboBox {
                id: botDifficulty
                anchors.top: botDifficultyLabel.bottom
                anchors.topMargin: marginSize / 2
                anchors.left: parent.left
                width: menuBar.comboBoxWidth
                model: [1, 2, 3, 4]
            }

        }

        Text {
            id: boardDepthLabel
            anchors.top: botOptions.bottom
            anchors.topMargin: marginSize
            anchors.left: numPlayersDropDown.left
            text: "Board depth:"
            font.family: defaultFont
            font.pointSize: menuBar.textSize
        }

        ComboBox {
            id: boardDepthDropDown
            width: menuBar.comboBoxWidth
            anchors.top: boardDepthLabel.bottom
            anchors.topMargin: marginSize
            anchors.left: numPlayersDropDown.left
            property int minDepth: 3
            model: ["3", "4", "5", "6", "7", "8"]
            currentIndex: 1
        }

        Text {
            id: boardSymmetryLabel
            anchors.top: boardDepthDropDown.bottom
            anchors.topMargin: marginSize
            anchors.left: numPlayersDropDown.left
            text: "Circles in middle:"
            font.family: defaultFont
            font.pointSize: menuBar.textSize
        }

        ComboBox {
            id: boardSymmetryDropDown
            width: menuBar.comboBoxWidth
            anchors.top: boardSymmetryLabel.bottom
            anchors.topMargin: marginSize
            anchors.left: numPlayersDropDown.left
            property int minSymmetry: 3
            model: ["3", "4", "5", "6"]
            currentIndex: 1
        }

        Rectangle {
            id: restartButton
            anchors.top: boardSymmetryDropDown.bottom
            anchors.topMargin: marginSize
            anchors.horizontalCenter: boardSymmetryDropDown.horizontalCenter
            color: "#F0EE9000"
            width: menuBar.comboBoxWidth - 2 * marginSize
            height: 30

            Text {
                id: buttonLabel
                anchors.centerIn: parent
                text: "Restart Game"
                font.bold: true
                font.pointSize: menuBar.textSize
                font.family: defaultFont
            }

            MouseArea {
                id: buttonMouseArea
                anchors.fill: parent
                onClicked: {
                    aCircleBoard.restartGame(numPlayersDropDown.currentIndex + 1,
                                             boardDepthDropDown.currentIndex + boardDepthDropDown.minDepth,
                                             boardSymmetryDropDown.currentIndex + boardSymmetryDropDown.minSymmetry,
                                             botOptions.botPlayer,
                                             botDifficulty.currentIndex)
                }
            }
        }

    }

    Rectangle {
        id: statusBar
        anchors.left: parent.left
        anchors.top: aCircleBoard.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        property int textSize: 12
        property int marginSize: 30

        Rectangle {
            id: scoreBoard
            width: windowWidth / 2
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            color: "#308030FF"


            Text {
                id: scoreLabel
                anchors.bottom: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                text: "SCORE"
                font.bold: true
                font.family: defaultFont
                font.pointSize: statusBar.textSize
            }


            Text {
                id: scoreText
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: scoreLabel.bottom
                text: ""
                font.family: defaultFont
                font.pointSize: statusBar.textSize
            }
        }
        Rectangle {
            id: remainingRoundsBar
            anchors.left: scoreBoard.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            color: "#307080BB"

            Text {
                id: remainingRoundsLabel
                anchors.bottom: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                text: "REMAINING ROUNDS"
                font.bold: true
                font.family: defaultFont
                font.pointSize: statusBar.textSize
            }

            Text {
                id: remainingRoundsText
                anchors.top: remainingRoundsLabel.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                text: ""
                font.family: defaultFont
                font.pointSize: statusBar.textSize
            }
        }

    }

}
