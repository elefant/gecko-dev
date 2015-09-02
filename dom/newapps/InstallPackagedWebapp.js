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
  classDescription: "InstallPackagedWebapp JavaScript XPCOM Component",
  classID:          Components.ID("{5cc6554a-5421-4a5e-b8c2-c62e8b7f4f3f}"),
  contractID:       "@mozilla.org/newapps/installpackagedwebapp;1",

  QueryInterface: XPCOMUtils.generateQI([Components.interfaces.nsIInstallPackagedWebapp]),
  
  /**
   * Install permissions for signed packaged web content
   * @param string manifestContent
   *        The manifest content of the cached package.
   * @param string aOrigin
   *        The package origin.
   * @param string aManifestURL
   *        The manifest URL of the package.
   * @returns boolean
   **/

  installPackagedWebapp: function(manifestContent, aOrigin, aManifestURL) {

    var aManifest = JSON.parse(manifestContent);
 
    //TODO: get package identifier from the manifest to build
    //the signed packaged origin.

    PermissionsInstaller.installPermissions({
      manifest: aManifest,
      manifestURL: aManifestURL,
      origin: aOrigin,
      isPreinstalled: false,
      kind: "" //empty if not trusted hosted app
    }, false, function() {
      debug("Error installing permissions for " + aOrigin);
      return false;
    });

    //TODO: register app handlers (system msg)
   
    return true; 
  },
};

var NSGetFactory = XPCOMUtils.generateNSGetFactory([InstallPackagedWebapp]);
