import QtQuick 2.9
import CubeItem 1.0
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {
    visible: true
    width: 640
    height: 480
    Rectangle{
        id: cube
        anchors.fill: parent
        anchors.margins: 10
        color: Qt.rgba(139/255,224/255,1,1)
        Image {
            id: background
            source: ""
            anchors.fill: parent
        }
        CubeItem{
            id: cubeItem
            texturePath: "qrc:/texture0.jpg"
            anchors.fill: parent
        }
    }
    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        onAccepted: {
            console.log("File selected: " + fileDialog.fileUrls)
            texturePathEdit.text = fileDialog.fileUrl
        }
    }

    Rectangle{
        anchors.bottom: cube.bottom
        anchors.left: cube.left
        anchors.right: cube.right
        anchors.margins: 10
        height: cube.height * 0.33
        color: Qt.rgba(1,1,1,0.66)
        border.color: Qt.rgba(1,1,1,1)
        radius: 10
        ColumnLayout{
            anchors.fill: parent
            anchors.margins: 10
            RowLayout{
                Button{
                    id:changeTextureBtn
                    text: "选择纹理"
                    onClicked: {
                        fileDialog.open()
                    }
                }

                Rectangle{
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter
                    height: changeTextureBtn.height
                    TextField{
                        id: texturePathEdit
                        anchors.fill: parent
                        text: ""
                        selectByMouse: true
                        placeholderText: "纹理URL"
                    }
                }

                Button{
                    id:applyTextureBtn
                    text: "应用纹理"
                    onClicked: {
                        cubeItem.setTexture(texturePathEdit.text)
                    }
                }
            }
            RowLayout{
                FileDialog {
                    id: backgroundDialog
                    title: "Please choose a file"
                    folder: shortcuts.home
                    onAccepted: {
                        console.log("File selected: " + fileDialog.fileUrls)
                        backgroundUrl.text = backgroundDialog.fileUrl
                    }
                    onRejected: {
                        console.log("Canceled")
                    }
                }
                Button{
                    id:changeBackgroundBtn
                    text: "选择背景"
                    onClicked: {
                        backgroundDialog.open()
                    }
                }
                Rectangle{
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter
                    height: changeBackgroundBtn.height
                    TextField{
                        id: backgroundUrl
                        focus: true
                        selectByMouse: true
                        anchors.fill: parent
                        clip: true
                        placeholderText: "背景URL"
                    }
                }
                Button{
                    id: applayBackgroundBtn
                    text:"应用"
                    onClicked: {
                        background.source = backgroundUrl.text
                    }
                }
            }
        }
    }
}
