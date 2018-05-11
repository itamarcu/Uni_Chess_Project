CC = gcc
OBJS = Button.o Auxiliary.o ChessBoard.o ChessConsoleUI.o ChessGameLogic.o ChessGameSettings.o ChessGUI.o ChessParser.o \
FileOperations.o HandleAndDrawGame.o Minimax.o OptionsButtons.o PossibleMoveLogic.o OptionsButtons.o PossibleMoveLogic.o\
 SlotOptions.o Undo.o Widget.o Window.o main.o
 
EXEC = myprog
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(SDL_LIB) -o $@
Button.o: Button.c Auxiliary.h Button.h ChessBoard.h ChessConsoleUI.h ChessGameLogic.h ChessGameSettings.h ChessGUI.h ChessParser.h FileOperations.h GUIStructs.h HandleAndDrawGame.h Minimax.h OptionsButtons.h PossibleMoveLogic.h SlotOptions.h Undo.h Widget.h Window.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Auxiliary.o: Auxiliary.c Auxiliary.h Button.h ChessBoard.h ChessConsoleUI.h ChessGameLogic.h ChessGameSettings.h ChessGUI.h ChessParser.h FileOperations.h GUIStructs.h HandleAndDrawGame.h Minimax.h OptionsButtons.h PossibleMoveLogic.h SlotOptions.h Undo.h Widget.h Window.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessBoard.o: ChessBoard.c Auxiliary.h Button.h ChessBoard.h ChessConsoleUI.h ChessGameLogic.h ChessGameSettings.h ChessGUI.h ChessParser.h FileOperations.h GUIStructs.h HandleAndDrawGame.h Minimax.h OptionsButtons.h PossibleMoveLogic.h SlotOptions.h Undo.h Widget.h Window.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessConsoleUI.o: ChessConsoleUI.c Auxiliary.h Button.h ChessBoard.h ChessConsoleUI.h ChessGameLogic.h ChessGameSettings.h ChessGUI.h ChessParser.h FileOperations.h GUIStructs.h HandleAndDrawGame.h Minimax.h OptionsButtons.h PossibleMoveLogic.h SlotOptions.h Undo.h Widget.h Window.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessGameLogic.o: ChessGameLogic.c Auxiliary.h Button.h ChessBoard.h ChessConsoleUI.h ChessGameLogic.h ChessGameSettings.h ChessGUI.h ChessParser.h FileOperations.h GUIStructs.h HandleAndDrawGame.h Minimax.h OptionsButtons.h PossibleMoveLogic.h SlotOptions.h Undo.h Widget.h Window.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessGameSettings.o: ChessGameSettings.c Auxiliary.h Button.h ChessBoard.h ChessConsoleUI.h ChessGameLogic.h ChessGameSettings.h ChessGUI.h ChessParser.h FileOperations.h GUIStructs.h HandleAndDrawGame.h Minimax.h OptionsButtons.h PossibleMoveLogic.h SlotOptions.h Undo.h Widget.h Window.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessGUI.o: ChessGUI.c Auxiliary.h Button.h ChessBoard.h ChessConsoleUI.h ChessGameLogic.h ChessGameSettings.h ChessGUI.h ChessParser.h FileOperations.h GUIStructs.h HandleAndDrawGame.h Minimax.h OptionsButtons.h PossibleMoveLogic.h SlotOptions.h Undo.h Widget.h Window.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessParser.o: ChessParser.c Auxiliary.h Button.h ChessBoard.h ChessConsoleUI.h ChessGameLogic.h ChessGameSettings.h ChessGUI.h ChessParser.h FileOperations.h GUIStructs.h HandleAndDrawGame.h Minimax.h OptionsButtons.h PossibleMoveLogic.h SlotOptions.h Undo.h Widget.h Window.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
FileOperations.o: FileOperations.c Auxiliary.h Button.h ChessBoard.h ChessConsoleUI.h ChessGameLogic.h ChessGameSettings.h ChessGUI.h ChessParser.h FileOperations.h GUIStructs.h HandleAndDrawGame.h Minimax.h OptionsButtons.h PossibleMoveLogic.h SlotOptions.h Undo.h Widget.h Window.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
HandleAndDrawGame.o: HandleAndDrawGame.c Auxiliary.h Button.h ChessBoard.h ChessConsoleUI.h ChessGameLogic.h ChessGameSettings.h ChessGUI.h ChessParser.h FileOperations.h GUIStructs.h HandleAndDrawGame.h Minimax.h OptionsButtons.h PossibleMoveLogic.h SlotOptions.h Undo.h Widget.h Window.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Minimax.o: Minimax.c Auxiliary.h Button.h ChessBoard.h ChessConsoleUI.h ChessGameLogic.h ChessGameSettings.h ChessGUI.h ChessParser.h FileOperations.h GUIStructs.h HandleAndDrawGame.h Minimax.h OptionsButtons.h PossibleMoveLogic.h SlotOptions.h Undo.h Widget.h Window.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
OptionsButtons.o: OptionsButtons.c Auxiliary.h Button.h ChessBoard.h ChessConsoleUI.h ChessGameLogic.h ChessGameSettings.h ChessGUI.h ChessParser.h FileOperations.h GUIStructs.h HandleAndDrawGame.h Minimax.h OptionsButtons.h PossibleMoveLogic.h SlotOptions.h Undo.h Widget.h Window.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
PossibleMoveLogic.o: PossibleMoveLogic.c Auxiliary.h Button.h ChessBoard.h ChessConsoleUI.h ChessGameLogic.h ChessGameSettings.h ChessGUI.h ChessParser.h FileOperations.h GUIStructs.h HandleAndDrawGame.h Minimax.h OptionsButtons.h PossibleMoveLogic.h SlotOptions.h Undo.h Widget.h Window.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SlotOptions.o: SlotOptions.c Auxiliary.h Button.h ChessBoard.h ChessConsoleUI.h ChessGameLogic.h ChessGameSettings.h ChessGUI.h ChessParser.h FileOperations.h GUIStructs.h HandleAndDrawGame.h Minimax.h OptionsButtons.h PossibleMoveLogic.h SlotOptions.h Undo.h Widget.h Window.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Undo.o: Undo.c Auxiliary.h Button.h ChessBoard.h ChessConsoleUI.h ChessGameLogic.h ChessGameSettings.h ChessGUI.h ChessParser.h FileOperations.h GUIStructs.h HandleAndDrawGame.h Minimax.h OptionsButtons.h PossibleMoveLogic.h SlotOptions.h Undo.h Widget.h Window.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Widget.o: Widget.c Auxiliary.h Button.h ChessBoard.h ChessConsoleUI.h ChessGameLogic.h ChessGameSettings.h ChessGUI.h ChessParser.h FileOperations.h GUIStructs.h HandleAndDrawGame.h Minimax.h OptionsButtons.h PossibleMoveLogic.h SlotOptions.h Undo.h Widget.h Window.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Window.o: Window.c Auxiliary.h Button.h ChessBoard.h ChessConsoleUI.h ChessGameLogic.h ChessGameSettings.h ChessGUI.h ChessParser.h FileOperations.h GUIStructs.h HandleAndDrawGame.h Minimax.h OptionsButtons.h PossibleMoveLogic.h SlotOptions.h Undo.h Widget.h Window.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
main.o: main.c Auxiliary.h Button.h ChessBoard.h ChessConsoleUI.h ChessGameLogic.h ChessGameSettings.h ChessGUI.h ChessParser.h FileOperations.h GUIStructs.h HandleAndDrawGame.h Minimax.h OptionsButtons.h PossibleMoveLogic.h SlotOptions.h Undo.h Widget.h Window.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
clean:
	rm -f *.o $(EXEC) $(UNIT_TESTS)
