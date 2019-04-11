/*
 * ball.c
 *
 *  Created on: Mar 9, 2019
 *      Author: EmilAliyev
 */

//Standard includes
#include <math.h>
#include <stdio.h>

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

//Header include
#include "ball.h"

//Custom includes
#include "acceleration.h"
#include "collision.h"

#define TRUE 1
#define FALSE 0

#define ENEMY_BALL_ACCELERATION 5

#define SPEED_LIMIT 10

#define KNOCKBACK_MULTIPLIER 3

//Test if two balls are colliding
static int collision_test(Ball ball1, Ball ball2)
{
    //Define one hit box for each ball
    HitBox box1;
    HitBox box2;

    box1.xmin = ball1.x - ball1.radius;
    box1.xmax = ball1.x + ball1.radius;
    box1.ymin = ball1.y - ball1.radius;
    box1.ymax = ball1.y + ball1.radius;

    box2.xmin = ball2.x - ball2.radius;
    box2.xmax = ball2.x + ball2.radius;
    box2.ymin = ball2.y - ball2.radius;
    box2.ymax = ball2.y + ball2.radius;

    if(hitboxOverlap(box1, box2))
        return TRUE;

    return FALSE;
}

//When two balls collide
static void on_collide(Ball *ball1, Ball *ball2)
{
    //m1v1 = m2v2
    //v1 = v2



    //Swap velocities and negate them
    int xvelb1 = ball1->xvel;
    int yvelb1 = ball1->yvel;

    int xvelb2 = ball2->xvel;
    int yvelb2 = ball2->yvel;

    ball1->xvel = xvelb2 * KNOCKBACK_MULTIPLIER;
    ball1->yvel = yvelb2 * KNOCKBACK_MULTIPLIER;

    ball2->xvel = xvelb1 * 4;
    ball2->yvel = yvelb1 * 4;


}

//Calculate enemy ball's velocity
static void enemy_velocity_calc(Ball *ball, Ball *enemyball)
{
    //Enemy ball follows you

    //Get position vector of your ball relative to enemy
    float xpos = ball->x - enemyball->x;
    float ypos = ball->y - enemyball->y;

    //Get magnitude
    float mag = sqrt(pow(xpos, 2) + pow(ypos, 2));

    //Turn position vector into direction vector
    xpos = xpos/mag;
    ypos = ypos/mag;

    //Velocity = direction * speed
    enemyball->xvel = xpos * ENEMY_BALL_ACCELERATION;
    enemyball->yvel = ypos * ENEMY_BALL_ACCELERATION;

}

//Initialize a new ball
Ball create_ball(int startx, int starty, int radius, int color)
{
    Ball ball;
    ball.x = startx;
    ball.y = starty;
    ball.radius = radius;
    ball.color = color;
    ball.xvel = 0;
    ball.yvel = 0;

    return ball;
}

//Calculate velocity of own ball
static void player_velocity_calc(Ball *ball)
{
    ball->xvel = getYAcceleration()/5;
    ball->yvel = getXAcceleration()/5;
}

//Called on ball continuously
static void player_ball_loop(Ball *ball)
{
    //Clear the ball
    clearBall(*ball);

    //Change ball's position according to the velocity
    ball->x += ball->xvel;
    ball->y += ball->yvel;

    //Redraw the ball
    drawBall(*ball);
}

//Called on enemy ball continuously
static void enemy_ball_loop(Ball *ball, Ball *enemyball)
{
    clearBall(*enemyball);

    //Move enemy ball
    enemyball->x += enemyball->xvel;
    enemyball->y += enemyball->yvel;

    //Redraw the ball
    drawBall(*enemyball);

}

//Calculate velocities of balls in event of no collision
void noCollisionVelocityCalc(Ball *ball, Ball *enemyball)
{
    //Calculate velocity of own ball
    player_velocity_calc(ball);

    //Calculate velocity of enemy ball
    enemy_velocity_calc(ball, enemyball);
}

static void enemy_calculate_acceleration(Ball *ball, Ball *enemyball)
{
    //Enemy ball follows you

    //Get position vector of your ball relative to enemy
    float xpos = ball->x - enemyball->x;
    float ypos = ball->y - enemyball->y;

    //Get magnitude
    float mag = sqrt(pow(xpos, 2) + pow(ypos, 2));

    //Turn position vector into direction vector
    xpos = xpos/mag;
    ypos = ypos/mag;

    //Acceleration = |Acceleration| * direction
    enemyball->xacc = xpos * ENEMY_BALL_ACCELERATION;
    enemyball->yacc = ypos * ENEMY_BALL_ACCELERATION;
}

static void self_calculate_acceleration(Ball *ball)
{
    ball->xacc = getYAcceleration()/5;
    ball->yacc = getXAcceleration()/5;
}

//Calculate acceleration for each ball
static void calculate_acceleration(Ball *ball, Ball *enemyball)
{
    //Calculate own acceleration
    self_calculate_acceleration(ball);

    //Calculate enemy acceleration
    enemy_calculate_acceleration(ball, enemyball);
}

//Called on balls continuously
void ball_loop(Ball *ball, Ball *enemyball)
{
    /*
     * 1. Calculate acceleration for each ball
     * 2. Check for collision. If true, calculate velocities based on collision.
     * 3. Update velocities based on accelerations
     * 4. Update positions based on velocities
     *
     */

    calculate_acceleration(ball, enemyball);

    //Check for collision
    if(collision_test(*ball, *enemyball))
        on_collide(ball, enemyball);

    else
    {
        noCollisionVelocityCalc(ball, enemyball);
    }

    player_ball_loop(ball);
    enemy_ball_loop(ball, enemyball);
}

//Clear the ball
void drawBall(Ball ball)
{
    drawCircle(ball.x, ball.y, ball.radius, ball.color);
}

//Draw the ball
void clearBall(Ball ball)
{
    drawCircle(ball.x, ball.y, ball.radius, BLACK);
}

//Check if (all of) ball is out of bounds
int ball_outofbounds(Ball ball, int xmin, int xmax, int ymin, int ymax)
{
    if((ball.x + ball.radius) < xmin)
        return TRUE;

    if((ball.x - ball.radius) > xmax)
        return TRUE;

    if((ball.y + ball.radius) < ymin)
        return TRUE;

    if((ball.y - ball.radius) > ymax)
        return TRUE;

    return FALSE;
}
