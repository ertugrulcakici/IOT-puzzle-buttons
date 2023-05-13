#include <Arduino.h>

#define BUZZER_PIN 8
#define PISTON_PIN 3

#define BLACK_SENSOR_PIN A0
#define RED_SENSOR_PIN A1
#define BLUE_SENSOR_PIN A2
#define GREEN_SENSOR_PIN A3

#define BLACK_VALUE 1
#define RED_VALUE 2
#define BLUE_VALUE 3
#define GREEN_VALUE 4

int sequence[10] = {};
// 2nd: 1 black
// 4th: 3 red
// 1st: 2 blue
// 3rd: 4 green
// 1st in sequence is blue 2 times, 2nd is black 1 time, 3rd is green 4 times, 4th is red 3 times
int correctSequence[10] = {3, 3, 1, 4, 4, 4, 4, 2, 2, 2};

// Debug and print modes are defined for debugging and logging
#define DEBUG_MODE
// #define PRINT_MODE

void setup()
{
// The DEBUG_MODE and PRINT_MODE macros are used to control debugging and logging
#ifdef DEBUG_MODE
    Serial.begin(9600);
    Serial.println("DEBUG MODE");
#endif
#ifdef PRINT_MODE
    Serial.begin(9600);
    Serial.println("PRINT MODE");
#endif
    pinMode(PISTON_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
}

void loop()
{
// If PRINT_MODE is defined, print the analog values of each color sensor
#ifdef PRINT_MODE
    while (true)
    {
        Serial.print("A0: ");
        Serial.print(analogRead(BLACK_SENSOR_PIN));
        Serial.print(" A1: ");
        Serial.print(analogRead(RED_SENSOR_PIN));
        Serial.print(" A2: ");
        Serial.print(analogRead(BLUE_SENSOR_PIN));
        Serial.print(" A3: ");
        Serial.println(analogRead(GREEN_SENSOR_PIN));
        delay(300);
    }
#endif

    // Read the sensor values
    int blackVal = analogRead(BLACK_SENSOR_PIN);
    int redVal = analogRead(RED_SENSOR_PIN);
    int blueVal = analogRead(BLUE_SENSOR_PIN);
    int greenVal = analogRead(GREEN_SENSOR_PIN);

    // If any sensor reads 0, the buzzer is activated
    if (blackVal == 0 || redVal == 0 || blueVal == 0 || greenVal == 0)
    {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(150);
        digitalWrite(BUZZER_PIN, LOW);

        // If a specific sensor reads 0, its corresponding value is added to the sequence
        // and a message is printed in debug mode
        if (blackVal == 0)
        {
            addValueToSequence(BLACK_VALUE);
#ifdef DEBUG_MODE
            Serial.println("Black added to sequence");
#endif
            while (analogRead(BLACK_SENSOR_PIN) == 0)
            {
                delay(100);
            }
        }
        if (redVal == 0)
        {
            addValueToSequence(RED_VALUE);
#ifdef DEBUG_MODE
            Serial.println("Red added to sequence");
#endif
            while (analogRead(RED_SENSOR_PIN) == 0)
            {
                delay(100);
            }
        }
        if (blueVal == 0)
        {
            addValueToSequence(BLUE_VALUE);
#ifdef DEBUG_MODE
            Serial.println("Blue added to sequence");
#endif
            while (analogRead(BLUE_SENSOR_PIN) == 0)
            {
                delay(100);
            }
        }
        if (greenVal == 0)
        {
            addValueToSequence(GREEN_VALUE);
#ifdef DEBUG_MODE
            Serial.println("Green added to sequence");
#endif
            while (analogRead(GREEN_SENSOR_PIN) == 0)
            {
                delay(100);
            }
        }

        // If the sequence is correct, activate the piston
        if (checkSequence())
        {
#ifdef DEBUG_MODE
            Serial.println("Sequence is correct");
#endif
            digitalWrite(PISTON_PIN, HIGH);
            delay(1000);
            digitalWrite(PISTON_PIN, LOW);
        }
// If in debug mode, print the current sequence
#ifdef DEBUG_MODE
        printSequence();
#endif
    }
    delay(150);
}

void addValueToSequence(int value)
{
    // Shift sequence array by one to the left
    for (int i = 0; i < 9; i++)
    {
        sequence[i] = sequence[i + 1];
    }
    // Add new value to the last position
    sequence[9] = value;
}

bool checkSequence()
{
    // Check if the current sequence is correct
    for (int i = 0; i < 10; i++)
    {
        if (sequence[i] != correctSequence[i])
        {
            return false;
        }
    }
    return true;
}

void printSequence()
{
    // Print current sequence
    for (int i = 0; i < 10; i++)
    {
        Serial.print(sequence[i]);
        Serial.print(" ");
    }
    Serial.println();
}
