#ifndef mozilla_FuzzTraitsPrimitive_h
#define mozilla_FuzzTraitsPrimitive_h

template<class T>
struct FuzzTraits {};

template<>
struct FuzzTraits<bool>
{
  using ParamType = bool;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<int8_t>
{
  using ParamType = int8_t;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<int16_t>
{
  using ParamType = int16_t;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<int32_t>
{
  using ParamType = int32_t;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<int64_t>
{
  using ParamType = int64_t;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<uint8_t>
{
  using ParamType = uint8_t;
  static ParamType Fuzz();
};

template<>
struct FuzzTraits<uint16_t>
{
  using ParamType = uint16_t;
  static ParamType Fuzz();
};

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
FuzzTraits<bool>::Fuzz() -> ParamType
{
  return false;
}

auto
FuzzTraits<int8_t>::Fuzz() -> ParamType
{
  return -99;
}

auto
FuzzTraits<int16_t>::Fuzz() -> ParamType
{
  return 65001;
}

auto
FuzzTraits<int32_t>::Fuzz() -> ParamType
{
  return 5566;
}

auto
FuzzTraits<int64_t>::Fuzz() -> ParamType
{
  return 7788;
}

auto
FuzzTraits<uint8_t>::Fuzz() -> ParamType
{
  return 8;
}

auto
FuzzTraits<uint16_t>::Fuzz() -> ParamType
{
  return 69;
}

auto
FuzzTraits<uint32_t>::Fuzz() -> ParamType
{
  return 7788;
}

auto
FuzzTraits<uint64_t>::Fuzz() -> ParamType
{
  return 5566;
}

#endif // mozilla_net_FuzzTraitsPrimitive_h