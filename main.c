#include <avr/io.h>
#include "P0/p0.h"
#include "P1/P1.h" 
#include <time.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include "serial/serial.h"




int main(void)
{
    // initialize the serial communication
    USART_Init(MYUBRR);
    setup_p1();
    uint16_t sequence_delay;
    while(1) {
        uint16_t pot_value = read_pot1();
        sequence_delay = (((uint32_t)pot_value * 500) / 1023) + 50;

        char buffer[10]; // Buffer to hold the string representation of pot_value
        sprintf(buffer, "%u", sequence_delay); // Convert pot_value to string
        sendString(buffer); // Send the string over USART
        sendString("\n\r"); // Send newline and carriage return for better readability

        int leds[3] = {led1, led2, led3};  // LED function references
        int states[][3] = {
            {1, 0, 0},  // led1 ON
            {0, 1, 0},  // led2 ON  
            {0, 0, 1},  // led3 ON
        };
        uint8_t last_state;

        while (!(sw3()))
        {
            for (int i = 0; i < 3; i++) {
                // Set the LED states based on the array
                led1(states[i][0]);
                led2(states[i][1]);
                led3(states[i][2]);
                set_number(i + 1);  // Display the current LED number

                wait(sequence_delay);  // Delay

                while(sw1()) {}  // Wait for switch press
                // Turn all LEDs off
                led1(0);
                led2(0);
                led3(0);


            }

            while (sw2()) // reverse led sequence
            {
                for (int i = 2; i >= 0; i--) {
                    // Set the LED states based on the array
                    led1(states[i][0]);                    
                    led2(states[i][1]);                    
                    led3(states[i][2]);
                    set_number(i + 1);  // Display the current LED number
                    wait(sequence_delay);  // Delay

                    while(sw1()) {}  // Wait for switch press

                    // Turn all LEDs off
                    led1(0);
                    led2(0);
                    led3(0);

                }
            }




        }


    }
}