CC = gcc
OBJS = Button.o Auxiliary.o ChessBoard.o ChessConsoleUI.o ChessGameLogic.o ChessGameSettings.o ChessGUI.o ChessParser.o \
FileOperations.o HandleAndDrawGame.o Minimax.o OptionsButtons.o PossibleMoveLogic.o SlotOptions.o Undo.o Widget.o \
 Window.o main.o

EXEC = chessprog
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(SDL_LIB) -o $@
Button.o: Button.c Button.h GUIStructs.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Auxiliary.o: Auxiliary.c Auxiliary.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessBoard.o: ChessBoard.c ChessBoard.h Auxiliary.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessConsoleUI.o: ChessConsoleUI.c ChessConsoleUI.h ChessParser.h ChessGameLogic.h Minimax.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessGameLogic.o: ChessGameLogic.c ChessGameLogic.h PossibleMoveLogic.h ChessGameSettings.h Minimax.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessGameSettings.o: ChessGameSettings.c ChessGameSettings.h ChessBoard.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessGUI.o: ChessGUI.c ChessGUI.h ChessGameLogic.h Window.h Button.h OptionsButtons.h SlotOptions.h Undo.h HandleAndDrawGame.h FileOperations.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessParser.o: ChessParser.c ChessParser.h Auxiliary.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
FileOperations.o: FileOperations.c FileOperations.h ChessGameSettings.h ChessBoard.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
HandleAndDrawGame.o: HandleAndDrawGame.c HandleAndDrawGame.h GUIStructs.h ChessGameSettings.h Window.h ChessGameLogic.h Minimax.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
main.o: main.c ChessConsoleUI.h ChessGUI.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Minimax.o: Minimax.c Minimax.h ChessGameSettings.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
OptionsButtons.o: OptionsButtons.c OptionsButtons.h GUIStructs.h Button.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
PossibleMoveLogic.o: PossibleMoveLogic.c PossibleMoveLogic.h ChessGameSettings.h ChessGameLogic.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SlotOptions.o: SlotOptions.c SlotOptions.h GUIStructs.h ChessGameSettings.h Window.h FileOperations.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Undo.o: Undo.c Undo.h ChessGameSettings.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Widget.o: Widget.c Widget.h GUIStructs.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Window.o: Window.c Window.h GUIStructs.h Widget.h Button.h Auxiliary.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
clean:
	rm -f *.o $(EXEC) $(UNIT_TESTS)
