#include <Arduino.h>
#include "app/ui_controller.h"

int buttonPins[] = {6,7,15,16,46,5,14,9};


// Массив для сопоставления битов → PhysicalButtons
PhysicalButtons buttonMap[8] = {
    PhysicalButtons::RIGHT, // bit 0
    PhysicalButtons::UP,    // bit 1
    PhysicalButtons::DOWN,  // bit 2
    PhysicalButtons::LEFT,  // bit 3
    PhysicalButtons::FUNC1, // bit 4
    PhysicalButtons::FUNC2, // bit 5
    PhysicalButtons::FUNC3, // bit 6
    PhysicalButtons::FUNC4  // bit 7
};

// ----- ДЕБАУНСЕР -----
const uint32_t debounceMs = 10;

// Состояния
bool stableState[8]   = {0};
bool lastRawState[8]  = {0};
uint32_t lastChangeMs[8] = {0};

// геттер нажатых кнопок — как в вашем коде
uint8_t readButtons()
{
    uint8_t buttons = 0;
    for (int i = 0; i < 8; i++)
    {
        if (digitalRead(buttonPins[i]) == LOW)  // кнопка нажата
            buttons |= (1 << i);
    }
    return buttons;
}

void pollButtons()
{
    uint32_t now = millis();
    uint8_t raw = readButtons();

    //Serial.println(raw);

    for (int i = 0; i < 8; i++)
    {
        bool rawPressed = raw & (1 << i);

        // Смена сырого состояния?
        if (rawPressed != lastRawState[i])
        {
            lastRawState[i] = rawPressed;
            lastChangeMs[i] = now;
        }

        // Проверяем — прошло ли достаточно времени для дебаунса
        if (rawPressed != stableState[i] && (now - lastChangeMs[i] > debounceMs))
        {
            // Стабильное новое состояние
            stableState[i] = rawPressed;

            if (stableState[i])  // реагируем только на нажатие
            {
                appInstance->handlePhysicalButton(buttonMap[i]);
            }
        }
    }
}

void initButtons()
{    
  for(int i = 0 ; i < 8; i++)
    pinMode(buttonPins[i], INPUT_PULLUP);
}