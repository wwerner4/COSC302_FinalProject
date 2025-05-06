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