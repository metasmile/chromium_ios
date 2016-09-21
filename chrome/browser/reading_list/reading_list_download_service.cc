// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ios/chrome/browser/reading_list/reading_list_download_service.h"

#include <utility>

#include "base/bind.h"
#include "base/files/file_path.h"
#include "ios/chrome/browser/reading_list/reading_list_entry.h"
#include "ios/chrome/browser/reading_list/reading_list_model.h"

ReadingListDownloadService::ReadingListDownloadService(
    ReadingListModel* reading_list_model,
    dom_distiller::DomDistillerService* distiller_service,
    PrefService* prefs,
    base::FilePath chrome_profile_path)
    : reading_list_model_(reading_list_model) {
  DCHECK(reading_list_model);
  url_downloader_ = std::unique_ptr<URLDownloader>(
      new URLDownloader(distiller_service, prefs, chrome_profile_path,
                        base::Bind(&ReadingListDownloadService::OnDownloadEnd,
                                   base::Unretained(this)),
                        base::Bind(&ReadingListDownloadService::OnDeleteEnd,
                                   base::Unretained(this))));
}

ReadingListDownloadService::~ReadingListDownloadService() {}

void ReadingListDownloadService::Initialize() {
  reading_list_model_->AddObserver(this);
}

void ReadingListDownloadService::Shutdown() {
  reading_list_model_->RemoveObserver(this);
}

void ReadingListDownloadService::ReadingListModelLoaded(
    const ReadingListModel* model) {
  DCHECK_EQ(reading_list_model_, model);
  DownloadAllEntries();
}

void ReadingListDownloadService::ReadingListWillRemoveReadEntry(
    const ReadingListModel* model,
    size_t index) {
  DCHECK_EQ(reading_list_model_, model);
  RemoveDownloadedEntry(model->GetReadEntryAtIndex(index));
}

void ReadingListDownloadService::ReadingListWillRemoveUnreadEntry(
    const ReadingListModel* model,
    size_t index) {
  DCHECK_EQ(reading_list_model_, model);
  RemoveDownloadedEntry(model->GetUnreadEntryAtIndex(index));
}

void ReadingListDownloadService::ReadingListWillAddUnreadEntry(
    const ReadingListModel* model,
    const ReadingListEntry& entry) {
  DCHECK_EQ(reading_list_model_, model);
  DownloadEntry(entry);
}

void ReadingListDownloadService::ReadingListWillAddReadEntry(
    const ReadingListModel* model,
    const ReadingListEntry& entry) {
  DCHECK_EQ(reading_list_model_, model);
  DownloadEntry(entry);
}

void ReadingListDownloadService::DownloadAllEntries() {
  DCHECK(reading_list_model_->loaded());
  size_t size = reading_list_model_->unread_size();
  for (size_t i = 0; i < size; i++) {
    ReadingListEntry entry = reading_list_model_->GetUnreadEntryAtIndex(i);
    this->DownloadEntry(entry);
  }
  size = reading_list_model_->read_size();
  for (size_t i = 0; i < size; i++) {
    ReadingListEntry entry = reading_list_model_->GetReadEntryAtIndex(i);
    this->DownloadEntry(entry);
  }
}

void ReadingListDownloadService::DownloadEntry(const ReadingListEntry& entry) {
  DCHECK(reading_list_model_->loaded());
  if (entry.DistilledState() != ReadingListEntry::ERROR) {
    reading_list_model_->SetEntryDistilledState(entry.URL(),
                                                ReadingListEntry::PROCESSING);
    url_downloader_->DownloadOfflineURL(entry.URL());
  }
}

void ReadingListDownloadService::RemoveDownloadedEntry(
    const ReadingListEntry& entry) {
  DCHECK(reading_list_model_->loaded());
  url_downloader_->RemoveOfflineURL(entry.URL());
}

void ReadingListDownloadService::OnDownloadEnd(
    const GURL& url,
    URLDownloader::SuccessState success,
    const GURL& distilled_url,
    const std::string& title) {
  DCHECK(reading_list_model_->loaded());
  if ((success == URLDownloader::DOWNLOAD_SUCCESS ||
       success == URLDownloader::DOWNLOAD_EXISTS) &&
      distilled_url.is_valid()) {
    reading_list_model_->SetEntryDistilledURL(url, distilled_url);
  } else if (success == URLDownloader::ERROR_RETRY) {
    reading_list_model_->SetEntryDistilledState(url,
                                                ReadingListEntry::WILL_RETRY);
  } else if (success == URLDownloader::ERROR_PERMANENT) {
    reading_list_model_->SetEntryDistilledState(url, ReadingListEntry::ERROR);
  }
}

void ReadingListDownloadService::OnDeleteEnd(const GURL& url, bool success) {
  // Nothing to update as this is only called when deleting reading list entries
}
