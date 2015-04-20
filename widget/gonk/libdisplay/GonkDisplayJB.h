/* Copyright 2013 Mozilla Foundation and Mozilla contributors
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

#ifndef GONKDISPLAYJB_H
#define GONKDISPLAYJB_H

#include "GonkDisplay.h"
#include "FramebufferSurface.h"
#include "hardware/hwcomposer.h"
#include "hardware/power.h"
#include "ui/Fence.h"
#include "utils/RefBase.h"
#include "nsIDisplayDevice.h"
#include "nsTArray.h"

namespace mozilla {

class MOZ_EXPORT GonkDisplayJB : public GonkDisplay {
public:
    GonkDisplayJB();
    ~GonkDisplayJB();

    virtual ANativeWindow* GetNativeWindow(const uint32_t aType = DISPLAY_PRIMARY);

    virtual void SetEnabled(bool enabled);

    virtual void OnEnabled(OnEnabledCallbackType callback);

    virtual void* GetHWCDevice();

    virtual void* GetFBSurface(const uint32_t aType = DISPLAY_PRIMARY);

    virtual bool SwapBuffers(EGLDisplay dpy, EGLSurface sur);

    virtual ANativeWindowBuffer* DequeueBuffer();

    virtual bool QueueBuffer(ANativeWindowBuffer* buf);

    virtual void UpdateFBSurface(EGLDisplay dpy, EGLSurface sur);

    virtual void SetFBReleaseFd(int fd, const uint32_t aType = DISPLAY_PRIMARY);

    virtual int GetPrevFBAcquireFd(const uint32_t aType = DISPLAY_PRIMARY);

    bool Post(buffer_handle_t buf, int fence);

    virtual uint32_t AddDisplay(
        const uint32_t aType,
        const android::sp<android::IGraphicBufferProducer>& aProducer = nullptr);

    virtual void RemoveDisplay(const uint32_t aType);

    virtual float GetXdpi(const uint32_t aType = DISPLAY_PRIMARY);

    virtual int32_t GetSurfaceformat(const uint32_t aType = DISPLAY_PRIMARY);

protected:
    virtual DisplayDevice* GetDevice(const uint32_t aType);

private:
    void NotifyDisplayChange(nsIDisplayDevice* aDisplayDevice);

    hw_module_t const*        mModule;
    hw_module_t const*        mFBModule;
    hwc_composer_device_1_t*  mHwc;
    hwc_display_contents_1_t* mList;
    framebuffer_device_t*     mFBDevice;
    power_module_t*           mPowerModule;
    android::sp<android::IGraphicBufferAlloc> mAlloc;
    OnEnabledCallbackType mEnabledCallback;
    nsTArray<DisplayDevice> mDevices;
};

}

#endif /* GONKDISPLAYJB_H */
