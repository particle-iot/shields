#include "RelayShield.h"

#define relay1 D3
#define relay2 D4
#define relay3 D5
#define relay4 D6

bool relays[5] = {false,false,false,false,false};

RelayShield::RelayShield(){
    
}

void RelayShield::begin(){
    pinMode(relay1, OUTPUT);
    pinMode(relay2, OUTPUT);
    pinMode(relay3, OUTPUT);
    pinMode(relay4, OUTPUT);
}

void RelayShield::on(uint8_t i){
    switch(i){
        case 1:
            digitalWrite(relay1, HIGH);
            relays[1] = true;
            break;
        case 2:
            digitalWrite(relay2, HIGH);
            relays[2] = true;
            break;
        case 3:
            digitalWrite(relay3, HIGH);
            relays[3] = true;
            break;
        case 4:
            digitalWrite(relay4, HIGH);
            relays[4] = true;
            break;
    }
}

void RelayShield::off(uint8_t i){
    switch(i){
        case 1:
            digitalWrite(relay1, LOW);
            relays[1] = false;
            break;
        case 2:
            digitalWrite(relay2, LOW);
            relays[2] = false;
            break;
        case 3:
            digitalWrite(relay3, LOW);
            relays[3] = false;
            break;
        case 4:
            digitalWrite(relay4, LOW);
            relays[4] = false;
            break;
    }
}

void RelayShield::allOn(){
    digitalWrite(relay1,HIGH);
    relays[1] = true;
    digitalWrite(relay2,HIGH);
    relays[2] = true;
    digitalWrite(relay3,HIGH);
    relays[3] = true;
    digitalWrite(relay4,HIGH);
    relays[4] = true;
}

void RelayShield::allOff(){
    digitalWrite(relay1,LOW);
    relays[1] = false;
    digitalWrite(relay2,LOW);
    relays[2] = false;
    digitalWrite(relay3,LOW);
    relays[3] = false;
    digitalWrite(relay4,LOW);
    relays[4] = false;
}

bool RelayShield::isOn(uint8_t i){
    return relays[i];
}