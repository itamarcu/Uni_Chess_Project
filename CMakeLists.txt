cmake_minimum_required(VERSION 3.9)
project(Uni_Chess_Project C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(Uni_Chess_Project main.c Auxiliary.c Auxiliary.h ChessBoard.c
        ChessBoard.h ChessParser.c ChessParser.h Minimax.c Minimax.h ChessGUI.c
        ChessGUI.h ChessGameLogic.c ChessGameLogic.h ChessGameSettings.c
        ChessGameSettings.h ChessConsoleUI.c ChessConsoleUI.h PossibleMoveLogic.c
        PossibleMoveLogic.h Button.c Button.h Widget.c Widget.h Window.c Window.h
        ChessMainMenuWindow.c ChessMainMenuWindow.h GUIStructs.h
        ChessGameModeWindow.c ChessGameModeWindow.h)

set(SOURCE_FILES main.c)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -lmingw32")

include_directories(${PROJECT_SOURCE_DIR}/include/SDL2)
link_directories(${PROJECT_SOURCE_DIR}/lib)

target_link_libraries(Uni_Chess_Project SDL2main SDL2)


