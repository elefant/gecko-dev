Cu.import("resource://gre/modules/NetUtil.jsm");

function debug(s) {
  dump(s + '\n');
}

let gHttpServ = null;

add_test(function() {
  let urlUtils = Cc["@mozilla.org/url-classifier/utils;1"]
                   .getService(Ci.nsIUrlClassifierUtils);

  let request = urlUtils.makeUpdateRequestV4(["goog-phish-shavar"], [""], 1);

  debug("Request: " + JSON.stringify(request));

  let reqeustBase64 = btoa(request);

  let BASE_URL = "http://localhost:4444/";
  // To test with real google server:
  // 1. $ export MOZ_DISABLE_NONLOCAL_CONNECTIONS=0
  // 2. Uncomment the following line to point to real google server
  //BASE_URL = "https://safebrowsing.googleapis.com/";

  // If this API KEY becomes invalid, get a new one at https://goo.gl/YQqhlr
  const API_KEY = "AIzaSyAku-Rm28kEvliP0pVfqRaRMei35bM2l_w";

  const REQUEST_URL = BASE_URL + "v4/encodedUpdates/" +
                      // The following is not important to local server test but
                      // matters if you are testing with real google server.
                      reqeustBase64 + "?" +
                      "alt=proto&client_id=unittest&client_version=1.0&key=" + API_KEY;

  let channel = NetUtil.newChannel({uri: REQUEST_URL, loadUsingSystemPrincipal: true})
                       .QueryInterface(Components.interfaces.nsIHttpChannel);

  debug("REQUEST_URL: " + REQUEST_URL);

  channel.requestMethod = "GET";
  channel.setRequestHeader("Content-Type", "application/x-protobuf", false);
  channel.asyncOpen2({
    data: "",

    onStartRequest: function (request, ctx) {},

    onDataAvailable: function (request, context, stream, offset, count) {
      let data = NetUtil.readInputStreamToString(stream, stream.available());
      this.data += data;
    },

    onStopRequest: function (request, ctx, status) {
      debug('onStopRequest: ' + status);
      debug("Response length: " + this.data.length);

      // Wrap stream to nsIStringInputStream.
      let dataStream = Cc["@mozilla.org/io/string-input-stream;1"]
                         .createInstance(Ci.nsIStringInputStream);
      dataStream.data = this.data;

      let responses = urlUtils.parseUpdateResponseV4(dataStream);

      let r = responses.queryElementAt(0, Ci.nsISBUpdateResponse);
      ok(!!r, "We got a valid response!");
      equal(r.listName, 'goog-phish-shavar');
      equal(r.isFullUpdate, true);
      ok(r.newState);
      equal(r.fixedLengthAdditions.length, 4179840);
      equal(r.removalIndices, null);

      run_next_test();
    }
  });
});

function run_test() {
  gHttpServ = new HttpServer();

  gHttpServ.registerDirectory("/", do_get_cwd());
  gHttpServ.registerPrefixHandler("/v4/encodedUpdates/", function(request, response) {
    let updateResponse = readFileToString('data/test_update_response');

    response.setStatusLine(request.httpVersion, 200, "OK");
    response.bodyOutputStream.write(updateResponse, updateResponse.length);
  });
  gHttpServ.start(4444);

  run_next_test();
}

// Construct an update from a file.
function readFileToString(aFilename) {
  let f = do_get_file(aFilename);
  let stream = Cc["@mozilla.org/network/file-input-stream;1"]
    .createInstance(Ci.nsIFileInputStream);
  stream.init(f, -1, 0, 0);
  let buf = NetUtil.readInputStreamToString(stream, stream.available());
  return buf;
}
