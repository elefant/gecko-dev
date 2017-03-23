#ifndef mozilla_net_FuzzTraitsNeckoChannelParams_h
#define mozilla_net_FuzzTraitsNeckoChannelParams_h

#include "mozilla/net/NeckoChannelParams.h"
#include "nsIStandardURL.h"
#include "FuzzTraitsPrimitive.h"

using namespace mozilla::dom;
using namespace mozilla::ipc;

template<>
struct FuzzTraits<SimpleURIParams>
{
  using ParamType = SimpleURIParams;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<StandardURLParams>
{
  using ParamType = StandardURLParams;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<URIParams>
{
  using ParamType = URIParams;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<OptionalURIParams>
{
  using ParamType = OptionalURIParams;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<mozilla::net::HttpChannelOpenArgs>
{
  using ParamType = mozilla::net::HttpChannelOpenArgs;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<PBrowserOrId>
{
  using ParamType = PBrowserOrId;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<TabId>
{
  using ParamType = TabId;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<OptionalIPCStream>
{
  using ParamType = OptionalIPCStream;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<mozilla::net::OptionalLoadInfoArgs>
{
  using ParamType = mozilla::net::OptionalLoadInfoArgs;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<mozilla::net::OptionalHttpResponseHead>
{
  using ParamType = mozilla::net::OptionalHttpResponseHead;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<mozilla::net::OptionalCorsPreflightArgs>
{
  using ParamType = mozilla::net::OptionalCorsPreflightArgs;
  static ParamType Fuzz();
};

////////////////////////////////////////////////////////////////////////////.
// Implementations

auto
FuzzTraits<SimpleURIParams>::Fuzz() -> ParamType
{
  SimpleURIParams p;
  p.scheme() = NS_LITERAL_CSTRING("https");
  p.path() = NS_LITERAL_CSTRING("www.mozilla.org");
  return p;
}

auto
FuzzTraits<StandardURLParams>::Fuzz() -> ParamType
{
  StandardURLParams p;
  p.spec() = NS_LITERAL_CSTRING("www.mozilla.org");
  p.urlType() = nsIStandardURL::URLTYPE_STANDARD;
  p.hostEncoding() = 1; // nsIStandardURL::eEncoding_ASCII
  return p;
}

auto
FuzzTraits<URIParams>::Fuzz() -> ParamType
{
  //return FuzzTraits<SimplURLParams>::Fuzz();
  return FuzzTraits<StandardURLParams>::Fuzz();
}

auto
FuzzTraits<OptionalURIParams>::Fuzz() -> ParamType
{
  //return FuzzTraits<void_t>::Fuzz();
  return FuzzTraits<URIParams>::Fuzz();
}

auto
FuzzTraits<PBrowserOrId>::Fuzz() -> ParamType
{
  return FuzzTraits<TabId>::Fuzz();
}

auto
FuzzTraits<TabId>::Fuzz() -> ParamType
{
  return TabId(FuzzTraits<uint64_t>::Fuzz());
}

auto
FuzzTraits<OptionalIPCStream>::Fuzz() -> ParamType
{
  return mozilla::void_t();
}

auto
FuzzTraits<mozilla::net::OptionalLoadInfoArgs>::Fuzz() -> ParamType
{
  return mozilla::void_t();
}

auto
FuzzTraits<mozilla::net::OptionalHttpResponseHead>::Fuzz() -> ParamType
{
  return mozilla::void_t();
}

auto
FuzzTraits<mozilla::net::OptionalCorsPreflightArgs>::Fuzz() -> ParamType
{
  return mozilla::void_t();
}

auto
FuzzTraits<mozilla::net::HttpChannelOpenArgs>::Fuzz() -> ParamType
{
  mozilla::net::HttpChannelOpenArgs openArgs;

  openArgs.uri() = FuzzTraits<URIParams>::Fuzz();
  openArgs.original() = FuzzTraits<OptionalURIParams>::Fuzz();
  openArgs.doc() = FuzzTraits<OptionalURIParams>::Fuzz();
  openArgs.referrer() = FuzzTraits<OptionalURIParams>::Fuzz();

  //uint32_t                    referrerPolicy;
  openArgs.referrerPolicy() = FuzzTraits<uint32_t>::Fuzz();

  //OptionalURIParams           apiRedirectTo;
  openArgs.apiRedirectTo() = FuzzTraits<OptionalURIParams>::Fuzz();

  //OptionalURIParams           topWindowURI;
  openArgs.topWindowURI() = FuzzTraits<OptionalURIParams>::Fuzz();

  //uint32_t                    loadFlags;
  //RequestHeaderTuples         requestHeaders;
  //nsCString                   requestMethod;
  //OptionalIPCStream           uploadStream;
  openArgs.uploadStream() = FuzzTraits<OptionalIPCStream>::Fuzz();

  //bool                        uploadStreamHasHeaders;
  //int16_t                     priority;
  //uint32_t                    classOfService;
  //uint8_t                     redirectionLimit;
  //bool                        allowSTS;
  //uint32_t                    thirdPartyFlags;
  //bool                        resumeAt;
  //uint64_t                    startPos;
  //nsCString                   entityID;
  //bool                        chooseApplicationCache;
  //nsCString                   appCacheClientID;
  //bool                        allowSpdy;
  //bool                        allowAltSvc;
  //bool                        beConservative;
  //OptionalLoadInfoArgs        loadInfo;
  openArgs.loadInfo() = FuzzTraits<mozilla::net::OptionalLoadInfoArgs>::Fuzz();

  //OptionalHttpResponseHead    synthesizedResponseHead;
  openArgs.synthesizedResponseHead() = FuzzTraits<mozilla::net::OptionalHttpResponseHead>::Fuzz();

  //nsCString                   synthesizedSecurityInfoSerialization;
  //uint32_t                    cacheKey;
  //nsCString                   requestContextID;
  //OptionalCorsPreflightArgs   preflightArgs;
  openArgs.preflightArgs() = FuzzTraits<mozilla::net::OptionalCorsPreflightArgs>::Fuzz();

  //uint32_t                    initialRwin;
  //bool                        blockAuthPrompt;
  //bool                        suspendAfterSynthesizeResponse;
  //bool                        allowStaleCacheContent;
  //nsCString                   contentTypeHint;
  //nsCString                   channelId;
  //uint64_t                    contentWindowId;
  //nsCString                   preferredAlternativeType;
  //uint64_t                    topLevelOuterContentWindowId;

  return openArgs;
}

#endif