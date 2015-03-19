#ifndef WIFI_DISPLAY_MANAGER_H
#define WIFI_DISPLAY_MANAGER_H

#include <utils/StrongPointer.h>
#include <media/IRemoteDisplay.h>

using android::sp;

namespace mozilla {

// WifiDisplay API for b2g
//__attribute__ ((visibility ("default")))
sp<android::IRemoteDisplay> listenForRemoteDisplay(const char* iface);

} // end of namespace mozilla

#endif
