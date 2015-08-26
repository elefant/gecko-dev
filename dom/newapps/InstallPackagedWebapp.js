/* This Source Code Form is subject to the terms of the Mozilla Public
 *  * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 *   * You can obtain one at http://mozilla.org/MPL/2.0/. */

const { classes: Cc, interfaces: Ci, utils: Cu, results: Cr, Constructor: CC } = Components;

Cu.import("resource://gre/modules/XPCOMUtils.jsm");
Cu.import("resource://gre/modules/AppsUtils.jsm");

XPCOMUtils.defineLazyModuleGetter(this, "PermissionsInstaller",
    "resource://gre/modules/PermissionsInstaller.jsm");

function InstallPackagedWebapp() {
}

InstallPackagedWebapp.prototype = {
  classDescription: "InstallPackagedWebapp JavaScript XPCOM Component",
  classID:          Components.ID("{5cc6554a-5421-4a5e-b8c2-c62e8b7f4f3f}"),
  contractID:       "@mozilla.org/newapps/installpackagedwebapp;1",

  QueryInterface: XPCOMUtils.generateQI([Components.interfaces.nsIInstallPackagedWebapp]),

  installPackagedWebapp: function(manifestContent, aOrigin, aManifestURL) {

    var aManifest = JSON.parse(manifestContent);
   
    PermissionsInstaller.installPermissions({
      manifest: aManifest,
      manifestURL: aManifestURL,
      origin: aOrigin,
      kind: ""
    }, false);

    //TODO: register app handlers (system msg)
    return; 
  },
};

var NSGetFactory = XPCOMUtils.generateNSGetFactory([InstallPackagedWebapp]);
