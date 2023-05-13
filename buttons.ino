#include <Arduino.h>

#define buzzer 8
#define piston 3

#define black A0
#define red A1 
#define blue A2
#define green A3

#define blackValue 1
#define redValue 2
#define blueValue 3
#define greenValue 4

int order[10] = {};
// 2. 1t 
// 4. 3t
// 1. 2t
// 3. 4t
// 1. sırada mavi 2 defa, 2. sırada siyah 1 defa, 3. sırada yeşil 4 defa, 4. sırada kırmızı 3 defa
// kırmızı pembe kahverengi mavi
int correctOrder[10] = {3, 3, 1, 4, 4, 4, 4, 2, 2, 2};


 
#define DEBUG_MODE
// #define PRINT_MODE


void setup()
{
    #ifdef DEBUG_MODE
    Serial.begin(9600);
    Serial.println("DEBUG MODE");
    #endif
    #ifdef PRINT_MODE
    Serial.begin(9600);
    Serial.println("PRINT MODE");
    #endif
    pinMode(piston, OUTPUT);
    pinMode(buzzer,OUTPUT);
}

void loop()
{

    #ifdef PRINT_MODE
    while (true)
    {
        Serial.print("A0: ");
        Serial.print(analogRead(black));
        Serial.print(" A1: ");
        Serial.print(analogRead(red));
        Serial.print(" A2: ");
        Serial.print(analogRead(blue));
        Serial.print(" A3: ");
        Serial.println(analogRead(green));
        delay(300);
    }
    #endif


    int blackv = analogRead(black);
    int redv = analogRead(red);
    int bluev = analogRead(blue);
    int greenv = analogRead(green);

    if (blackv == 0 || redv == 0 || bluev == 0 || greenv == 0) {
        digitalWrite(buzzer, HIGH);
        delay(150);
        digitalWrite(buzzer, LOW);
        if (blackv == 0) {
            addValue(blackValue);
            #ifdef DEBUG_MODE
            Serial.println("siyah sıraya eklendi");
            # endif
            while (analogRead(black) == 0) {delay(100);}
        }
        if (redv == 0) {
            addValue(redValue);
            #ifdef DEBUG_MODE
            Serial.println("Kırmızı sıraya eklendi");
            #endif
            while (analogRead(red) == 0) {delay(100);}
        }
        if (bluev == 0) {
            addValue(blueValue);
            #ifdef DEBUG_MODE
            Serial.println("Mavi sıraya eklendi");
            #endif
            while (analogRead(blue) == 0) {delay(100);}
        }
        if (greenv == 0) {
            addValue(greenValue);
            #ifdef DEBUG_MODE
            Serial.println("Yeşil sıraya eklendi");
            #endif
            while (analogRead(green) == 0) {delay(100);}
        }

        if (checkOrder()) {
            #ifdef DEBUG_MODE
            Serial.println("Sıra doğru");
            #endif
            digitalWrite(piston, HIGH);
            delay(1000);
            digitalWrite(piston, LOW);
        }
        #ifdef DEBUG_MODE
        printOrder();
        #endif
    }
    delay(150);
}


void addValue(int value) {
    // shift order array by one to left
    for (int i = 0; i < 9; i++) {
        order[i] = order[i + 1];
    }
    // add new value to the last position
    order[9] = value;
}

bool checkOrder () {
    for (int i = 0; i < 10; i++) {
        if (order[i] != correctOrder[i]) {
            return false;
        }
    }
    return true;
}

void printOrder () {
    for (int i = 0; i < 10; i++) {
        Serial.print(order[i]);
        Serial.print(" ");
    }
    Serial.println();
}
