/*
 * oled_display.h
 *
 *  Created on: Feb 17, 2019
 *      Author: EmilAliyev
 */

#ifndef OLED_DISPLAY_H_
#define OLED_DISPLAY_H_


void oledincrement();
void OLEDSetup();
void printChar(char c);
void printString(char *str);
void endGameMessage(char *str);
void titleScreenMessage(char *str);


#endif /* OLED_DISPLAY_H_ */
