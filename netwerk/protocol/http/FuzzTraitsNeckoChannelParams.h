#ifndef mozilla_net_FuzzTraitsNeckoChannelParams_h
#define mozilla_net_FuzzTraitsNeckoChannelParams_h

#include "mozilla/net/NeckoChannelParams.h"
#include "nsIStandardURL.h"
#include "FuzzTraitsPrimitive.h"

using namespace mozilla::dom;
using namespace mozilla::ipc;

using mozilla::OriginAttributes;

template<>
struct FuzzTraits<nsresult>
{
  using ParamType = nsresult;
  static ParamType Fuzz();
};

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
struct FuzzTraits<OriginAttributes>
{
  using ParamType = OriginAttributes;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<nsCString>
{
  using ParamType = nsCString;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<mozilla::net::RequestHeaderTuples>
{
  using ParamType = mozilla::net::RequestHeaderTuples;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<mozilla::net::nsHttpResponseHead>
{
  using ParamType = mozilla::net::nsHttpResponseHead;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<ContentPrincipalInfoOriginNoSuffix>
{
  using ParamType = ContentPrincipalInfoOriginNoSuffix;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<PrincipalInfo>
{
  using ParamType = PrincipalInfo;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<OptionalPrincipalInfo>
{
  using ParamType = OptionalPrincipalInfo;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<ContentPrincipalInfo>
{
  using ParamType = ContentPrincipalInfo;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<SystemPrincipalInfo>
{
  using ParamType = SystemPrincipalInfo;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<NullPrincipalInfo>
{
  using ParamType = NullPrincipalInfo;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<ExpandedPrincipalInfo>
{
  using ParamType = ExpandedPrincipalInfo;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<OptionalIPCStream>
{
  using ParamType = OptionalIPCStream;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<mozilla::net::HttpChannelOpenArgs>
{
  using ParamType = mozilla::net::HttpChannelOpenArgs;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<mozilla::net::LoadInfoArgs>
{
  using ParamType = mozilla::net::LoadInfoArgs;
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
FuzzTraits<nsresult>::Fuzz() -> ParamType
{
  return NS_OK;
}

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
FuzzTraits<nsCString>::Fuzz() -> ParamType
{
  return NS_LITERAL_CSTRING("CatchMeIfYouCan");
}

auto
FuzzTraits<mozilla::net::RequestHeaderTuples>::Fuzz() -> ParamType
{
  return mozilla::net::RequestHeaderTuples();
}

auto
FuzzTraits<mozilla::net::nsHttpResponseHead>::Fuzz() -> ParamType
{
  return mozilla::net::nsHttpResponseHead();
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
FuzzTraits<OriginAttributes>::Fuzz() -> ParamType
{
  return OriginAttributes(FuzzTraits<uint32_t>::Fuzz(),
                          FuzzTraits<bool>::Fuzz());
}

auto
FuzzTraits<ContentPrincipalInfoOriginNoSuffix>::Fuzz() -> ParamType
{
  //return FuzzTraits<void_t>::Fuzz();
  return FuzzTraits<nsCString>::Fuzz();
}

auto
FuzzTraits<OptionalPrincipalInfo>::Fuzz() -> ParamType
{
  //return FuzzTraits<void_t>::Fuzz();
  return FuzzTraits<PrincipalInfo>::Fuzz();
}

auto
FuzzTraits<PrincipalInfo>::Fuzz() -> ParamType
{
  //return FuzzTraits<ContentPrincipalInfo>::Fuzz();
  //return FuzzTraits<SystemPrincipalInfo>::Fuzz();
  //return FuzzTraits<NullPrincipalInfo>::Fuzz();
  //return FuzzTraits<ExpandedPrincipalInfo>::Fuzz();
  return FuzzTraits<ContentPrincipalInfo>::Fuzz();
}

auto
FuzzTraits<ContentPrincipalInfo>::Fuzz() -> ParamType
{
  return ContentPrincipalInfo(FuzzTraits<OriginAttributes>::Fuzz(),
                              FuzzTraits<ContentPrincipalInfoOriginNoSuffix>::Fuzz(),
                              FuzzTraits<nsCString>::Fuzz());
}

auto
FuzzTraits<SystemPrincipalInfo>::Fuzz() -> ParamType
{
  return SystemPrincipalInfo();
}

auto
FuzzTraits<NullPrincipalInfo>::Fuzz() -> ParamType
{
  return NullPrincipalInfo();
}

auto
FuzzTraits<ExpandedPrincipalInfo>::Fuzz() -> ParamType
{
  return ExpandedPrincipalInfo();
}

auto
FuzzTraits<mozilla::net::LoadInfoArgs>::Fuzz() -> ParamType
{
  mozilla::net::LoadInfoArgs args;

  //OptionalPrincipalInfo requestingPrincipalInfo;
  args.requestingPrincipalInfo() = FuzzTraits<OptionalPrincipalInfo>::Fuzz();

  //PrincipalInfo         triggeringPrincipalInfo;
  args.triggeringPrincipalInfo() = FuzzTraits<PrincipalInfo>::Fuzz();

  //OptionalPrincipalInfo principalToInheritInfo;
  args.principalToInheritInfo() = FuzzTraits<OptionalPrincipalInfo>::Fuzz();

  //OptionalPrincipalInfo sandboxedLoadingPrincipalInfo;
  args.sandboxedLoadingPrincipalInfo() = FuzzTraits<OptionalPrincipalInfo>::Fuzz();

  //uint32_t              securityFlags;
  //uint32_t              contentPolicyType;
  //uint32_t              tainting;
  //bool                  upgradeInsecureRequests;
  //bool                  verifySignedContent;
  //bool                  enforceSRI;
  //bool                  forceInheritPrincipalDropped;
  //uint64_t              innerWindowID;
  //uint64_t              outerWindowID;
  //uint64_t              parentOuterWindowID;
  //uint64_t              frameOuterWindowID;
  //bool                  enforceSecurity;
  //bool                  initialSecurityCheckDone;
  //bool                  isInThirdPartyContext;
  //OriginAttributes      originAttributes;
  args.originAttributes() = FuzzTraits<OriginAttributes>::Fuzz();

  //PrincipalInfo[]       redirectChainIncludingInternalRedirects;
  //PrincipalInfo[]       redirectChain;
  //nsCString[]           corsUnsafeHeaders;
  //bool                  forcePreflight;
  //bool                  isPreflight;
  //bool                  forceHSTSPriming;
  //bool                  mixedContentWouldBlock;

  return args;
}

auto
FuzzTraits<mozilla::net::OptionalLoadInfoArgs>::Fuzz() -> ParamType
{
  //return mozilla::void_t();
  return FuzzTraits<mozilla::net::LoadInfoArgs>::Fuzz();
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