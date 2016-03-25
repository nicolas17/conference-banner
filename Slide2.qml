import QtQuick 2.3

Rectangle {
    id: slide2
    width: 1280
    height: 720
    color: "#231f20"

    //width: 1366
    //height: width/(16/9)

    anchors.horizontalCenter: parent.horizontalCenter
    anchors.top: parent.bottom

    Text {
        text: "[Acá iría la lista de charlas]"
        font.family: fjalla.name
        font.pixelSize: 70
        color: "#0089cf";
        y: 320

        anchors.horizontalCenter: parent.horizontalCenter
    }
}
