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
    function escapeHTML(s) {
        return s.replace(/&/g,"&amp;")
                .replace(/</g,"&lt;");
    }

    id: slide2
    width: 1280
    height: 720
    color: "#231f20"

    //width: 1366
    //height: width/(16/9)

    ListModel {
        id: talkModel1
        ListElement {
            room: "Aula 1"
            name: "Leaflet Avanzado - Cómo hacer plugins"
        }
        ListElement {
            room: "Aula 2"
            name: "GeoNetwork Avanzado"
        }
        ListElement {
            room: "Aula 3"
            name: "QGIS II - Creación de datos geoespaciales"
        }
    }
    ListModel {
        id: talkModel2
        ListElement {
            room: "Aula 1"
            name: "Geoinformática aplicada a la cartografía"
        }
        ListElement {
            room: "Aula 2"
            name: "Análisis espacial con R"
        }
        ListElement {
            room: "Aula 3"
            name: "QGIS III - Geoprocesos"
        }
    }
    ListModel {
        id: talkModel3
        ListElement {
            room: "Sala 1"
            name: "Casos de uso de FOSS4G en el sector azucarero Colombiano"
        }
        ListElement {
            room: "Sala 2"
            name: "Caracterización ambiental de la zona periurbana de Villa María para la conformación de Indicadores Ambientales"
        }
    }
    ListModel {
        id: talkModel4
        ListElement {
            room: "Sala 1"
            name: "IndoorGML, pros y contras"
        }
        ListElement {
            room: "Sala 2"
            name: "gvSIG Online, solución integral para Infraestructuras de Datos Espaciales"
        }
    }

    Item {
        x: 15
        y: 15
        Text {
            id: txtNow
            color: "#fdba12"
            text: "Ahora "
            font.pixelSize: 50
            font.family: fjalla.name
        }
        Text {
            id: txtTime
            color: "white"
            text: "15:00 \u2013 15:30"
            font.pixelSize: 38
            font.family: fjalla.name

            anchors.left: txtNow.right
            anchors.baseline: txtNow.baseline
        }

        ListView {
            id: talkListView
            y:25+12+83
            width: 800
            height: 600
            model: currentTalksModel
            highlightFollowsCurrentItem: true
            highlightMoveDuration: 1000
            highlightMoveVelocity: -1
            interactive: false // disable mouse flicking

            delegate: Item {
                height: txt.contentHeight+30
                Text {
                    id: txt
                    text: "<font color='white'>"+escapeHTML(room)+': </font>'+escapeHTML(title)
                    font.family: fjalla.name
                    font.pixelSize: 40
                    color: "#0089cf";
                    wrapMode: Text.WordWrap
                    textFormat: Text.StyledText
                    width: 800
                    //renderType: Text.NativeRendering
                    lineHeight: 1.2
                }
            }
        }
    }

    Item {
        x: 830+15
        y: 15
        Text {
            id: txtNext
            color: "#fdba12"
            text: "Próximo "
            font.pixelSize: 32
            font.family: fjalla.name
        }
        Text {
            id: txtNextTime
            color: "white"
            text: "15:30 \u2013 16:00"
            font.pixelSize: 24
            font.family: fjalla.name

            anchors.left: txtNext.right
            anchors.baseline: txtNext.baseline
        }

        ListView {
            id: nextTalkListView
            y:25+12+83
            width: 400
            height: 600
            model: talkModel4
            highlightFollowsCurrentItem: true
            highlightMoveDuration: 1000
            highlightMoveVelocity: -1
            interactive: false // disable mouse flicking

            delegate: Item {
                height: txt.contentHeight+30
                Text {
                    id: txt
                    text: "<font color='white'>"+escapeHTML(room)+': </font>'+escapeHTML(name)
                    font.family: fjalla.name
                    font.pixelSize: 25
                    color: "#0089cf";
                    wrapMode: Text.WordWrap
                    textFormat: Text.StyledText
                    width: nextTalkListView.width
                    //renderType: Text.NativeRendering
                }
            }
        }
    }
}
