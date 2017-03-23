#ifndef mozilla_FuzzTraitsPrimitive_h
#define mozilla_FuzzTraitsPrimitive_h

template<class T>
struct FuzzTraits {};

template<>
struct FuzzTraits<uint32_t>
{
  using ParamType = uint32_t;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<uint64_t>
{
  using ParamType = uint64_t;
  static ParamType Fuzz();
};

//////////////////////////////////////////////////////////////
// Implementation

auto
FuzzTraits<uint64_t>::Fuzz() -> ParamType
{
  return 5566;
}

auto
FuzzTraits<uint32_t>::Fuzz() -> ParamType
{
  return 7788;
}

#endif // mozilla_net_FuzzTraitsPrimitive_h