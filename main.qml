import QtQuick 2.3

Rectangle {
    FontLoader { id: fjalla; source: "FjallaOne-Regular.ttf" }
    color: "black"

    Item {
        width: 1280
        height: 720
        clip: true
        scale: Math.min(parent.height, parent.width/(16/9))/720
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        Item {
            width: 1280
            height: 720
            Slide1 {}
            Slide2 {}
            SequentialAnimation on y {
                loops: Animation.Infinite
                PauseAnimation { duration: 4000 }
                NumberAnimation {
                    from: 0; to: -720;
                    easing.type: Easing.InOutCirc; duration: 500
                }
                PauseAnimation { duration: 4000 }
                NumberAnimation {
                    from: -720; to: 0;
                    easing.type: Easing.InOutCirc; duration: 500
                }
            }
        }
    }
}
