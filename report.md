1. Special libraries needed to build/compile if personal machines were used
    The graphics for our game were developed with the Simple and Fast Multimedia Library (SFML V2.5.1) in C++. This library must be installed on the system used to compile our program. To install SFML V2.5.1 on a Linux machine, run the following command in the terminal: `sudo apt-get install libsfml-dev=2.5.1`. We were not able to run our project on the Hydra/Tesla machines because we do not have to capability to install SFML on those machines.
2. Instructions on how to build/compile your program
    Once SFML is installed, the program can be compiled using the included Makefile: `make all`. The executable for the program is playGame: `./playGame`.
3. User manual with how the controls/interactions work
    After launching the game, a GUI window should open on your machine. This window contains all the necessary mechanisms for a user to interact with the game. Clicking the 'X' button in the top right corner of the window will close the GUI and kill the program. To play the game, first click the "Play Game" button to enter the main game scene. At this point, the game loop will begin. When it is your turn to bet/fold, select the amount that you want to bet with the bet increment/decrement buttons. These are the two buttons with white triangles/arrows. To bet, press the check/call/raise button. If you want to fold, press the fold button. A betting round continues until all players have either bet the same amount or folded. Then, more cards are placed on the table, or the game ends (depending on the current stage of the game). When a hand ends, the cpu players' cards will be revealed and the winner will be determined. The win message is displayed for 10 seconds, then a new hand automatically starts (you won't be able to close the game during this 10-second period, as the program sleeps at this time). New hands will continue to be dealt indefinitely, until all but one player is out of chips or the user closes the game window.
4. Known bugs. If you document what you know is wrong, and these are minor issues, we will not take off for them. Examples include weird behavior, image mismatches, other last minute issues. If you document more serious issues (seg faults on conclusion, program crashing, etc.) we will give more partial credit.
    
5. Any other information that will inform the instructor and TAs for final grading. If there is special equipment, e.g., NES controllers or a special library, we’ll make an appointment for the demo during exam week
    This project should be compilable from any linux system that has SFML installed, but if an in-person demo of our project is necessary, we will be available during finals week.

    Mason's Time Log:
00:20 - Wrote out a basic rubric milestones-wmw.txt. This document will guide future development work and team meetings.
01:50 - Wrote and tested a basic cardDeck class with the capability to handle multiple decks of cards. The class allows for deck shuffling, card drawing, card discarding, and adding in fresh 52-card decks to the larger game deck. I also setup a basic makefile to facilitate quick testing of all files in our repo.
01:45 - Watched a tutorial on the basics of Unreal Engine (https://www.youtube.com/watch?v=XRmn-EYt8wI&list=PLncmXJdh4q88DFCEVuGpOY3AGQwBvoQnh). I skipped the sections of the video that seemed less relevant at this time, like sections on the asset marketplace and on mesh materials. This tutorial gave me a basic idea of how to build scenes in Unreal, but I still would like to learn more about what is possible with C++ scripting in Unreal before I feel comfortable committing to using Unreal in our project.
01:15 - Watched a tutorial on working with C++ in Unreal studio (https://www.youtube.com/watch?v=nvruYLgjKkk&list=PL-m4pn2uJvXHL5rxdudkhqrSRM5gN43YN). As of now, it looks like building a project in Unreal is less compartmentalized than I had hoped. An alternative, lighter weight C++ library might be a better option. I watched a few tutorials on SFML ( https://www.youtube.com/watch?v=eE3qGNdEpEs & https://www.youtube.com/watch?v=0WlBFTf5vhM), and that library seems to suit our needs better.

01:15 - Installed SFML 2.5.1. Figured out how to compile and link with header files from an external library. Wrote a test program to draw an empty GUI window.
01:00 - Reading SFML documentation and drawing "Hello World" to a GUI window
00:15 - Troubleshooting window scaling issues

00:30 - Solved the window scaling issues (by resetting the window's defined size on each resize event)
01:10 - Setting up a custom class to contain drawable game scenes. Troubleshooting an issue with the virtual sf::Drawable class causing a seg fault when iterating through and drawing elements of a vector of Drawables
01:10 - Solved the issue with sf::Drawables (created objects were going out of scope and being deleted). Continued reformatting graphics code to exist in its own class, with different scenes created by calling class functions.
00:40 - Finished setting up the custom GameGraphics class. Started creating the title screen scene.

01:30 - Finished a first draft of the title screen scene. Added functionality for detecting interactable buttons being pressed.
00:40 - Started creating the main game scene. Set up infrastructure to handle textures and sprites (to draw game assets).
00:20 - Added decoration to the title screen. modified the interactable button handler onClick()
01:30 - Laying out the main game scene. So far, the table cards, player's hand, player's chip count, and betting/folding interface are drawn. Button interaction is not yet present. CPU player hands still need to be drawn, and stylistic changes to the buttons must be made
00:15 - Adding a VectorArray drawable to represent up and down arrows for betting values
00:25 - Finished the VectorArrays. Added visuals representing the hands of cpu players
02:30 - Wrote the first iteration of the GameState class, which stores information about the game of poker being played (ex. each player's hand) and the game state (ex. at what point in the game are we?). Integrated this class into the GameGraphics class to dynamically update the main game scene as the game is played.
00:30 - Fixing bugs in the gameplay loop
00:30 - Adding indicators for folded players
02:30 - Bug fixes (mostly fixing a bug where after the user folds, no cpu players would bet). Adding an end screen to show hidden cpu cards and display the winner.
02:40 - Testing/debugging Jake's code for determining the winning hand at the end of the game. This is mostly working now, with a few outstanding bugs at edge cases.
00:30 - fixing bugs related to betting in Jake's cpu player logic
01:00 - Repairing files that Jake accidentally overwrote by pushing before pulling updates. Fixing a memory leak. Debugging issues with bets larger than a player's number of remaining chips.
00:40 - Bugfixes related to player betting. added a game over condition for when all but 1 player is out of chips.
00:30 - Fixed a bug where straights were being detected on hands of size < 5, causing ai to be overconfident.
1:00 - Fixed a bug where a straight flush was being detected when a hand was really just a flush (of the 7 card in the player's hand, a different 5 were used for the flush than for the straight)
2:00 - code documentation, bug fixing, added an int to store the pot's value at the start of a game stage. This int is used to determine ai bets, preventing runaway raising based on the current pot.
1:15 - More game loop bug fixes, tweaks to ai player



    Meeting Log:
2/18 @ 12:35pm - We talked about our initial ideas for the project before turning in our project proposal. We chose to build a poker game. I decided that I would focus on giving our game a GUI and would research the feasibility of developing our game in a game engine such as Unity, Unreal, or Godot. Jake planned to focus on the backend of the poker game, as he is interested in the game of poker.

3/31 @ 3:00pm - We talked briefly about our ideas for the scope of the project before challenge07 was due. Neither of us made any progress on the project before this point, so at this time we reaffirmed our general areas of focus before we began the work on our compilable tasks for challenge07.

4/3 @ 12:35pm - We met after class and discussed the results of challenge07. Jake and I had built some basic infrastructure for representing playing cards digitally. I told Jake that I intended to change the means by which we would create a GUI from a dedicated game engine to a C++ library called SFML.

4/22 @ 8:00pm - Jake and I met before the due date for the second progress report. I told Jake about my progress learning SFML and creating a class to store the graphical functionality of the game. Jake told me about his research/progress into using machine learning to create our cpu opponents. At this point, my next goal was to create clickable buttons and begin laying out the main game scene in SFML. Jake was to begin building his AI model.

4/29 @ 10:40am - We met to practice our BYODS presentation. We also discussed our current progress. At this point, I had finished implementing buttons and creating a title screen. Jake explained the basic decision tree that the cpu player AI would follow, what parameters his model would control, and (at a high level) how the model-training math would work. We talked a little about connecting Jake's backend code to my frontend GUI, but decided to wait until the next day (lab demo day) to go into detail.

4/30 @ 12:40pm - Jake and I worked together in his lab section, discussing the points at which my frontend and his backend need to interact. I wrote out a few placeholder functions for Jake to implement that could pass input from his model to the game loop that I created. My next steps were to give the main game scene the capability to reflect the state of the game, and Jake's next goal was to connect his AI agent into the game loop. Jake also plans to add a function that will determine which player has won a given hand.

5/6 @ 10:00am - Jake and I worked out some bugs together and decided on which build to show off during demo day. During this meeting and into demo day, we discussed what work remains on the project. I have some UI and game flow bugs, and Jake has some tuning of cpu player parameters to complete.

    Resources Used:
https://www.sfml-dev.org/tutorials/2.5/
https://georgeblackwell.itch.io/playing-cards-sprite-pack
https://www.dafont.com/minecraft.font