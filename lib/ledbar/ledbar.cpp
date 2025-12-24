#include "Adafruit_LEDBackpack.h"

const int MAXDB = -50;
const int MINDB = -100;

void initBar(Adafruit_24bargraph& bar){
    bar.begin(0x70);  // pass in the address

    for (uint8_t b=0; b<24; b++ ){
        if ((b % 3) == 0)  bar.setBar(b, LED_RED);
        if ((b % 3) == 1)  bar.setBar(b, LED_YELLOW);
        if ((b % 3) == 2)  bar.setBar(b, LED_GREEN);
    }
    bar.writeDisplay();
}

void setBar(Adafruit_24bargraph& bar, float rssi){
    int num_stars = map(constrain(rssi, MINDB, MAXDB), MINDB, MAXDB, 0, 24);
    if (num_stars > 0){
        for (int i=23; i>=0; i--) {
            if ((i / 8) == 0)  bar.setBar(i, LED_GREEN);
            else if ((i / 16) == 0)  bar.setBar(i, LED_YELLOW);
            else if ((i / 24) == 0)  bar.setBar(i, LED_RED);
        }
        for (int i=0; i<24-num_stars; i++){
            bar.setBar(i, LED_OFF);
        }
        bar.writeDisplay();
    }else{
        for (int i=0; i<24; i++){
            bar.setBar(i, LED_OFF);
        }
        bar.writeDisplay();
        for (int i=0; i<24; i++){
            bar.setBar(i, LED_RED);
            bar.writeDisplay();
            delay(40);
            bar.setBar(i, LED_OFF);
            bar.writeDisplay();
        }
    }
}
