#ifndef mozilla_PBrowserFuzzer_h
#define mozilla_PBrowserFuzzer_h

#include "FuzzerBase.h"
#include "mozilla/dom/PBrowserChild.h"

namespace mozilla {

class PBrowserFuzzer : public FuzzerBase
                     //, public dom::PBrowserChild
{
public:
  PBrowserFuzzer(dom::PBrowserChild* aPBrowserChild);

private:
  /*
  virtual void ActorDestroy(ActorDestroyReason aWhy) override
  {
    mIPCIsAlive = false;
  }
  */

  uint32_t mCallIndex = 0;
  //const uint32_t kParentMessageNum = 17;

  dom::PBrowserChild* mPBrowserChild;

  virtual nsresult CreateParentActor() override;
  virtual bool SendOneIPCMessage() override;
};

} // end of namespace mozilla

#endif