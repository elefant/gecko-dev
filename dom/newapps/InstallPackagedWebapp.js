/* This Source Code Form is subject to the terms of the Mozilla Public
 *  * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 *   * You can obtain one at http://mozilla.org/MPL/2.0/. */

const { classes: Cc, interfaces: Ci, utils: Cu, results: Cr, Constructor: CC } = Components;

Cu.import("resource://gre/modules/XPCOMUtils.jsm");
Cu.import("resource://gre/modules/AppsUtils.jsm");

XPCOMUtils.defineLazyModuleGetter(this, "PermissionsInstaller",
    "resource://gre/modules/PermissionsInstaller.jsm");

function debug(aMsg) {
  dump("-*-*- InstallPackagedWebapps.js : " + aMsg + "\n");
}

function InstallPackagedWebapp() {
}

InstallPackagedWebapp.prototype = {
  QueryInterface: XPCOMUtils.generateQI([Ci.nsIInstallPackagedWebapp]),
  classID:          Components.ID("{5cc6554a-5421-4a5e-b8c2-c62e8b7f4f3f}"),
  classInfo: XPCOMUtils.generateCI({
    classID:          Components.ID("{5cc6554a-5421-4a5e-b8c2-c62e8b7f4f3f}"),
    classDescription: "InstallPackagedWebapp JavaScript XPCOM Component",
    interfaces: [Ci.nsIInstallPackagedWebapp]
  }),


  /**
   * Install permissions for signed packaged web content
   * @param string manifestContent
   *        The manifest content of the cached package.
   * @param string aOrigin
   *        The package origin.
   * @param string aManifestURL
   *        The manifest URL of the package.
   * @param uint32 aAppId
   *        The app id
   * @returns boolean
   **/

  installPackagedWebapp: function(aManifestContent, aOrigin, aManifestURL, aAppId) {

    try {
      let isSuccess = true;
      let manifest = JSON.parse(aManifestContent);

      PermissionsInstaller.installPermissions({
        manifest: manifest,
        manifestURL: aManifestURL,
        origin: aOrigin,
        localId: aAppId,
        isPreinstalled: false,
        isCachedPackage: true
      }, false, function() {
        throw "Error installing permissions in PermissionsInstaller";
      });

      // TODO: register app handlers (system msg)

      return isSuccess;
    }
    catch(ex) {
      Cu.reportError(ex);
      return false;
    }
  },
};

this.NSGetFactory = XPCOMUtils.generateNSGetFactory([InstallPackagedWebapp]);
