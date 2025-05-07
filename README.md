To use the makefile/compile the project, you must have SFML 2.5.1 installed on your machine.

To install SFML 2.5.1 on linux/WSL, run the following command:
`sudo apt-get install libsfml-dev=2.5.1` (or maybe just `sudo apt-get install libsfml-dev`)

To compile a program that uses the SFML library, project object files must be linked with SFML library files. See Makefile for an example.

Resources:
https://www.sfml-dev.org/tutorials/2.5/
https://georgeblackwell.itch.io/playing-cards-sprite-pack
https://www.dafont.com/minecraft.font

TODO:

implement split pots (maybe don't split, instead implement a hierarchy of suits so that one hand always has a higher card than the other)

check turn order at each stage