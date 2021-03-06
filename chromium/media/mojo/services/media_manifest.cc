// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "media/mojo/services/media_manifest.h"

#include "base/no_destructor.h"
#include "media/mojo/interfaces/constants.mojom.h"
#include "media/mojo/interfaces/media_service.mojom.h"
#include "services/service_manager/public/cpp/manifest_builder.h"

#if defined(IS_CHROMECAST)
#include "chromecast/common/mojom/constants.mojom.h"
#endif

namespace media {

const service_manager::Manifest& GetMediaManifest() {
  static base::NoDestructor<service_manager::Manifest> manifest {
    service_manager::ManifestBuilder()
        .WithServiceName(mojom::kMediaServiceName)
        .WithDisplayName("Media Service")
        .ExposeCapability(
            "media:media",
            service_manager::Manifest::InterfaceList<mojom::MediaService>())
#if defined(IS_CHROMECAST)
        .RequireCapability(chromecast::mojom::kChromecastServiceName,
                           "multizone")
#endif
        .Build()
  };
  return *manifest;
}

}  // namespace media
