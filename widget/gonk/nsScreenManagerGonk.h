/* -*- Mode: C++; tab-width: 40; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* Copyright 2012 Mozilla Foundation and Mozilla contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef nsScreenManagerGonk_h___
#define nsScreenManagerGonk_h___

#include "mozilla/Hal.h"
#include "nsCOMPtr.h"

#include "nsBaseScreen.h"
#include "nsIScreenManager.h"
#include <utils/StrongPointer.h>

namespace android
{
    class IGraphicBufferProducer;
}

class nsScreenGonk : public nsBaseScreen
{
    typedef mozilla::hal::ScreenConfiguration ScreenConfiguration;

public:
    nsScreenGonk(const ScreenConfiguration& aConfig,
                 uint32_t aId,
                 uint32_t aPhysicalRotation,
                 void* aNativeWindow);

    ~nsScreenGonk();

    // nsIScreen.
    NS_IMETHOD GetId(uint32_t* aId);
    NS_IMETHOD GetRect(int32_t* aLeft, int32_t* aTop, int32_t* aWidth, int32_t* aHeight);
    NS_IMETHOD GetAvailRect(int32_t* aLeft, int32_t* aTop, int32_t* aWidth, int32_t* aHeight);
    NS_IMETHOD GetPixelDepth(int32_t* aPixelDepth);
    NS_IMETHOD GetColorDepth(int32_t* aColorDepth);
    NS_IMETHOD GetRotation(uint32_t* aRotation);
    NS_IMETHOD SetRotation(uint32_t  aRotation);

    uint32_t GetId();
    nsIntRect GetRect();
    float GetDpi();
    void* GetNativeWindow();
    nsIntRect GetNaturalRect();
    uint32_t GetEffectiveScreenRotation();
    ScreenConfiguration GetScreenConfiguration();
    bool IsPrimaryScreen();

    static uint32_t GetRotation();
    static ScreenConfiguration GetConfiguration();

private:
    uint32_t mId;
    int32_t  mWidth;
    int32_t  mHeight;
    int32_t  mPixelDepth;
    int32_t  mColorDepth;
    uint32_t mRotation;
    void*    mNativeWindow;
    uint32_t mPhysicalRotation;
    uint32_t mNaturalWidth;
    uint32_t mNaturalHeight;
    float    mDpi;
};

class nsScreenManagerGonk final : public nsIScreenManager
{
public:
    nsScreenManagerGonk();

    NS_DECL_ISUPPORTS
    NS_DECL_NSISCREENMANAGER

    nsCOMPtr<nsScreenGonk> GetPrimaryScreen();

    // The following three functions are "display type" based.
    // Other than these functions, we should use screen id to
    // manipulate on nsWindow and GonkDisplay devices.
    nsCOMPtr<nsScreenGonk> ScreenForType(uint32_t aDisplayType);

    void AddScreen(uint32_t aDisplayType,
                   const android::sp<android::IGraphicBufferProducer>& aProducer = nullptr);

    void RemoveScreen(uint32_t aDisplayType);

    void AddPrimaryScreen();

protected:
    ~nsScreenManagerGonk();

    nsTArray<nsCOMPtr<nsScreenGonk>> mScreens;
};

nsCOMPtr<nsScreenManagerGonk> GetScreenManager();

#endif /* nsScreenManagerGonk_h___ */
