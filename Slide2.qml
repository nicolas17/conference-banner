/*
 * Copyright (c) 2016 Nicolás Alvarez
 *
 * Licensed under the General Public License version 3,
 * or (at your option) any later version. See the LICENSE
 * file for details.
 */

import QtQuick 2.3

Rectangle {
    id: slide2
    width: 1280
    height: 720
    color: "#231f20"

    //width: 1366
    //height: width/(16/9)

    Text {
        text: "[Acá iría la lista de charlas]"
        font.family: fjalla.name
        font.pixelSize: 70
        color: "#0089cf";
        y: 320

        anchors.horizontalCenter: parent.horizontalCenter
    }
}
