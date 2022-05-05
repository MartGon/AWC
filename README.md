# AWC (Advance Wars Clone)

## Description

AWC is a library which provides common utilities that can be found in most turn-based 2D strategy games. The library's functionality can be extended with Lua scripting, which can be used to define
new types of commands. Most of the core (if not all) is properly tested by the means of unit tests.

## Build Requirements

You will need the following tools:

- C++17 compatible compiler
- CMake 

## How to use

The best way to learn the basics is to check the [example application](https://github.com/MartGon/CLIChess).This is an implementation of the classic game of chess, which can be played through the command line.

## High-Level Goals

- Lua Scripting. Adding new special units shouldn't require to recompile the game
- Networking. It should be playable through the Internet.
- View Independent. The core of the game should be detached from its representation.
- Undo and Redo. Commands or user operations should be completely undoable/redoable

## About

This project started as an Advance Wars clone, hence its name, but ended up turning into a library.
