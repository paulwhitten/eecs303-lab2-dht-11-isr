/**

    Minimal ISR interaction with DHT 11.

    Performs ten readings of the sensor.  The rising
    edge microsecond timings are saved and output to
    the console.

    To compile:
      gcc filename.c -o filename -lwiringPi
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sched.h>
#include <stdint.h>
#include <time.h>

#include "wiringPi.h"

#define MAX_EDGES 100

// Storage for timings of rising edges
// we really only need 43 or so
unsigned int risingEdgeTimes[MAX_EDGES];
// index for current storage location
static volatile int risingEdgeIndex = 0;

// Using the last pin on the pi
// Trying to use pin 7 GPIO 4 gave
// me errors
const int SENSOR_PIN_NUM = 21;

void initiateRead();
void sensorReadISR();
void setupGpio();
void releaseGpio();

int main()
{
    int i;

    if (piHiPri(99) == -1)
    {
        printf("Error setting priority! Exiting");
        return -1;
    }

    if (wiringPiSetupGpio() < 0) {
        printf("Error setting up eiting pi for GPIO! Exiting");
        return -1;
    }

    if (wiringPiISR(SENSOR_PIN_NUM, INT_EDGE_RISING, sensorReadISR) < 0)
    {
            printf("ERROR SETTING ISR!\n");
    }               
    
    int readIdx = 0;
    while (readIdx < 10)
    {
            printf("\nSample Number: %i\n", readIdx);
            
            initiateRead();
            
            //delay two seconds
            delayMicroseconds(2000000);
            
            printf("%d edges\n", risingEdgeIndex);
            for (i = 0; i < risingEdgeIndex; i++) {
                    printf("%u\n", risingEdgeTimes[i]);
            }       
            
            ++readIdx;
    }       
            
    // Free the GPIO
    releaseGpio();
    
    return 0;
}

/*
 * Prepares the sensor for reading.
 */
void initiateRead()
{
    // pull pin down for 18 milliseconds
    pinMode( SENSOR_PIN_NUM, OUTPUT );
    digitalWrite( SENSOR_PIN_NUM, LOW );
    delay( 18 );
    // pull it up for 40 microseconds
    digitalWrite( SENSOR_PIN_NUM, HIGH );
    delayMicroseconds( 40 );
    // prepare to read the pin
    pinMode( SENSOR_PIN_NUM, INPUT );
    // reset the index
    risingEdgeIndex = 0;
}

/*
 * ISR for reading the sensor. There are several states which this ISR accounts for.
 */
void sensorReadISR()
{
    // save the microsecond timing
    if (risingEdgeIndex < MAX_EDGES) {
            risingEdgeTimes[risingEdgeIndex++] = micros();
    }
}


// Releases the GPIO reservation.
void releaseGpio()
{
    pinMode(SENSOR_PIN_NUM, INPUT);
}
