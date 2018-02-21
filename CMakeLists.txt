cmake_minimum_required(VERSION 3.9)
project(Uni_Chess_Project C)

set(CMAKE_C_STANDARD 99)

add_executable(Uni_Chess_Project main.c Auxiliary.c Auxiliary.h ChessBoard.c ChessBoard.h ChessParser.c ChessParser.h Minimax.c Minimax.h ChessGUI.c ChessGUI.h ChessGameLogic.c ChessGameLogic.h ChessGameSettings.c ChessGameSettings.h ChessConsoleUI.c ChessConsoleUI.h PossibleMoveLogic.c PossibleMoveLogic.h)