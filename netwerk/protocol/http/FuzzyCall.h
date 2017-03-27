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

#define FUZZY_CALL0(Message)\
  FuzzyCall(Send##Message(), #Message)
#define FUZZY_CALL1(Message, P1)\
  FuzzyCall(Send##Message(FT(P1)), #Message)
#define FUZZY_CALL2(Message, P1, P2)\
  FuzzyCall(Send##Message(FT(P1), FT(P2)), #Message)
#define FUZZY_CALL3(Message, P1, P2, P3)\
  FuzzyCall(Send##Message(FT(P1), FT(P2), FT(P3)), #Message)
#define FUZZY_CALL4(Message, P1, P2, P3, P4)\
  FuzzyCall(Send##Message(FT(P1), FT(P2), FT(P3), FT(P4)), #Message)
#define FUZZY_CALL5(Message, P1, P2, P3, P4, P5)\
  FuzzyCall(Send##Message(FT(P1), FT(P2), FT(P3), FT(P4), FT(P5)), #Message)
#define FUZZY_CALL6(Message, P1, P2, P3, P4, P5, P6)\
  FuzzyCall(Send##Message(FT(P1), FT(P2), FT(P3), FT(P4), FT(P5), FT(P6)), #Message)
#define FUZZY_CALL7(Message, P1, P2, P3, P4, P5, P6, P7)\
  FuzzyCall(Send##Message(FT(P1), FT(P2), FT(P3), FT(P4), FT(P5), FT(P6), FT(P7)), #Message)
#define FUZZY_CALL8(Message, P1, P2, P3, P4, P5, P6, P7, P8)\
  FuzzyCall(Send##Message(FT(P1), FT(P2), FT(P3), FT(P4), FT(P5), FT(P6), FT(P7), FT(P8)), #Message)
#define FUZZY_CALL9(Message, P1, P2, P3, P4, P5, P6, P7, P8, P9)\
  FuzzyCall(Send##Message(FT(P1), FT(P2), FT(P3), FT(P4), FT(P5), FT(P6), FT(P7), FT(P8), FT(P9)), #Message)
#define FUZZY_CALL10(Message, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)\
  FuzzyCall(Send##Message(FT(P1), FT(P2), FT(P3), FT(P4),FT(P5), FT(P6),FT(P7), FT(P8),FT(P9), FT(P10)), #Message)

#endif