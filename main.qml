/*
 * Copyright (c) 2016 Nicolás Alvarez
 *
 * Licensed under the General Public License version 3,
 * or (at your option) any later version. See the LICENSE
 * file for details.
 */


import QtQuick 2.3
import QtQml.Models 2.1

Rectangle {
    FontLoader { id: fjalla; source: "FjallaOne-Regular.ttf" }
    color: "black"

    ObjectModel {
        id: slideModel
        Slide1 {}
        Slide2 {}
        Slide1 {} // always repeat the last one
    }

    Item {
        width: 1280
        height: 720
        clip: true
        scale: Math.min(parent.height, parent.width/(16/9))/720
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        ListView {
            id: slideListView
            width: 1280
            height: 720
            model: slideModel
            highlightFollowsCurrentItem: true
            highlightMoveDuration: 1000
            highlightMoveVelocity: -1
            highlightRangeMode: ListView.StrictlyEnforceRange
            interactive: false // disable mouse flicking

            Timer {
                interval: 3000
                repeat: true
                running: true
                onTriggered: ++slideListView.currentIndex;
            }
            onAtYEndChanged: if (atYEnd) positionViewAtBeginning()
        }
    }
}
