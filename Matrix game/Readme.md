
**Name of the project:** Matrix game

**Hardware:** 1 8x8 LED matrix,1 MAX7219 Driver, 1 LCD, 1 joystick, 1 buzzer, wires, arduino, connector cable.

**Description:**
  The project implements a game where the player has to catch food to gain points (represented as a led on the matrix) and 
  run from the bombs (represented as 4 leds on the matrix in the shape of an square). He can move with the joystick on the 
  last line of the matrix. The food and the bombs moves from the first line to the player's line.
  The game will have 3 levels. As the level increse the speed of the food and bombs will increse too and the numbers of bombs
  will grow.
  Every time the game starts, the player receive 3 lives and if he touches a bomb, he will lose a life. When he has 0 lives, 
  the game is over and the buzzer make a sound. 
  The game is won when the player finish level 3. 
  The highscore will be saved in the EEPROM.
  
  On the lcd, the following will be displayed:
  - a welcome message for 3 seconds;
  - a menu with the options:
    - Play: it starts the game;
    - Score: display the higher score;
    - Settings: sets the lcd settings and enters the player name;
    - Info.
  - while playing: display the number of lives and the score
  - when the game is over: it display an message  and after it lets the player the choice to try again.
  
  **Schematic:**
    https://drive.google.com/file/d/17Jmdf1YwmRRz_w2g-2Uttom1LtA4_aUl/view?usp=sharing
