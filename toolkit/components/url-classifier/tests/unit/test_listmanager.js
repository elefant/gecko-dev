let listManager = Cc["@mozilla.org/url-classifier/listmanager;1"]
                    .getService(Ci.nsIUrlListManager);

function testRemoveApiKeyFromUrl() {
  let testData = [
    // No query at all.
    ["https://foo.com?aaa=xxx",
     "https://foo.com?aaa=xxx"],

    // No API key.
    ["https://foo.com?aaa=xxx",
     "https://foo.com?aaa=xxx"],

    // Only API key in query.
    ["https://foo.com?key=whateverKey",
     "https://foo.com"],

    // API Key and other non-sensitve info in query.
    ["https://foo.com?key=whateverKey&aaa=xxx",
     "https://foo.com?aaa=xxx"],

    // API Key and other non-sensitve info in query (reversed order).
    ["https://foo.com?aaa=xxx&key=whateverKey",
     "https://foo.com?aaa=xxx"],

    // Mix query with ref.
    ["https://foo.com?abc=xxx&key=whateverKey#whateverRef",
     "https://foo.com?abc=xxx#whateverRef"],
  ];

  testData.forEach(function(v) {
    do_check_eq(listManager.removeSensitiveQuery(v[0]), v[1]);
  });
}

function run_test() {
  testRemoveApiKeyFromUrl();
}
