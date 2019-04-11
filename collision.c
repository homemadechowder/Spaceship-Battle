/*
 * collision.c
 *
 *  Created on: Mar 11, 2019
 *      Author: EmilAliyev
 */


#include "collision.h"

#define TRUE 1
#define FALSE 0

//Check if coordinates x, y are inside hit box
static int insideBox(int x, int y, HitBox box)
{
    //If x is not within bounds
    if(!((x > box.xmin) && (x < box.xmax)))
        return FALSE;

    //If y is not within bounds
    if(!((y > box.ymin) && (x < box.ymax)))
        return FALSE;

    return TRUE;
}

//Check if any corners of box 1 are inside box 2
static int hitboxCornerTest(HitBox box1, HitBox box2)
{
    //Lower-left corner
    if(insideBox(box1.xmin, box1.ymin, box2))
        return TRUE;

    //Upper-left corner
    if(insideBox(box1.xmin, box1.ymax, box2))
        return TRUE;

    //Upper-right corner
    if(insideBox(box1.xmax, box1.ymax, box2))
        return TRUE;

    //Lower-right corner
    if(insideBox(box1.xmax, box1.ymin, box2))
        return TRUE;

    return FALSE;
}

//Check if hit boxes overlap
int hitboxOverlap(HitBox box1, HitBox box2)
{
    //Check if any corners of box1 are inside box2

    if(hitboxCornerTest(box1, box2))
        return TRUE;

    //Check if any corners of box2 are inside box1
    if(hitboxCornerTest(box2, box1))
        return TRUE;

    return FALSE;
}
