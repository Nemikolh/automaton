Cellular automaton
==================

Cellular automaton that can be easily edited with a Qt interface.
The program has been made in a very short time.
The goal was to get a functional program, providing a simple interface to interact with the automaton.
It is intented for learning purposes, thus I won't spent a lot of time to improve it.
As a side note, you'll find bits of code on how to use `luabind` and how to use model with QML.

Install
=======

You need the following library in order to compile this project:
 * Qt 5.2.1 compiled with xcb support
 * Lua 5.2
 * [luabind](https://github.com/Oberon00/luabind)


Usage
=====

When running the application, you will see three example of automaton. The lua code consist of three function:
 * `set_cell(cell)` which is called when you click on a cell
 * `next_state(cell)` which is called whenever we made the computation to the next step.
 * `init_cell(cell)` which is called on all the cells when you click on the "Call init" button.
