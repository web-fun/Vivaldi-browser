// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/content_capture/common/content_capture_struct_traits.h"

#include "mojo/public/cpp/base/string16_mojom_traits.h"
#include "ui/gfx/geometry/mojo/geometry_struct_traits.h"

namespace mojo {

// static
bool StructTraits<content_capture::mojom::ContentCaptureDataDataView,
                  content_capture::ContentCaptureData>::
    Read(content_capture::mojom::ContentCaptureDataDataView data,
         content_capture::ContentCaptureData* out_data) {
  out_data->id = data.id();
  if (!data.ReadValue(&out_data->value))
    return false;
  if (!data.ReadBounds(&out_data->bounds))
    return false;
  if (!data.ReadChildren(&out_data->children))
    return false;

  return true;
}

}  // namespace mojo
