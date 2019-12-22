
**Name of the project:** Catch and run

**Hardware:** 1 8x8 LED matrix, 1 MAX7219 Driver, 1 LCD, 1 joystick, 1 potentiometer, 2 breadbords, wires, arduino, connector cable.

**Description:**
  The project implements a game where the player has to catch food to gain points (represented as a led on the matrix) and 
  run from the bombs (represented as 4 leds on the matrix in the shape of a square). He can move with the joystick on the 
  last line of the matrix. The food and the bombs move from the first line to the player's line.
  The game will have 3 levels. As the level increse the speed of the food and bombs will increse too and the numbers of bombs
  will grow.
  Every time the game starts, the player receive 3 lives and if he touches a bomb, he will lose a life. When he has 0 lives, 
  the game is over. The game is won when the player finish level 3. 
  The duration of each level is 1 minute. To win level 1, the player has to catch 5 points, for level 2, 10 points and for 
  level 3, 15 points. (In the demo video the duration was reduce to 20 seconds and the points needed to pass the levels to
  3, 5, respectively 6.)
  The highscore will be saved in the EEPROM.
  
  On the lcd, the following will be displayed:
  - a welcome message for 3 seconds;
  - a menu with the options:
    - Play: it starts the game;
    - Score: display the highest score;
    - Settings;
    - Info.
  - while playing: display the score, the level, the time since the level started and the number of lives;
  - when the game is over: it display an message based on the ending of the game.
  
  **Schematic:**
    https://drive.google.com/file/d/17Jmdf1YwmRRz_w2g-2Uttom1LtA4_aUl/view?usp=sharing
      
  **Assembled hardware:**
    https://drive.google.com/drive/u/0/folders/1M_rFhEDl3DsDYk1gDREBmD4OsxKsXGbz
      
  **Video:**
    https://youtu.be/PvMlsCZ_Ir0
    
