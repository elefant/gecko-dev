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

#ifndef GONKDISPLAY_H
#define GONKDISPLAY_H

#include <system/window.h>
#include "mozilla/Types.h"
#include "DisplayDevice.h"

namespace android {
    class IGraphicBufferProducer;
}

namespace mozilla {

namespace layers {
class Layer;
}

typedef void * EGLDisplay;
typedef void * EGLSurface;

class MOZ_EXPORT GonkDisplay {
public:
    enum {
        DISPLAY_PRIMARY, // The default builtin display.
        DISPLAY_EXTERNAL, // Display connected through HDMI.
        DISPLAY_VIRTUAL, // Display connected through network.
        NUM_DISPLAY_TYPES
    };

    virtual ANativeWindow* GetNativeWindow(const uint32_t aType = DISPLAY_PRIMARY) = 0;

    virtual void SetEnabled(bool enabled) = 0;

    typedef void (*OnEnabledCallbackType)(bool enabled);

    virtual void OnEnabled(OnEnabledCallbackType callback) = 0;

    virtual void* GetHWCDevice() = 0;

    virtual void* GetFBSurface(const uint32_t aType = DISPLAY_PRIMARY) = 0;

    virtual bool SwapBuffers(EGLDisplay dpy, EGLSurface sur) = 0;

    virtual ANativeWindowBuffer* DequeueBuffer() = 0;

    virtual bool QueueBuffer(ANativeWindowBuffer* buf) = 0;

    virtual void UpdateFBSurface(EGLDisplay dpy, EGLSurface sur) = 0;

    /**
     *
     */
    virtual uint32_t AddDisplay(
        const uint32_t aType,
        const android::sp<android::IGraphicBufferProducer>& aProducer = nullptr)
    {
        return aType;
    }

    /**
     *
     */
    virtual void RemoveDisplay(const uint32_t aType) {}

    virtual float GetXdpi(const uint32_t aType = DISPLAY_PRIMARY) = 0;

    virtual int32_t GetSurfaceformat(const uint32_t aType = DISPLAY_PRIMARY) = 0;

    /**
     * Set FramebufferSurface ReleaseFence's file descriptor.
     * ReleaseFence will be signaled after the HWC has finished reading
     * from a buffer.
     */
    virtual void SetFBReleaseFd(int fd, const uint32_t aType = DISPLAY_PRIMARY) = 0;

    /**
     * Get FramebufferSurface AcquireFence's file descriptor
     * AcquireFence will be signaled when a buffer's content is available.
     */
    virtual int GetPrevFBAcquireFd(const uint32_t aType = DISPLAY_PRIMARY) = 0;

    virtual DisplayDevice* GetDevice(const uint32_t aType) = 0;
};

MOZ_EXPORT __attribute__ ((weak))
GonkDisplay* GetGonkDisplay();

}
#endif /* GONKDISPLAY_H */
