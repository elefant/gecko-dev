#ifndef mozilla_FuzzTraitsCommon_h
#define mozilla_FuzzTraitsCommon_h

#include "mozilla/fuzzing/FuzzTraitsPrimitives.h"
#include "mozilla/ipc/BackgroundUtils.h"
#include "mozilla/ipc/PBackgroundSharedTypes.h"

using namespace mozilla::dom;
using namespace mozilla::ipc;

using mozilla::OriginAttributes;

template<>
struct FuzzTraits<nsresult>
{
  using ParamType = nsresult;
  inline static ParamType Fuzz();
};

template<>
struct FuzzTraits<OriginAttributes>
{
  using ParamType = OriginAttributes;
  inline static ParamType Fuzz();
};

template<>
struct FuzzTraits<nsCString>
{
  using ParamType = nsCString;
  inline static ParamType Fuzz();
};

template<>
struct FuzzTraits<PrincipalInfo>
{
  using ParamType = PrincipalInfo;
  inline static ParamType Fuzz();
};

template<>
struct FuzzTraits<OptionalPrincipalInfo>
{
  using ParamType = OptionalPrincipalInfo;
  inline static ParamType Fuzz();
};

template<>
struct FuzzTraits<ContentPrincipalInfo>
{
  using ParamType = ContentPrincipalInfo;
  inline static ParamType Fuzz();
};

template<>
struct FuzzTraits<SystemPrincipalInfo>
{
  using ParamType = SystemPrincipalInfo;
  inline static ParamType Fuzz();
};

template<>
struct FuzzTraits<NullPrincipalInfo>
{
  using ParamType = NullPrincipalInfo;
  inline static ParamType Fuzz();
};

template<>
struct FuzzTraits<ExpandedPrincipalInfo>
{
  using ParamType = ExpandedPrincipalInfo;
  inline static ParamType Fuzz();
};

template<>
struct FuzzTraits<ContentPrincipalInfoOriginNoSuffix>
{
  using ParamType = ContentPrincipalInfoOriginNoSuffix;
  inline static ParamType Fuzz();
};


////////////////////////////////////////////////////////////////
// Implementations

auto
FuzzTraits<nsresult>::Fuzz() -> ParamType
{
  return NS_OK;
}

auto
FuzzTraits<nsCString>::Fuzz() -> ParamType
{
  return NS_LITERAL_CSTRING("CatchMeIfYouCan");
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
  return RandomPick<ParamType>(
  {
    //mozilla::void_t(),
    nsCString("https://mozilla.org")
  });
}

auto
FuzzTraits<OptionalPrincipalInfo>::Fuzz() -> ParamType
{
  return RandomPick<ParamType>(
  {
    FuzzTraits<PrincipalInfo>::Fuzz(),
    //mozilla::void_t()
  });
}

auto
FuzzTraits<PrincipalInfo>::Fuzz() -> ParamType
{
  return RandomPick<ParamType>(
  {
    FuzzTraits<ContentPrincipalInfo>::Fuzz()
    //FuzzTraits<ContentPrincipalInfo>::Fuzz(),
    //FuzzTraits<SystemPrincipalInfo>::Fuzz(),
    //FuzzTraits<NullPrincipalInfo>::Fuzz(),
    //FuzzTraits<ExpandedPrincipalInfo>::Fuzz(),
  });
}

auto
FuzzTraits<ContentPrincipalInfo>::Fuzz() -> ParamType
{
  return RandomPick<ParamType>(
  {
    // By spec only.
    ContentPrincipalInfo(FuzzTraits<OriginAttributes>::Fuzz(),
                         mozilla::void_t(),
                         nsCString("https://mozilla.org")),

    // By OA + originNoSuffix.
    ContentPrincipalInfo(FuzzTraits<OriginAttributes>::Fuzz(),
                         nsCString("https://mozilla.org"),
                         nsCString("https://mozilla.org"))
  });
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

#endif