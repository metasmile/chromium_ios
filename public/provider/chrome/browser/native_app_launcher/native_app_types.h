// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IOS_PUBLIC_PROVIDER_CHROME_BROWSER_NATIVE_APP_LAUNCHER_NATIVE_APP_TYPES_H_
#define IOS_PUBLIC_PROVIDER_CHROME_BROWSER_NATIVE_APP_LAUNCHER_NATIVE_APP_TYPES_H_

// Types of user interactions with Native App Launcher infobars.
enum NativeAppActionType {
  NATIVE_APP_ACTION_IGNORE,
  NATIVE_APP_ACTION_DISMISS,
  NATIVE_APP_ACTION_CLICK_INSTALL,
  NATIVE_APP_ACTION_CLICK_LAUNCH,
  NATIVE_APP_ACTION_CLICK_ONCE,
  NATIVE_APP_ACTION_CLICK_ALWAYS,
  NATIVE_APP_ACTION_COUNT,
};

// Types of NativeAppInfoBarDelegate controllers.
enum NativeAppControllerType {
  NATIVE_APP_INSTALLER_CONTROLLER,
  NATIVE_APP_LAUNCHER_CONTROLLER,
  NATIVE_APP_OPEN_POLICY_CONTROLLER
};

#endif  // IOS_PUBLIC_PROVIDER_CHROME_BROWSER_NATIVE_APP_LAUNCHER_NATIVE_APP_TYPES_H_
