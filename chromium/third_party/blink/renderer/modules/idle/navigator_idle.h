// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_MODULES_IDLE_NAVIGATOR_IDLE_H_
#define THIRD_PARTY_BLINK_RENDERER_MODULES_IDLE_NAVIGATOR_IDLE_H_

#include "third_party/blink/renderer/platform/wtf/allocator.h"

namespace blink {

class IdleManager;
class Navigator;
class ScriptState;
class WorkerNavigator;

class NavigatorIdle final {
  STATIC_ONLY(NavigatorIdle);

 public:
  static IdleManager* idle(ScriptState*, Navigator&);
  static IdleManager* idle(ScriptState*, WorkerNavigator&);
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_MODULES_IDLE_NAVIGATOR_IDLE_H_
