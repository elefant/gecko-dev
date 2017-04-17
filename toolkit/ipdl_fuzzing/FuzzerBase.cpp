#include "FuzzerBase.h"
#include "nsComponentManagerUtils.h"
#include "nsPrintfCString.h"
#include "mozilla/ipc/BackgroundChild.h"
#include "mozilla/ipc/PBackgroundChild.h"
#include "mozilla/dom/ContentChild.h"
#include "mozilla/net/NeckoChild.h"

#undef LOG
#define LOG(args) printf_stderr(">>>>>> FuzzerBase <<<<<<< %s\n", (nsPrintfCString args).get())


namespace mozilla {

NS_IMPL_ISUPPORTS(FuzzerBase, nsITimerCallback)

FuzzerBase::FuzzerBase()
  : mTimer(do_CreateInstance("@mozilla.org/timer;1"))
{
}

nsresult
FuzzerBase::Start()
{
  nsresult rv = CreateParentActor();
  if (NS_FAILED(rv)) {
    LOG(("Failed to create parent actor"));
    return rv;
  }

  // The subclass is responsible for resetting this flag when needed.
  // BAD design!!!!
  mIPCIsAlive = true;

  rv = mTimer->InitWithCallback(this, 100, nsITimer::TYPE_REPEATING_SLACK);
  if (NS_FAILED(rv)) {
    LOG(("Failed to init timer for fuzzing."));
  }

  return rv;
}

dom::PContentChild*
FuzzerBase::EnsurePContentChild()
{
  auto cc = dom::ContentChild::GetSingleton();
  MOZ_ASSERT(cc, "Cannot obtain ContentChild");
  return cc;
}

ipc::PBackgroundChild*
FuzzerBase::EnsurePBackgroundChildForCurrentThread()
{
  auto existingBackgroundChild = ipc::BackgroundChild::GetForCurrentThread();
  if (!existingBackgroundChild) {
    existingBackgroundChild =
      ipc::BackgroundChild::SynchronouslyCreateForCurrentThread();
  }
  MOZ_ASSERT(existingBackgroundChild, "Cannot obtain PBackgroundChild");
  return existingBackgroundChild;
}

net::PNeckoChild*
FuzzerBase::EnsurePNeckoChild()
{
  return net::gNeckoChild;
}

/////////////////////////////////////////////////////////////////
// nsITimerCallback implementation
NS_IMETHODIMP
FuzzerBase::Notify(nsITimer *timer)
{
  if (!mIPCIsAlive) {
    LOG(("IPC has been closed."));
    timer->Cancel();
    return NS_OK;
  }

  bool callRet = SendOneIPCMessage();

  if (!callRet) {
    //LOG(("Failed to call %d", callIndex));
    timer->Cancel();
  }

  return NS_OK;
}

} // end of namespace mozilla