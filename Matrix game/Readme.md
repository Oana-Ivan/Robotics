# Catch and run
---

## Description

  The project implements a game where the player has to catch food to gain points (represented as a led on the matrix) and 
  run from the bombs (represented as 4 leds on the matrix in the shape of a square). He can move with the joystick on the 
  last line of the matrix. The food and the bombs move from the first line to the player's line.
  
  The game will have 3 levels. As the level increse, the speed of the food and bombs will increase too and the numbers of bombs
  will grow.
  
  Every time the game starts, the player receives 3 lives and if he touches a bomb, he will lose a life. When he has 0 lives, 
  the game is over. The game is won when the player finishes level 3. 
  
  The duration of each level is 1 minute. To win level 1, the player has to catch 5 points, for level 2, 10 points and for 
  level 3, 15 points. (In the demo video the duration was reduced to 20 seconds and the points needed to pass the levels to
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
  - when the game is over: it displays a message based on the ending of the game.

## How to play

- Choose the option "Play" on the menu pressing the button of the joystick
- Move with the joystick to:
      
     - catch food and gain points;
      
     - run from the bombs and don't lose lives.
- Collect:
      
     - 5 points for level 1;
      
     - 10 points for level 2;
      
     - 15 points for level 3.

## Schematic

   ![schema](https://user-images.githubusercontent.com/49486605/75517086-8a035800-5a06-11ea-9dcf-9ce1a6f04d5a.png)

## Video
  The video can be found [here](https://youtu.be/PvMlsCZ_Ir0).
 
## Hardware

- 1 8x8 LED matrix;
- 1 MAX7219 Driver;
- 1 LCD, 1 joystick; 
- 1 potentiometer;
- 2 breadbords; 
- wires;
- arduino; 
- connector cable.

## Assembled hardware 

![hardware](https://user-images.githubusercontent.com/49486605/75516949-398bfa80-5a06-11ea-942c-c485edc8116a.jpg)
 

    
