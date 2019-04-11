/*
 * oled_display.c
 *
 *  Created on: Feb 17, 2019
 *      Author: EmilAliyev
 */

//Header include
#include "oled_display.h"

// Driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "interrupt.h"
#include "hw_apps_rcm.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"
#include "prcm.h"
#include "gpio.h"
#include "utils.h"

// OLED includes
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1351.h"
#include "glcdfont.h"
#include "test.h"
#include "spi.h"

//libc includes
#include "string.h"

//SPI
#define SPI_IF_BIT_RATE  100000

//OLED
#define OLED_WIDTH 128
#define OLED_HEIGHT 128
#define OLED_INCREMENT 8 //Gap between characters on OLED
#define OLED_X_START 0 //X pixel to start printing at
#define OLED_Y_START 24 //y pixel to start printing T

//x and y oled pixels to write to
int oledx = OLED_X_START;
int oledy = OLED_Y_START;

//Pixels to start printing at if in middle of screen
#define OLED_X_MIDDLE_START 40
#define OLED_Y_MIDDLE_START 64

//Pixels to start printing at for title
#define OLED_X_TITLE_START 0
#define OLED_Y_TITLE_START 64

//x and y oled for recieving
int oledxR = 0;
int oledyR = 64;

//Reset to default values
static void resetDefaults()
{
    oledx = OLED_X_START;
    oledy = OLED_Y_START;
}


//Set up the OLED and SPI
void OLEDSetup()
{
    MAP_PRCMPeripheralClkEnable(PRCM_GSPI,PRCM_RUN_MODE_CLK);
    MAP_PRCMPeripheralReset(PRCM_GSPI);
    //
    // Reset SPI
    //
    MAP_SPIReset(GSPI_BASE);

    //
    // Configure SPI interface
    //
    MAP_SPIConfigSetExpClk(GSPI_BASE,MAP_PRCMPeripheralClockGet(PRCM_GSPI),
                     SPI_IF_BIT_RATE,SPI_MODE_MASTER,SPI_SUB_MODE_0,
                     (SPI_SW_CTRL_CS |
                     SPI_4PIN_MODE |
                     SPI_TURBO_OFF |
                     SPI_CS_ACTIVEHIGH |
                     SPI_WL_8));

    //
    // Enable SPI for communication
    //
    MAP_SPIEnable(GSPI_BASE);

    //
    // Muxing UART and SPI lines.
    //


    //
    // Enable the SPI module clock
    //


    //Initialize adafruit
    Adafruit_Init();

    //Clear screen
    fillScreen(BLACK);
}

//Increment to next pixel when a character is written
void oledIncrement()
{
    oledx+= OLED_INCREMENT;

    //If at end of row, move to next row
    if(oledx >= OLED_WIDTH)
    {
        oledx = OLED_X_START;
        oledy+= OLED_INCREMENT;

        //If at end of screen, wipe screen and start over
        if(oledy >= OLED_HEIGHT)
        {
            fillScreen(BLACK);
            oledy = OLED_Y_START;
        }
    }
}

//Print c to OLED display
void printChar(char c)
{
    drawChar(oledx, oledy, c, WHITE, BLACK, 1);
    oledIncrement();
}

//Print str to OLED display
void printString(char *str)
{
    int len = strlen(str);
    int i = 0;

    for(i = 0; i < len; i++)
    {
        printChar(str[i]);
    }
}

//Print string in middle of OLED display
static void printStringMiddle(char *str)
{
    oledx = OLED_X_MIDDLE_START;
    oledy = OLED_Y_MIDDLE_START;

    printString(str);

    resetDefaults();
}


//End of game message
void endGameMessage(char *str)
{
    fillScreen(BLACK);
    printStringMiddle(str);
}

//Display the title
void titleScreenMessage(char *str)
{
    fillScreen(BLACK);

    oledx = OLED_X_TITLE_START;
    oledy = OLED_Y_TITLE_START;

    printString(str);

    resetDefaults();

    fillScreen(BLACK);
}

