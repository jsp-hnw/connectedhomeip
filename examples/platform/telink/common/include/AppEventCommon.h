/*
 *
 *    Copyright (c) 2022 Project CHIP Authors
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#pragma once

#include <cstdint>

struct AppEvent;
typedef void (*EventHandler)(AppEvent *);

class LEDWidget;

struct AppEvent
{
    enum AppEventTypes
    {
        kEventType_Button = 0,
        kEventType_Timer,
        kEventType_UpdateLedState,
        kEventType_IdentifyStart,
        kEventType_IdentifyStop,
        kEventType_Lighting,
        kEventType_Thermostat,
        kEventType_Install,
        kEventType_Contact,
        kEventType_Start,
        kEventType_Lock
    };

    uint16_t Type;

    union
    {
        struct
        {
            uint8_t Action;
        } ButtonEvent;
        struct
        {
            void * Context;
        } TimerEvent;
        struct
        {
            uint8_t Action;
            int32_t Actor;
        } LightingEvent;
        struct
        {
            uint8_t Action;
        } ContactEvent;
        struct
        {
            uint8_t Action;
            int32_t Actor;
        } StartEvent;
        struct
        {
            LEDWidget * LedWidget;
        } UpdateLedStateEvent;
    };

    EventHandler Handler;
};
