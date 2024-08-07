/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
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

#include <app/clusters/keypad-input-server/keypad-input-server.h>
#include <jni.h>

using chip::app::CommandResponseHelper;
using KeypadInputDelegate = chip::app::Clusters::KeypadInput::Delegate;
using SendKeyResponseType = chip::app::Clusters::KeypadInput::Commands::SendKeyResponse::Type;

class KeypadInputManager : public KeypadInputDelegate
{
public:
    static void NewManager(jint endpoint, jobject manager);
    void InitializeWithObjects(jobject managerObject);

    void HandleSendKey(CommandResponseHelper<SendKeyResponseType> & helper,
                       const chip::app::Clusters::KeypadInput::CECKeyCodeEnum & keyCode) override;

    uint32_t GetFeatureMap(chip::EndpointId endpoint) override;

private:
    jobject mKeypadInputManagerObject = nullptr;
    jmethodID mSendKeyMethod          = nullptr;

    // TODO: set this based upon meta data from app
    uint32_t mDynamicEndpointFeatureMap = 7;
};
