#ifndef mozilla_FuzzTraitsPrimitive_h
#define mozilla_FuzzTraitsPrimitive_h

template<typename ParamType>
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
// Random pick utilities.
template<typename ParamType>
ParamType RandomPick(const std::vector<ParamType>& aCollection)
{
  int pickedIndex = FuzzTraits<uint32_t>::Fuzz() % aCollection.size();
  return aCollection[pickedIndex];
}

//////////////////////////////////////////////////////////////
// Implementation

auto
FuzzTraits<bool>::Fuzz() -> ParamType
{
  return FuzzTraits<uint8_t>::Fuzz() & 0x1;
}

auto
FuzzTraits<int8_t>::Fuzz() -> ParamType
{
  return FuzzTraits<uint8_t>::Fuzz();
}

auto
FuzzTraits<int16_t>::Fuzz() -> ParamType
{
  return FuzzTraits<uint16_t>::Fuzz();
}

auto
FuzzTraits<int32_t>::Fuzz() -> ParamType
{
  return FuzzTraits<uint32_t>::Fuzz();
}

auto
FuzzTraits<int64_t>::Fuzz() -> ParamType
{
  return FuzzTraits<uint64_t>::Fuzz();
}

auto
FuzzTraits<uint8_t>::Fuzz() -> ParamType
{
  return ((uint8_t)rand()) % 256;
}

auto
FuzzTraits<uint16_t>::Fuzz() -> ParamType
{
  uint16_t upper = FuzzTraits<uint8_t>::Fuzz();
  return (upper << 8) + FuzzTraits<uint8_t>::Fuzz();
}

auto
FuzzTraits<uint32_t>::Fuzz() -> ParamType
{
  uint32_t upper = FuzzTraits<uint16_t>::Fuzz();
  return (upper << 16) + FuzzTraits<uint16_t>::Fuzz();
}

auto
FuzzTraits<uint64_t>::Fuzz() -> ParamType
{
  uint64_t upper = FuzzTraits<uint32_t>::Fuzz();
  return (upper << 32) + FuzzTraits<uint32_t>::Fuzz();
}

#endif // mozilla_net_FuzzTraitsPrimitive_h