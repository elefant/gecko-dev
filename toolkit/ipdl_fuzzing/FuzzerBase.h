#ifndef mozilla_BaseFuzzer_h
#define mozilla_BaseFuzzer_h

#include "nsITimer.h"
#include "nsCOMPtr.h"
#include "nsError.h"

namespace mozilla {

namespace ipc { class PBackgroundChild; }
namespace dom { class PContentChild; }
namespace net { class PNeckoChild; }

class FuzzerBase : public nsITimerCallback
{
public:
  FuzzerBase();

  nsresult Start();

  NS_DECL_ISUPPORTS
  NS_DECL_NSITIMERCALLBACK

protected:
  // We cannot override this function on behalf of the subclass
  // so the subclass has the responsibility to reset mIPCIsAlive.
  // (Most likely by overriding the following function in the subclass
  //  implementation. See the following example.)
  //virtual void ActorDestroy(ActorDestroyReason aWhy) override
  //{
  //  mIPCIsAlive = false;
  //}
  bool mIPCIsAlive = false;

protected:
  virtual ~FuzzerBase() {};

  // Utilities for parent protocols (not only top level ones.)
  ipc::PBackgroundChild* EnsurePBackgroundChildForCurrentThread();
  dom::PContentChild* EnsurePContentChild();
  net::PNeckoChild* EnsurePNeckoChild();

private:
  // parentProtocol->SendPFooConstructor().
  virtual nsresult CreateParentActor() = 0;

  // Send one IPC message to the parent.
  // XXX This is a bad name. A better name needed...
  virtual bool SendOneIPCMessage() { return false; };

  nsCOMPtr<nsITimer> mTimer;
};

} // end of namespace mozilla

#endif