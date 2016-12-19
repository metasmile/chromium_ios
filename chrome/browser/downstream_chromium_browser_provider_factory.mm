// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/memory/ptr_util.h"
#include "ios/chrome/browser/downstream_chromium_browser_provider.h"
#include "ios/public/provider/chrome/browser/chrome_browser_provider.h"

namespace ios {
std::unique_ptr<ChromeBrowserProvider> CreateChromeBrowserProvider() {
  return base::MakeUnique<DownstreamChromiumBrowserProvider>();
}
}  // namespace ios
