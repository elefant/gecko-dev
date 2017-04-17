#ifndef FUZZY_CALL_H
#define FUZZY_CALL_H

// TODO: Template-ize the macros.

namespace {

bool FuzzyCall(bool aCallResult, const char* aMessageName)
{
  LOG(("Sending %s...", aMessageName));
  return aCallResult;
}

}

#define FT(P) FuzzTraits<P>::Fuzz()

#define FUZZY_CALL0(obj, Message)\
  FuzzyCall(obj->Send##Message(), #Message)
#define FUZZY_CALL1(obj, Message, P1)\
  FuzzyCall(obj->Send##Message(FT(P1)), #Message)
#define FUZZY_CALL2(obj, Message, P1, P2)\
  FuzzyCall(obj->Send##Message(FT(P1), FT(P2)), #Message)
#define FUZZY_CALL3(obj, Message, P1, P2, P3)\
  FuzzyCall(obj->Send##Message(FT(P1), FT(P2), FT(P3)), #Message)
#define FUZZY_CALL4(obj, Message, P1, P2, P3, P4)\
  FuzzyCall(obj->Send##Message(FT(P1), FT(P2), FT(P3), FT(P4)), #Message)
#define FUZZY_CALL5(obj, Message, P1, P2, P3, P4, P5)\
  FuzzyCall(obj->Send##Message(FT(P1), FT(P2), FT(P3), FT(P4), FT(P5)), #Message)
#define FUZZY_CALL6(obj, Message, P1, P2, P3, P4, P5, P6)\
  FuzzyCall(obj->Send##Message(FT(P1), FT(P2), FT(P3), FT(P4), FT(P5), FT(P6)), #Message)
#define FUZZY_CALL7(obj, Message, P1, P2, P3, P4, P5, P6, P7)\
  FuzzyCall(obj->Send##Message(FT(P1), FT(P2), FT(P3), FT(P4), FT(P5), FT(P6), FT(P7)), #Message)
#define FUZZY_CALL8(obj, Message, P1, P2, P3, P4, P5, P6, P7, P8)\
  FuzzyCall(obj->Send##Message(FT(P1), FT(P2), FT(P3), FT(P4), FT(P5), FT(P6), FT(P7), FT(P8)), #Message)
#define FUZZY_CALL9(obj, Message, P1, P2, P3, P4, P5, P6, P7, P8, P9)\
  FuzzyCall(obj->Send##Message(FT(P1), FT(P2), FT(P3), FT(P4), FT(P5), FT(P6), FT(P7), FT(P8), FT(P9)), #Message)
#define FUZZY_CALL10(obj, Message, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)\
  FuzzyCall(obj->Send##Message(FT(P1), FT(P2), FT(P3), FT(P4),FT(P5), FT(P6),FT(P7), FT(P8),FT(P9), FT(P10)), #Message)

#endif