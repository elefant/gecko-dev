/* Copyright 2015 Mozilla Foundation and Mozilla contributors
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
#include "DisplayDevice.h"
#include "GonkDisplay.h"
#include "FramebufferSurface.h"

using namespace android;

namespace mozilla {

NS_IMPL_ISUPPORTS(DisplayDevice, nsIDisplayDevice, nsISupports)

DisplayDevice::DisplayDevice()
  : mType(GonkDisplay::DISPLAY_PRIMARY)
  , mConnected(true)
{
}

DisplayDevice::DisplayDevice(uint32_t aType)
  : mType(aType)
  , mConnected(true)
{
}

DisplayDevice::~DisplayDevice()
{
}

DisplayDevice::DisplayDevice(const DisplayDevice& aDisplayDevice)
	: mType(aDisplayDevice.mType)
	, mConnected(aDisplayDevice.mConnected)
	, mWidth(aDisplayDevice.mWidth)
	, mHeight(aDisplayDevice.mHeight)
	, mXdpi(aDisplayDevice.mXdpi)
	, mSurfaceformat(aDisplayDevice.mSurfaceformat)
{
}

//
// nsIDisplayDevice implementation.
//
NS_IMETHODIMP DisplayDevice::GetConnected(bool *aConnected)
{
	*aConnected = mConnected;
	return NS_OK;
}

NS_IMETHODIMP DisplayDevice::GetType(int32_t *aType)
{
	*aType = mType;
	return NS_OK;
}

NS_IMETHODIMP DisplayDevice::GetHeight(int32_t *aHeight)
{
	*aHeight = mHeight;
	return NS_OK;
}

NS_IMETHODIMP DisplayDevice::GetWidth(int32_t *aWidth)
{
	*aWidth = mWidth;
	return NS_OK;
}

void DisplayDevice::SetConnected(bool aConnected)
{
	mConnected = aConnected;
}

int32_t DisplayDevice::GetSurfaceformat()
{
	return mSurfaceformat;
}

float DisplayDevice::GetXdpi()
{
	return mXdpi;
}

}
