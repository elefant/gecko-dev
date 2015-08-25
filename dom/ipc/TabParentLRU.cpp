/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "TabParentLRU.h"

#include "mozilla/ClearOnShutdown.h"

namespace mozilla {
namespace dom {

StaticAutoPtr<TabParentLRU> TabParentLRU::sSingleton;

TabParentLRU*
TabParentLRU::GetSingleton()
{
  MOZ_ASSERT(NS_IsMainThread());

  if (!sSingleton) {
    sSingleton = new TabParentLRU();
    sSingleton->Init();
    ClearOnShutdown(&sSingleton);
  }
  return sSingleton;
}

void
TabParentLRU::Init()
{
  MOZ_ASSERT(NS_IsMainThread());

  mLRUSize = Preferences::GetUint("nsec.tabs.lru-size", uint32_t(0));
}

void
TabParentLRU::Add(TabParent* aTab)
{
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(XRE_IsParentProcess());

  nsWeakPtr ptr = do_GetWeakReference(static_cast<nsITabParent*>(aTab));
  if (mLRUSize == 0) {
    Evict(ptr);
    return;
  }

  if (mLRU.Contains(ptr)) {
    return;
  }

  if (mLRU.Length() == mLRUSize) {
    nsWeakPtr victim = mLRU.LastElement();
    Evict(victim);
  }
  mLRU.InsertElementAt(0, ptr);
}

void
TabParentLRU::Remove(TabParent* aTab)
{
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(XRE_IsParentProcess());

  if (mLRUSize == 0) {
    return;
  }

  nsWeakPtr ptr = do_GetWeakReference(static_cast<nsITabParent*>(aTab));
  mLRU.RemoveElement(ptr);
}

void
TabParentLRU::Remove(ContentParent* aProcess)
{
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(XRE_IsParentProcess());

  nsAutoTArray<PBrowserParent*, 8> parents;
  parents.AppendElements(aProcess->ManagedPBrowserParent());
  for (auto parent: parents) {
    TabParent* tab = TabParent::GetFrom(parent);
    nsWeakPtr ptr = do_GetWeakReference(static_cast<nsITabParent*>(tab));
    Evict(ptr);
  }
}

void
TabParentLRU::Evict(nsWeakPtr aPtr)
{
  nsCOMPtr<nsITabParent> tab = do_QueryReferent(aPtr);
  if (tab) {
    TabParent::GetFrom(tab)->Destroy();
  }
  mLRU.RemoveElement(aPtr);
}

} // namespace dom
} // namespace mozilla
