import Qt 4.7

Rectangle {
    id: rect

    function sendMessage() {
        if(!ServerSelector.setServer(server.text, port.text, cert.text)) {
            reply.text = "Failed to set server"
        } else {
            reply.text = "Reply: " + ServerSelector.echo(msg.text)
        }
    }

    function reload() {
        ServerSelector.reload()
    }

    anchors.fill: parent

    Image {
        anchors.fill: parent
        fillMode: Image.Tile
        source: "images/background.png"
    }

    Rectangle {
        id: server_config
        anchors.centerIn: parent
        width: 500

        Rectangle {
            id: desc
            Text {
                text: "Connect to:                 "
                font.pixelSize: 16; font.bold: true
                anchors.centerIn: parent
            }
            height: 28
        }

        LineInput {
            id: server
            width: 300
            text: "selinux.inso.tuwien.ac.at"
            focus: true
            anchors.left: desc.right
            anchors.leftMargin: 7
        }

        Rectangle {
            id: colon
            anchors.left: server.right
            anchors.leftMargin: 7
            Text {
                text: ":"
                font.pixelSize: 16; font.bold: true
                anchors.centerIn: parent
            }
            height: 28
        }

        LineInput {
            id: port
            anchors.left: colon.right
            anchors.leftMargin: 5
            width: 80
            text: "1337"
            focus: true
        }

        LineInput {
            id: cert
            anchors.left: port.right
            anchors.leftMargin: 20
            width: 100
            text: "ca.crt"
            focus: true
        }
    }


    Rectangle {
        id: reply_rect
        anchors.top: server_config.bottom
        anchors.left: server_config.left
        anchors.topMargin: 35

        Rectangle {
            id: msg_desc
            Text {
                text: "Message:"
                font.pixelSize: 16; font.bold: true
                anchors.centerIn: parent
            }
            height: 28
        }

        LineInput {
            id: msg
            width: 300
            text: "Hello world"
            focus: true
            anchors.left: msg_desc.right
            anchors.leftMargin: 50
        }
    }


    Rectangle {
        anchors.top: reply_rect.bottom
        anchors.horizontalCenter: server_config.horizontalCenter
        anchors.topMargin: 35

        id: reply
        property alias text: tmp.text
        Text {
            id: tmp
            text: "n/a"
            font.pixelSize: 16; font.bold: true
            anchors.centerIn: parent
        }
        height: 28
    }


    Rectangle {
        id: btn_reload

        property bool pressed: false

        width: 100; height: 40
        anchors.right: parent.right; anchors.rightMargin: 20
        anchors.bottom: parent.bottom; anchors.bottomMargin: 80
        radius: 6
        color: pressed ? "gray" : "white"

        Text {
            anchors.centerIn: parent
            text: "Reload"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: { reload() }
        }
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
            text: "Send"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: { sendMessage() }
        }
    }
}