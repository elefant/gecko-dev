MARIONETTE_TIMEOUT = 60000;

let HOSTAPD_CONFIG_PATH = '/data/misc/wifi/hostapd/';

let HOSTAPD_COMMON_CONFIG = {
  driver: 'test',
  ctrl_interface: '/data/misc/wifi/hostapd',
  test_socket: 'DIR:/data/misc/wifi/sockets',
  hw_mode: 'b',
  channel: '2',
};

let HOSTAPD_CONFIG_LIST = [
  {
    ssid: 'ap0',
  },

  {
    ssid: 'ap1',
    wpa: 1,
    wpa_pairwise: 'TKIP CCMP',
    wpa_passphrase: '12345678',
  },

  {
    ssid: 'ap2',
    wpa: 2,
    rsn_pairwise: 'CCMP',
    wpa_passphrase: '12345678',
  },

];

function startTest() {
  let stockHostapds;

  // Get stock hostapd for later use.
  getProcessDetail('hostapd', function(detail) {
    stockHostapds = detail;

    startHostapds(function() {
      getProcessDetail('hostapd', function(detail) {
        is(detail.length, HOSTAPD_CONFIG_LIST.length + stockHostapds.length);
        onHostpadsStarted();
      });
    });
  });

  function onHostpadsStarted() {
    let wifiManager = window.navigator.mozWifiManager;
    ok(wifiManager, 'window.navigator.mozWifiManager');

    if (!wifiManager.enabled) {
      wifiManager.onenabled = function() {
        ok(wifiManager.enabled, "wifiManager.enabled");
        checkNetworks();
      }
      // Enable wifi.
      navigator.mozSettings.createLock().set({
        'wifi.enabled': true
      }).onerror = function() {
        ok(false, 'Unable to enable wifi');
        endTest();
      };
    } else { // Wifi already enabled.
      checkNetworks();
    }

    function getTestHostapdIndex(aSsid) {
      for (let i = 0; i < HOSTAPD_CONFIG_LIST.length; i++) {
        if (HOSTAPD_CONFIG_LIST[i].ssid === aSsid) {
          return i;
        }
      }
      log(aSsid + ' is stock hostap!');
      return -1;
    }

    function checkNetworks() {
      let request = wifiManager.getNetworks();
      request.onsuccess = function() {
        log("Networks: " + JSON.stringify(request.result));
        ok(request.result.length);

        // Chooose the first non-stock hostapd to test.
        for (let i = 0; i < request.result.length; i++) {
          let network = request.result[i];
          if (-1 !== getTestHostapdIndex(network.ssid)) {
            testAssociate(network);
            return;
          }
        }

        ok(false, 'Test hostapd NOT found.');
        endTest();
      }
    }

    function testAssociate(aNetwork) {
      log('testAssociate() with ' + aNetwork.ssid);
      setPasswordIfNeeded(aNetwork);
      wifiManager.onstatuschange = function(event) {
        log("event.status: " + event.status);
        log("event.network: " + JSON.stringify(event.network));
        if ("connected" === event.status) {
          ok(true, 'We got connected!');
          testDisableWifi();
        }
      };

      let req = wifiManager.associate(aNetwork);
      req.onsuccess = function() {
        ok(true);
      }
      req.onerror = function() {
        ok(false);
      }
    }

    function testDisableWifi() {
      log('testDisableWifi()');
      navigator.mozSettings.createLock().set({
        'wifi.enabled': false
      }).onerror = function() {
        ok(false, 'Unable to disable wifi');
        endTest();
      };

      wifiManager.ondisabled = function() {
        ok(true, 'Wifi successfully disabled!');
        endTest();
      }
    }

    function setPasswordIfNeeded(aNetwork) {
      let i = getTestHostapdIndex(aNetwork.ssid);
      if (-1 === i) {
        log('unknown ssid: ' + aNetwork.ssid);
        endTest();
        return;
      }

      if (!aNetwork.security.length) {
        return;
      }

      let security = aNetwork.security[0];
      if (/PSK$/.test(security)) {
        aNetwork.psk = HOSTAPD_CONFIG_LIST[i].wpa_passphrase;
        aNetwork.keyManagement = 'WPA-PSK';
      } else if (/WEP$/.test(security)) {
        aNetwork.wep = HOSTAPD_CONFIG_LIST[i].wpa_passphrase;
        aNetwork.keyManagement = 'WEP';
      }
    }

    function isStockHostapd(pid) {
      for (let i = 0; i < stockHostapds.length; i++) {
        if (stockHostapds[i].pid === pid) {
          return true;
        }
      }
      return false;
    }

    function killNonStockHostapds(aCallback) {
      let done = 0;
      getProcessDetail('hostapd', function(runningHostapds) {
        for (let i = 0; i < runningHostapds.length; i++) {
          if (!isStockHostapd(runningHostapds[i].pid)) {
            runEmulatorShell(["kill", "-9", runningHostapds[i].pid], function() {
              if (++done === runningHostapds.length) {
                aCallback();
              }
            });
          } else {
            if (++done === runningHostapds.length) {
              aCallback();
            }
          }
        }
      });
    }

    function endTest() {
      killNonStockHostapds(function() {
        getProcessDetail('hostapd', function(detail) {
          is(detail.length, stockHostapds.length);
          finish();
        });
      });
    }

  }
}

function writeFile(aFileName, aContent, aCallback) {
  // Workaround....
  if (-1 === aContent.indexOf(' ')) {
    aContent = '"' + aContent + '"';
  }
  runEmulatorShell(["echo", aContent, ">", aFileName], function(result) {
    aCallback(result);
  });
}

function writeHostapdConfFile(aFileName, aConfig, aCallback) {
  let content = "";
  for (let key in aConfig) {
    if (aConfig.hasOwnProperty(key)) {
      content += (key + '=' + aConfig[key] + '\n');
    }
  }
  writeFile(aFileName, content, aCallback);
}

function shallowClone(aObject) {
  return JSON.parse(JSON.stringify(aObject));
}

function startHostapds(aCallback) {
  startHostapd(0, function() {
    startHostapd(1, function() {
      startHostapd(2, function() {
        aCallback();
      });
    });
  });
}

function startHostapd(aIndex, aCallback) {
  let config = shallowClone(HOSTAPD_COMMON_CONFIG);
  for (let key in HOSTAPD_CONFIG_LIST[aIndex]) {
    config[key] = HOSTAPD_CONFIG_LIST[aIndex][key];
  }

  // MUST use this format because wpa_supplicant't test driver hard-codes it.
  config.interface = 'AP-' + aIndex;

  let configFileName = HOSTAPD_CONFIG_PATH + 'ap' + aIndex + '.conf';
  writeHostapdConfFile(configFileName, config, function() {
    runEmulatorShell(["hostapd", "-B", configFileName], function(result) {
      aCallback();
    });
  });
}

// Return an array of process detail:
//   - pname: full process name containing the given short process name.
//   - pid: process id.
function getProcessDetail(aProcessName, aCallback) {
  let detail = [];
  runEmulatorShell(["ps"], function(processes) {
    /*
    USER     PID   PPID  VSIZE  RSS     WCHAN    PC         NAME
    root      1     0     284    204   c009e6c4 0000deb4 S /init
    root      2     0     0      0     c0052c64 00000000 S kthreadd
    root      3     2     0      0     c0044978 00000000 S ksoftirqd/0
    */
    for (let i = 0; i < processes.length; i++) {
      let tokens = processes[i].split(/\s+/);
      let pname = tokens[tokens.length - 1];
      let pid = tokens[1];
      if (-1 !== pname.indexOf(aProcessName)) {
        detail.push({ pname: pname, pid: pid });
      }
    }

    aCallback(detail);
  });
}

// The permission required by this test case is wifi-manage and settings-write.
SpecialPowers.pushPermissions([{ "type": "wifi-manage", "allow": 1, "context": window.document }], function() {
  SpecialPowers.pushPermissions([{'type': 'settings-write', 'allow': true, 'context': document}], startTest);
});
