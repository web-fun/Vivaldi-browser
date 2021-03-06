// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/platform/instrumentation/resource_coordinator/frame_resource_coordinator.h"

#include <memory>

#include "base/memory/ptr_util.h"
#include "services/service_manager/public/cpp/interface_provider.h"
#include "third_party/blink/renderer/platform/runtime_enabled_features.h"

namespace blink {

namespace {

using resource_coordinator::mojom::InterventionPolicy;
using resource_coordinator::mojom::PolicyControlledIntervention;

}  // namespace

// static
std::unique_ptr<FrameResourceCoordinator> FrameResourceCoordinator::MaybeCreate(
    service_manager::InterfaceProvider* interface_provider) {
  if (!RuntimeEnabledFeatures::PerformanceManagerInstrumentationEnabled())
    return nullptr;

  return base::WrapUnique(new FrameResourceCoordinator(interface_provider));
}

FrameResourceCoordinator::FrameResourceCoordinator(
    service_manager::InterfaceProvider* interface_provider) {
  interface_provider->GetInterface(mojo::MakeRequest(&service_));
  DCHECK(service_);
}

FrameResourceCoordinator::~FrameResourceCoordinator() = default;

void FrameResourceCoordinator::SetNetworkAlmostIdle(bool idle) {
  service_->SetNetworkAlmostIdle(idle);
}

void FrameResourceCoordinator::SetLifecycleState(
    resource_coordinator::mojom::LifecycleState state) {
  service_->SetLifecycleState(state);
}

void FrameResourceCoordinator::SetHasNonEmptyBeforeUnload(
    bool has_nonempty_beforeunload) {
  service_->SetHasNonEmptyBeforeUnload(has_nonempty_beforeunload);
}

void FrameResourceCoordinator::SetInterventionPolicy(
    PolicyControlledIntervention intervention,
    InterventionPolicy policy) {
  service_->SetInterventionPolicy(intervention, policy);
}

void FrameResourceCoordinator::OnNonPersistentNotificationCreated() {
  service_->OnNonPersistentNotificationCreated();
}

}  // namespace blink
