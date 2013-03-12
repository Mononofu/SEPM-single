import Qt 4.7

Rectangle {
    id: rect

    property string text: "Using Qt class to echo this"

    function updateUI() {
        ServerSelector.toggleEcho(button.pressed); // calling ServerSelector::toggleEcho
        text.text = ServerSelector.echo(rect.text) // calling ServerSelector::echo
    }

    anchors.fill: parent
    color: "black"

    Component.onCompleted: updateUI()

    Text {
        id: text
        anchors.centerIn: parent
        color: "white"
    }

    Rectangle {
        id: button

        property bool pressed: false

        width: 100; height: 40
        anchors.right: parent.right; anchors.rightMargin: 20
        anchors.bottom: parent.bottom; anchors.bottomMargin: 20
        radius: 6
        color: pressed ? "gray" : "white"

        Text {
            anchors.centerIn: parent
            text: "Reverse"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: { button.pressed = !button.pressed; updateUI() }
        }
    }
}
