# Overview

'safebrowsing.proto' is modified from [1] with the following line addedd:

"package mozilla.safebrowsing;"

to avoid naming pollution. We use this source file along with protobuf compiler (protoc) to generate safebrowsing.pb.h/cc for safebrowsing v4 update and hash completion. The current generated files are compiled by protoc 2.6.1 since the protobuf library in gecko is not upgraded to 3.0 yet.

# Update

If you want to update to the upstream,

1. Checkout the latest one in [2]
2. Use protoc to generate safebrowsing.pb.h and safebrowsing.pb.cc. For example,

$ protoc -I=. --cpp_out="../protobuf/" safebrowsing.proto

(Note that we should use protoc v2.6.1 to compile. You can find the compiled protoc in [3] if you don't have one.)

[1] https://chromium.googlesource.com/chromium/src.git/+/9c4485f1ce7cac7ae82f7a4ae36ccc663afe806c/components/safe_browsing_db/safebrowsing.proto
[2] https://chromium.googlesource.com/chromium/src.git/+/master/components/safe_browsing_db/safebrowsing.proto
[3] http://repo1.maven.org/maven2/com/google/protobuf/protoc