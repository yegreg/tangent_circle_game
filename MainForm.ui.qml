import QtQuick 2.6

Rectangle {
    id: simplebutton
    color: "grey"
    width: 150; height: 75

    MouseArea{
            id: buttonMouseArea

            anchors.fill: parent //anchor all sides of the mouse area to the rectangle's anchors
                    //onClicked handles valid mouse button clicks
            onClicked: console.log(buttonLabel.text + " clicked" )
        }

    Text{
        id: buttonLabel
        anchors.centerIn: parent
        text: "button label"
    }
}
