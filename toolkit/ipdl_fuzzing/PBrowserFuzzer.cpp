#include "PBrowserFuzzer.h"
#include "nsPrintfCString.h"

#include "FuzzTraitsPrimitives.h"
#include "FuzzTraitsCommon.h"

#undef LOG
#define LOG(args) printf_stderr(">>>>>> PBrowserFuzzer <<<<<<< %s\n", (nsPrintfCString args).get())
#include "FuzzyCall.h"

namespace mozilla {

PBrowserFuzzer::PBrowserFuzzer(dom::PBrowserChild* aPBrowserChild)
  : mPBrowserChild(aPBrowserChild)
{
}

nsresult
PBrowserFuzzer::CreateParentActor()
{
  return mPBrowserChild ? NS_OK : NS_ERROR_FAILURE;
}

bool
PBrowserFuzzer::SendOneIPCMessage()
{
  return FUZZY_CALL2(mPBrowserChild, MoveFocus, bool, bool);
}

}

