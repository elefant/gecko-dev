#define LOG_NDEBUG 0
#define LOG_TAG "WifiDisplayManager"
#include <utils/Log.h>

#include "WifiDisplayManager.h"
#include <media/IMediaPlayerService.h>
#include <media/IRemoteDisplayClient.h>
#include <binder/IServiceManager.h>
#include <media/AudioSystem.h>
#include <media/AudioSystem.h>
#include <media/IAudioPolicyService.h>
#include <utils/String8.h>
#include <gui/IGraphicBufferProducer.h>
#include "libdisplay/GonkDisplay.h"

using namespace android;

namespace mozilla {

static status_t enableAudioSubmix(bool enable) {
  ALOGI("Enabling AUDIO_DEVICE_IN_REMOTE_SUBMIX");

  status_t err = AudioSystem::setDeviceConnectionState(
          AUDIO_DEVICE_IN_REMOTE_SUBMIX,
          enable
              ? AUDIO_POLICY_DEVICE_STATE_AVAILABLE
              : AUDIO_POLICY_DEVICE_STATE_UNAVAILABLE,
          NULL /* device_address */);

  if (err != OK) {
      return err;
  }

  err = AudioSystem::setDeviceConnectionState(
          AUDIO_DEVICE_OUT_REMOTE_SUBMIX,
          enable
              ? AUDIO_POLICY_DEVICE_STATE_AVAILABLE
              : AUDIO_POLICY_DEVICE_STATE_UNAVAILABLE,
          NULL /* device_address */);

  return err;
}

class NativeRemoteDisplayClient : public BnRemoteDisplayClient {
public:
    virtual void onDisplayConnected(const sp<IGraphicBufferProducer>& bufferProducer,
            uint32_t width, uint32_t height, uint32_t flags, uint32_t session) {
        ALOGI("Callback onDisplayConnected");
        mozilla::GetGonkDisplay()->SetVirtualDisplayBuffer(bufferProducer);
    }

    virtual void onDisplayDisconnected() {
        ALOGI("Callback onDisplayDisconnected");
        enableAudioSubmix(false);
        mozilla::GetGonkDisplay()->SetVirtualDisplayBuffer(nullptr);
    }

    virtual void onDisplayError(int32_t error) {
        ALOGI("Callback onDisplayError");
    }
};

sp<android::IRemoteDisplay> listenForRemoteDisplay(const char* iface)
{
    sp<IServiceManager> sm = defaultServiceManager();

    sp<IMediaPlayerService> service = interface_cast<IMediaPlayerService>(sm->getService(String16("media.player")));
    if (service == NULL) {
        ALOGE("Could not obtain IMediaPlayerService from service manager");
        return 0;
    }

    enableAudioSubmix(true);

    sp<NativeRemoteDisplayClient> client(new NativeRemoteDisplayClient());
    sp<IRemoteDisplay> display = service->listenForRemoteDisplay(client, String8(iface));
    if (display == NULL) {
        ALOGE("Wifi display service rejected request to listen for remote display '%s'.",
                (const char*)iface);
        return 0;
    }

    return display;
}

} // end of namespace mozilla
