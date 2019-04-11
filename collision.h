/*
 * collision.h
 *
 *  Created on: Mar 11, 2019
 *      Author: EmilAliyev
 */

#ifndef COLLISION_H_
#define COLLISION_H_

//The hit box
typedef struct HitBox
{
    int xmin;
    int xmax;
    int ymin;
    int ymax;
}HitBox;

int hitboxOverlap(HitBox box1, HitBox box2);

#endif /* COLLISION_H_ */
