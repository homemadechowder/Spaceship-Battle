/*
 * ball.h
 *
 *  Created on: Mar 9, 2019
 *      Author: EmilAliyev
 */

#ifndef BALL_H_
#define BALL_H_

typedef struct Ball
{
    float x; //The ball's x coordinate
    float y; //The ball's y coordinate
    int radius; //The ball's radius
    int color; //The ball's color
    float xvel; //The ball's x velocity
    float yvel; //The ball's y velocity
    float xacc; //The ball's x acceleration
    float yacc; //The ball's y acceleration
}Ball;

Ball create_ball(int startx, int starty, int radius, int color);

void ball_loop(Ball *ball, Ball *enemyball);

void clearBall(Ball ball);

void drawBall(Ball ball);

int ball_outofbounds(Ball ball, int xmin, int xmax, int ymin, int ymax);

#endif /* BALL_H_ */
