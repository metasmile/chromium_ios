// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ios/chrome/browser/favicon/favicon_service_factory.h"

#include "base/memory/ptr_util.h"
#include "base/memory/singleton.h"
#include "components/favicon/core/favicon_service.h"
#include "components/keyed_service/core/service_access_type.h"
#include "components/keyed_service/ios/browser_state_dependency_manager.h"
#include "ios/chrome/browser/browser_state/chrome_browser_state.h"
#include "ios/chrome/browser/favicon/favicon_client_impl.h"
#include "ios/chrome/browser/history/history_service_factory.h"

namespace ios {

// static
favicon::FaviconService* FaviconServiceFactory::GetForBrowserState(
    ios::ChromeBrowserState* browser_state,
    ServiceAccessType access_type) {
  if (!browser_state->IsOffTheRecord()) {
    return static_cast<favicon::FaviconService*>(
        GetInstance()->GetServiceForBrowserState(browser_state, true));
  } else if (access_type == ServiceAccessType::EXPLICIT_ACCESS) {
    return static_cast<favicon::FaviconService*>(
        GetInstance()->GetServiceForBrowserState(
            browser_state->GetOriginalChromeBrowserState(), true));
  }

  // ios::ChromeBrowserState is OffTheRecord without access.
  NOTREACHED() << "ChromeBrowserState is OffTheRecord";
  return nullptr;
}

// static
FaviconServiceFactory* FaviconServiceFactory::GetInstance() {
  return base::Singleton<FaviconServiceFactory>::get();
}

FaviconServiceFactory::FaviconServiceFactory()
    : BrowserStateKeyedServiceFactory(
          "FaviconService",
          BrowserStateDependencyManager::GetInstance()) {
  DependsOn(ios::HistoryServiceFactory::GetInstance());
}

FaviconServiceFactory::~FaviconServiceFactory() {
}

std::unique_ptr<KeyedService> FaviconServiceFactory::BuildServiceInstanceFor(
    web::BrowserState* context) const {
  ios::ChromeBrowserState* browser_state =
      ios::ChromeBrowserState::FromBrowserState(context);
  return base::MakeUnique<favicon::FaviconService>(
      base::MakeUnique<FaviconClientImpl>(),
      ios::HistoryServiceFactory::GetForBrowserState(
          browser_state, ServiceAccessType::EXPLICIT_ACCESS));
}

}  // namespace ios
