/*
 * game.c
 *
 *  Created on: Mar 9, 2019
 *      Author: EmilAliyev
 */

//Header include
#include "game.h"

//Standard library includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "rom.h"
#include "rom_map.h"
#include "interrupt.h"
#include "prcm.h"
#include "utils.h"
#include "uart.h"
#include "spi.h"

//Common interface includes
#include "i2c_if.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1351.h"
#include "glcdfont.h"
#include "test.h"

//Custom includes
#include "ball.h"
#include "network.h"
#include "oled_display.h"


#define ACC_X_DIVISOR 10
#define ACC_Y_DIVISOR 1000

//Ball definitions
#define BALL_X_START 48
#define BALL_Y_START 64
#define BALL_RADIUS 4
#define BALL_COLOR BLUE

#define ENEMY_BALL_X_START 80
#define ENEMY_BALL_Y_START 64
#define ENEMY_BALL_RADIUS 4
#define ENEMY_BALL_COLOR RED

//Boundaries
#define X_MIN_BOUNDARY 0
#define X_MAX_BOUNDARY 128
#define Y_MIN_BOUNDARY 0
#define Y_MAX_BOUNDARY 128

//Victory/defeat
#define VICTORY 1
#define DEFEAT 0

//Title
#define TITLE "Spaceship Battle"
#define INSTRUC "Survive as Long as Possible!"

//Victory/defeat messages
#define VICTORY_MESSAGE "You win!"
#define DEFEAT_MESSAGE "You lose!"

//Global variables
int ballx;
int bally;

//The ball
Ball ball;

//The enemy ball
Ball enemyball;

//Display on win
void winScreen()
{
    endGameMessage(VICTORY_MESSAGE);
    sendMessage(VICTORY_MESSAGE);
}

//Display on loss
void loseScreen()
{
    endGameMessage(DEFEAT_MESSAGE);
    sendMessage(DEFEAT_MESSAGE);
}

//Display title screen
static void titleScreen()
{
    fillScreen(WHITE);

    titleScreenMessage(TITLE);
    titleScreenMessage(INTRUC);
}


//Called at start of game
void game_start()
{

    //Create the ball
    ball = create_ball(BALL_X_START, BALL_Y_START, BALL_RADIUS, BALL_COLOR);

    //Create enemy ball
    enemyball = create_ball(ENEMY_BALL_X_START, ENEMY_BALL_Y_START, ENEMY_BALL_RADIUS, ENEMY_BALL_COLOR);

    titleScreen();

    //Draw balls
    drawBall(ball);
    drawBall(enemyball);
}

//Called continuously
void game_loop()
{
    //Update balls
    ball_loop(&ball, &enemyball);

    //You lose if your ball goes out of bounds
    if(ball_outofbounds(ball, X_MIN_BOUNDARY, X_MAX_BOUNDARY, Y_MIN_BOUNDARY, Y_MAX_BOUNDARY))
        game_end(DEFEAT);

    //You win if the enemy ball goes out of bounds
    if(ball_outofbounds(enemyball, X_MIN_BOUNDARY, X_MAX_BOUNDARY, Y_MIN_BOUNDARY, Y_MAX_BOUNDARY))
        game_end(VICTORY);

}

//Called at end of game
void game_end(int result)
{
    if(result == DEFEAT)
        loseScreen();

    if(result == VICTORY)
        winScreen();


    //Do nothing
    while(1)
    {

    }
}
