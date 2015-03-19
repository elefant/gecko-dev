/* -*- Mode: Java; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set shiftwidth=2 tabstop=2 autoindent cindent expandtab: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

"use strict";

this.EXPORTED_SYMBOLS = ["WifiP2pWfdDevice"];

// Device type constants.
const DEVICE_TYPE_SOURCE                 = 0;
const DEVICE_TYPE_PRIMARY_SINK           = 1;
const DEVICE_TYPE_SECONDARY_SINK         = 2;
const DEVICE_TYPE_SOURCE_OR_PRIMARY_SINK = 3;

// Device information bitmap.
const DEVICE_TYPE                    = 0x3;
const COUPLED_SINK_SUPPORT_AT_SOURCE = 0x4;
const COUPLED_SINK_SUPPORT_AT_SINK   = 0x8;
const SESSION_AVAILABLE              = 0x30;
const SESSION_AVAILABLE_BIT1         = 0x10;
const SESSION_AVAILABLE_BIT2         = 0x20;

this.WifiP2pWfdDevice = function(aWfdDeviceInfoInHex) {
  let wfdDevice = {};

  if (aWfdDeviceInfoInHex) {
    let deviceInfo = parseInt(aWfdDeviceInfoInHex.substr(0, 4), 16);
    wfdDevice = {
      deviceType: deviceInfo & DEVICE_TYPE,
      sessionAvailable: 0 !== (deviceInfo & SESSION_AVAILABLE),
      coupleSinkSupportAtSink: 0 !== (deviceInfo & COUPLED_SINK_SUPPORT_AT_SINK),
      coupleSinkSupportAtSource: 0 !== (deviceInfo & COUPLED_SINK_SUPPORT_AT_SOURCE),
      controlPort: parseInt(aWfdDeviceInfoInHex.substr(4, 4), 16),
      maxThroughput: parseInt(aWfdDeviceInfoInHex.substr(8, 4), 16)
    };
  }

  wfdDevice.__exposedProps__ = {
    deviceType: 'r',
    sessionAvailable: 'r',
    coupleSinkSupportAtSink: 'r',
    coupleSinkSupportAtSource: 'r',
    controlPort: 'r',
    maxThroughput: 'r'
  };

  wfdDevice.toHexString = function() {
    // Compose deviceInfo.
    let deviceInfo = 0;
    if (wfdDevice.sessionAvailable) {
      deviceInfo |= SESSION_AVAILABLE_BIT1;
      deviceInfo &= ~SESSION_AVAILABLE_BIT2;
    }
    if (wfdDevice.coupleSinkSupportAtSink) {
      deviceInfo |= COUPLED_SINK_SUPPORT_AT_SINK;
    }
    if (wfdDevice.coupleSinkSupportAtSource) {
      deviceInfo |= COUPLED_SINK_SUPPORT_AT_SOURCE;
    }
    deviceInfo |= wfdDevice.deviceType;

    function padZero(aString, aLength) {
      while(aString.length < aLength) {
        aString = '0' + aString;
      }
      return aString;
    }

    return "0006" +
           padZero(deviceInfo.toString(16), 4) +
           padZero(wfdDevice.controlPort.toString(16), 4) +
           padZero(wfdDevice.maxThroughput.toString(16), 4);
  };

  return wfdDevice;
};

// Expose device type constants.
this.WifiP2pWfdDevice.DEVICE_TYPE_SOURCE                 = DEVICE_TYPE_SOURCE;
this.WifiP2pWfdDevice.DEVICE_TYPE_PRIMARY_SINK           = DEVICE_TYPE_PRIMARY_SINK;
this.WifiP2pWfdDevice.DEVICE_TYPE_SECONDARY_SINK         = DEVICE_TYPE_SECONDARY_SINK;
this.WifiP2pWfdDevice.DEVICE_TYPE_SOURCE_OR_PRIMARY_SINK = DEVICE_TYPE_SOURCE_OR_PRIMARY_SINK;
