// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IOS_CHROME_BROWSER_UI_READING_LIST_READING_LIST_VIEW_CONTROLLER_H_
#define IOS_CHROME_BROWSER_UI_READING_LIST_READING_LIST_VIEW_CONTROLLER_H_

#import "ios/chrome/browser/ui/collection_view/collection_view_controller.h"

#include <string>

#import "ios/chrome/browser/ui/reading_list/reading_list_toolbar.h"

namespace favicon {
class LargeIconService;
}

class ReadingListDownloadService;
class ReadingListModel;
@class TabModel;

@interface ReadingListViewController
    : CollectionViewController<ReadingListToolbarDelegate>

@property(nonatomic, readonly) ReadingListModel* readingListModel;
@property(weak, nonatomic, readonly) TabModel* tabModel;
@property(nonatomic, readonly) favicon::LargeIconService* largeIconService;
@property(nonatomic, readonly)
    ReadingListDownloadService* readingListDownloadService;

- (instancetype)initWithModel:(ReadingListModel*)model
                      tabModel:(TabModel*)tabModel
              largeIconService:(favicon::LargeIconService*)largeIconService
    readingListDownloadService:
        (ReadingListDownloadService*)readingListDownloadService
    NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithStyle:(CollectionViewControllerStyle)style
    NS_UNAVAILABLE;

@end

@interface ReadingListViewController (Testing)

// Dismisses this view controller.
- (void)dismiss;

@end

#endif  // IOS_CHROME_BROWSER_UI_READING_LIST_READING_LIST_VIEW_CONTROLLER_H_