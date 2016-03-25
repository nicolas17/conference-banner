/*
 * Copyright (c) 2016 Nicolás Alvarez
 *
 * Licensed under the General Public License version 3,
 * or (at your option) any later version. See the LICENSE
 * file for details.
 */

import QtQuick 2.3

Rectangle {
    id: slide1
    width: 1280
    height: 720
    color: "#231f20"

    //width: 1366
    //height: width/(16/9)

    Image {
        id: bkgnd
        source: "patagonia-bg.png"
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
    }
    Image {
        id: logo
        source: "foss4g-ar-1.png"
        anchors.verticalCenter: parent.verticalCenter
        x: 13+15+15
        width: 560
        height: 560
    }
    Item {
        id: topText
        y: 200
        anchors.left: logo.right
        anchors.leftMargin: 90
        anchors.right: parent.right

        Text {
            text: "Conferencia de Geomática Libre"
            font.family: fjalla.name
            font.pixelSize: 88
            color: "#fdba12";
            wrapMode: Text.WordWrap

            anchors.left: parent.left
            anchors.right: parent.right
        }
        Text {
            text: "5 al 9 de abril"
            font.family: fjalla.name
            font.pixelSize: 65
            color: "#0089cf";
            y: 225

            anchors.left: parent.left
            anchors.right: parent.right
        }
        Text {
            text: "Instituto Geográfico Nacional"
            font.family: fjalla.name
            font.pixelSize: 48
            color: "#0089cf";
            y: 320

            anchors.left: parent.left
            anchors.right: parent.right
        }
    }
}
