#include <Arduino.h>

float batteryVoltage(uint8_t pin){
    uint32_t Vbatt = 0;
    for(int i = 0; i < 16; i++) {
    Vbatt = Vbatt + analogReadMilliVolts(pin); // ADC with correction   
    }
    float Vbattf = 2 * Vbatt / 16 / 1000.0;     // attenuation ratio 1/2, mV --> V
    return Vbattf;
}
