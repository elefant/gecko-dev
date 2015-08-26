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
  let manifestString = '{"test": "test"}';
  let aOrigin = "http://test.com";
  let aManifestURL = "http://test.com/manifest.json";
  let res = mod.installPackagedWebapp(manifestString, aOrigin, aManifestURL); 
  //equal(res, true);
}
