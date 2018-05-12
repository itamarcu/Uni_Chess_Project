This is the chess game we've made. It supports a full console/graphical game, including a minimax algorithm and load/save/undo.

When we ran tests in the nova server through MobaXTerm, we encountered one bug we couldn't solve - the application kept crashing after showing message boxes, due to some SDL/X11 issue.
Since we didn't manage to fix this in time, we added a workaround - you can run the program with the "-m" mode and it will avoid using message boxes.
The total number of slots can be redefined inside ChessGui.h, in the "define" section.

We did not do the bonus (concurrency and threads), but we did highly optimize the minimax algorithm, so it should run even Expert difficulty mode very fast (~2 seconds worst case).

Usage:

    <program_name> [mode] [debug-flag]


Modes:

    -c -console                    :  console (default, if no flag)
    -g -graphic                    :  graphics, including message boxes
    -m --graphic-no-message-boxes  :  graphics, but without message boxes


Debug-flag:

    -d --debug                     :  debug mode, adds a few extra prints


Extras:

    * While holding Ctrl and clicking, you will play as if you were the computer (with the current difficulty).
      Holding Shift will do the same but twice (to prevent you switching place with computer, or enable it).
    * This can be simulated in console mode as well, using the command "auto".
    * Console mode has another useful function: instead of writing "move <1,B> to <3,D>" you can
      write "mov b1 d3" or "mov 1b 3d".
