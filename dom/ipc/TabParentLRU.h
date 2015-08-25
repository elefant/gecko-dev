/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_ipc_TabParentLRU_h
#define mozilla_dom_ipc_TabParentLRU_h

#include "mozilla/StaticPtr.h"

#include "nsTArray.h"
#include "nsWeakPtr.h"

namespace mozilla {
namespace dom {

class ContentParent;
class TabParent;

class TabParentLRU final
{
public:
  static TabParentLRU* GetSingleton();

  void Init();
  void Add(TabParent* aTab);
  void Remove(TabParent* aTab);
  void Remove(ContentParent* aProcess);

private:
  void Evict(nsWeakPtr aPtr);

  static StaticAutoPtr<TabParentLRU> sSingleton;
  uint32_t mLRUSize;
  nsTArray<nsWeakPtr> mLRU;
};

} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_ipc_TabParentLRU_h
