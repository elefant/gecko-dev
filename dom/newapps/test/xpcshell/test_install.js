/* -*- indent-tabs-mode: nil; js-indent-level: 2 -*- */
/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/ */

'use strict';

const { classes: Cc, interfaces: Ci, utils: Cu } = Components;

Cu.import('resource://gre/modules/XPCOMUtils.jsm');
Cu.import('resource://gre/modules/Services.jsm');

const mod = Cc['@mozilla.org/newapps/installpackagedwebapp;1']
                  .getService(Ci.nsIInstallPackagedWebapp);

function run_test() {

  let manifest = {
    start_url: "start.html",
    launch_path: "other.html"
  };

  //trigger error at install
  let aOrigin = "";
  let aManifestURL = "";
  let manifestString = "boum"
  let appId = 123456789;
  let res = mod.installPackagedWebapp(manifestString, aOrigin, aManifestURL, appId);
  equal(res, false);

  aOrigin = "http://test.com";
  aManifestURL = "http://test.com/manifest.json";
  manifestString = JSON.stringify(manifest);
  res = mod.installPackagedWebapp(manifestString, aOrigin, aManifestURL, appId);
  equal(res, true);
}
