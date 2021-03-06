// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "ios/chrome/browser/content_suggestions/content_suggestions_coordinator.h"

#include "base/mac/scoped_nsobject.h"
#import "ios/chrome/browser/content_suggestions/content_suggestions_mediator.h"
#include "ios/chrome/browser/ntp_snippets/ios_chrome_content_suggestions_service_factory.h"
#import "ios/chrome/browser/ui/content_suggestions/content_suggestions_commands.h"
#import "ios/chrome/browser/ui/content_suggestions/content_suggestions_view_controller.h"
#include "ios/chrome/grit/ios_strings.h"
#include "ui/base/l10n/l10n_util.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

@interface ContentSuggestionsCoordinator ()<ContentSuggestionsCommands> {
  UINavigationController* _navigationController;
  ContentSuggestionsMediator* _contentSuggestionsMediator;
}

@end

@implementation ContentSuggestionsCoordinator

@synthesize browserState = _browserState;
@synthesize visible = _visible;

- (void)start {
  if (self.visible || !self.browserState) {
    // Prevent this coordinator from being started twice in a row or without a
    // browser state.
    return;
  }

  _visible = YES;

  _contentSuggestionsMediator = [[ContentSuggestionsMediator alloc]
      initWithContentService:IOSChromeContentSuggestionsServiceFactory::
                                 GetForBrowserState(self.browserState)];

  ContentSuggestionsViewController* suggestionsViewController =
      [[ContentSuggestionsViewController alloc]
          initWithStyle:CollectionViewControllerStyleDefault
             dataSource:_contentSuggestionsMediator];

  suggestionsViewController.suggestionCommandHandler = self;
  _navigationController = [[UINavigationController alloc]
      initWithRootViewController:suggestionsViewController];

  suggestionsViewController.navigationItem.leftBarButtonItem =
      [[UIBarButtonItem alloc]
          initWithTitle:l10n_util::GetNSString(IDS_IOS_SUGGESTIONS_DONE)
                  style:UIBarButtonItemStylePlain
                 target:self
                 action:@selector(stop)];

  [self.baseViewController presentViewController:_navigationController
                                        animated:YES
                                      completion:nil];
}

- (void)stop {
  [[_navigationController presentingViewController]
      dismissViewControllerAnimated:YES
                         completion:nil];
  _navigationController = nil;
  _visible = NO;
}

#pragma mark - ContentSuggestionsCommands

- (void)openReadingList {
}

- (void)openFirstPageOfReadingList {
}

- (void)addEmptyItem {
}

- (void)openFaviconAtIndex:(NSInteger)index {
}

@end
