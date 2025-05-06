To use the makefile/run cardDeckTest, you must have SFML 2.5.1 installed on your machine.

To install SFML 2.5.1 on linux/WSL, run the following command:
`sudo apt-get install libsfml-dev=2.5.1` (or maybe just `sudo apt-get install libsfml-dev`)

To compile a program that uses the SFML library, project object files must be linked with SFML library files. See Makefile for an example.

Resources:
https://www.sfml-dev.org/tutorials/2.5/
https://georgeblackwell.itch.io/playing-cards-sprite-pack
https://www.dafont.com/minecraft.font

TODO:

ace-low flushes score as if they are ace-high flushes
implement split pots

draw the rank of the winning hand to GUI
draw user's bet this stage to GUI

implement chip floor at chips[player] == 0 and hand over. player should lose the game at this point

check turn order at each stage
strange player betting behavior on blind hands