import QtQuick 2.1
import QtQuick.Window 2.0
import QtQuick.Controls 1.0

Rectangle {

    id: mainWindow;
    width: 1000
    height: 700

    Rectangle {

        id: leftPanel
        height: mainWindow.height - 60
        width: mainWindow.width * 0.4 - 60 - 15
        anchors.left: grid.right
        anchors.leftMargin: 15

        y: 30
        z: 1

        TabView {
            objectName: "tab"
            id: tabLuaCode
            width: parent.width
            height: (parent.height - createCell.height) / 2
            Component.onCompleted: {
                addTab("Game of Life", game_of_life)
                addTab("Ex 2", ex2)
                addTab("Forest Fire", forestFire)
            }

            property string luaCode

            function getText()
            {
                return tabLuaCode.luaCode
            }

            Component {
                id: game_of_life

                TextArea {
                    id: gol
                    objectName: "luaCode"

                    onTextChanged: tabLuaCode.luaCode = gol.text
                    onFocusChanged: tabLuaCode.luaCode = gol.text
                    onVisibleChanged: tabLuaCode.luaCode = gol.text
                    Component.onCompleted: automaton.addSyntaxHighlightingSupport(gol);

                    text: "-- First Example:
        function next_state(cell)
            local nb = 0
            for n in cell.neighbors do
                 if n.old_state == State(0)
                 then
                    nb = nb + 1
                 end
            end
            if nb == 3 then
                cell.state = State(0)
            elseif nb ~= 2 then
                cell.state = State(255)
            end
        end

        function init_cell(cell)
            cell.state = State(255 * math.random(0,1))
        end

        function set_cell(cell)
            cell.state = State(0)
        end"
                }
            }

            Component {
                id: ex2

                TextArea {
                    id: ex2ta
                    objectName: "luaCode"

                    onTextChanged: tabLuaCode.luaCode = ex2ta.text
                    onFocusChanged: tabLuaCode.luaCode = ex2ta.text
                    onVisibleChanged: tabLuaCode.luaCode = ex2ta.text
                    Component.onCompleted: automaton.addSyntaxHighlightingSupport(ex2ta);

                    text: "-- Second example:
        yellow = State(255,255,0)
        red = State(255,0,0)
        orange = State(255,85,0)
        yelrange = State(255,170,0)
        pick_table = { yellow, yelrange, orange, red }

        function next_state(cell)
            local nb = 0
            for n in cell.neighbors do
                if n.old_state == cell.state then
                    nb = nb + 1
                end
            end
            if nb >= 3 then
                set_cell(cell)
            end
        end

        function init_cell(cell)
            cell.state = pick_table[math.random(1,4)]
        end

        function set_cell(cell)
            if cell.state == yellow then
                cell.state = yelrange
            elseif cell.state == yelrange then
                cell.state = orange
            elseif cell.state == orange then
                cell.state = red
            elseif cell.state == red then
                cell.state = yellow
            end
        end"
                }
            }

            Component {
                id: forestFire

                TextArea {
                    id: forestFireta
                    objectName: "luaCode"

                    onTextChanged: tabLuaCode.luaCode = forestFireta.text
                    onFocusChanged: tabLuaCode.luaCode = forestFireta.text
                    onVisibleChanged: tabLuaCode.luaCode = forestFireta.text
                    Component.onCompleted: automaton.addSyntaxHighlightingSupport(forestFireta);

                    text: "red = State(255,0,0)
green = State(0,255,0)
blue = State(255,255,255)
gray = State(125,125,125)
pick_table = { red, green, blue }

-- First Example:
        function next_state(cell)
            local nb = 0
            for n in cell.neighbors do
                 if n.old_state == red
                 then
                    nb = nb + 1
                 end
            end
            if cell.state == green then
                if nb >= 1 then
                    cell.state = red
                end
             elseif cell.state == red then
                cell.state = gray
             elseif cell.state == gray then
                cell.state = blue
             end
        end

       function random(i, j, pct)
              local result = math.random(0., 1.)
              if result > pct then
                  return i
              else
                  return j
              end
        end

        function init_cell(cell)
            cell.state = random(green, blue, 0.75)
        end

        function set_cell(cell)
            cell.state = red
        end"
                }
            }

        }

        TextArea {
            id: luaConsole
            objectName: "luaConsole"
            width: parent.width
            height: (parent.height - createCell.height) / 4 - 5
            anchors.top: tabLuaCode.bottom
            anchors.topMargin: 5
            readOnly: true
            text: automaton.last_lua_error
            textFormat: TextEdit.RichText
        }

        Rectangle {

            id: buttons
            anchors.top:  luaConsole.bottom
            anchors.topMargin: 5
            height: createCell.height

            Button {
                id: createCell
                text: "Create Cells"
                onClicked: automaton.set_grid(automatonHeight.value, automatonWidth.value)
            }

            Button {
                id: callInit
                tooltip: "Call the lua function init_cell(cell)"
                anchors.left: createCell.right
                text: "Call Init"
                onClicked: automaton.call_lua_init_cell(tabLuaCode.getText())
            }

            Button {
                property bool state: true
                id: runCode
                anchors.left: callInit.right
                text : "Run"
                tooltip: "Run/Pause the lua script"
                onClicked: {
                    if (runCode.state) {
                        automaton.run_lua_code(tabLuaCode.getText())
                        runCode.state = false
                        runCode.text = "Pause"
                    } else {
                        automaton.pause()
                        runCode.text = "Run"
                        runCode.state = true
                    }
                }
            }

        }

        Slider {
            width: leftPanel.width
            id: timeInterval
            anchors.top: buttons.bottom
            anchors.topMargin: 5
            maximumValue: 1000
            minimumValue: 0
            stepSize: 10
            value: 1000
            onValueChanged: automaton.setTimeInterval(timeInterval.value);
        }

        Rectangle {
            anchors.top: timeInterval.bottom
            anchors.topMargin: 5
            width: leftPanel.width
            height: 30

            SpinBox {
                id: automatonWidth
                value: 20
                width: 60
            }

            SpinBox {
                id: automatonHeight
                anchors.left: automatonWidth.right
                value: 20
                width: 60
            }

            CheckBox {
                id: showNumbers
                checked: true
                anchors.left: automatonHeight.right
            }
        }
    }

    GridView {
        x: 30
        y: 30

        width: mainWindow.width * 0.6
        height: width
        id: grid
        cellWidth: 80; cellHeight: 80

        model: automaton
        delegate: Rectangle {
            radius: 2
            id : cell
            width: grid.cellWidth - 4
            height: grid.cellHeight - 4
            color: cell_state

            Connections {
                target: grid.model
                onDataChanged: {
                    cell.color = cell_state
                }
            }

            Text {
                text: showNumbers.checked ? index : ""
                font.pointSize: Math.max(30. * cell.width / 80.0, 10)
                color: "gray"
                anchors.centerIn: parent
            }

            MouseArea {
                anchors.fill: parent
                onClicked: automaton.setCellState(tabLuaCode.getText(), index)
                hoverEnabled: true
                onEntered: parent.border.color = "gold"
                onExited:  parent.border.color = "white"
            }
        }

        onCountChanged: {
            grid.cellWidth = grid.width / automaton.columns()
            grid.cellHeight = grid.height / automaton.rows()
        }

        onWidthChanged: {
            grid.cellWidth = Math.min(grid.height / automaton.rows(), grid.width / automaton.columns())
            grid.cellHeight = grid.cellWidth
        }

        onHeightChanged: {
            grid.cellHeight = Math.min(grid.height / automaton.rows(), grid.width / automaton.columns())
            grid.cellWidth = grid.cellHeight
        }
    }
}
