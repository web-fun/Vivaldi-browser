// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/autofill_assistant/browser/features.h"

#include "base/feature_list.h"

namespace autofill_assistant {
namespace features {

const base::Feature kAutofillAssistant{"AutofillAssistant",
                                       base::FEATURE_ENABLED_BY_DEFAULT};

// Controls whether to query backend service to start any assisted actions.
const base::Feature kAutofillAssistantChromeEntry{
    "AutofillAssistantChromeEntry", base::FEATURE_DISABLED_BY_DEFAULT};

}  // namespace features
}  // namespace autofill_assistant