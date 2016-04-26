# Copyright 2016 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'chromium_code': 1,
  },
  'targets': [
    {
      'target_name': 'ios_web_shell_test',
      'type': 'loadable_module',
      'mac_xctest_bundle': 1,
      'variables': {
        'test_host': '<(_target_name)_host',
      },
      'dependencies': [
        '<(test_host)',
      ],
      'xcode_settings': {
        'WRAPPER_EXTENSION': 'xctest',
        'TEST_HOST': '${BUILT_PRODUCTS_DIR}/<(test_host).app/<(test_host)',
        'BUNDLE_LOADER': '$(TEST_HOST)',
        'CODE_SIGN_IDENTITY[sdk=iphoneos*]': 'iPhone Developer',
        'INFOPLIST_FILE': 'shell/test/Module-Info.plist',
        'OTHER_LDFLAGS': [
          '-bundle_loader <(test_host).app/<(test_host)',
        ],
      },
      'sources': [
        'shell/test/shell_test.mm',
      ],
      'link_settings': {
        'libraries': [
          'Foundation.framework',
          'XCTest.framework',
        ],
      },
    },
    {
      # Create a test host for earl grey tests, so Xcode 7.3 and above
      # doesn't contaminate the app structure.
      'target_name': 'ios_web_shell_test_host',
      'includes': [
        'ios_web_shell_exe.gypi',
      ],
      'link_settings': {
        'libraries': [
          'XCTest.framework',
        ],
      },
      'xcode_settings': {
        'INFOPLIST_FILE': 'shell/test/Host-Info.plist',
      },
      'dependencies': [
        '<(DEPTH)/ios/third_party/earl_grey/earl_grey.gyp:EarlGrey',
        'ios_web.gyp:ios_web_test_support',
      ],
      'sources': [
        'public/test/http_server_util.h',
        'public/test/http_server_util.mm',
        'shell/test/navigation_test_util.h',
        'shell/test/navigation_test_util.mm',
        'shell/test/shell_matchers.h',
        'shell/test/shell_matchers.mm',
        'shell/test/web_shell_navigation_egtest.mm',
        'shell/test/web_shell_test_util.h',
        'shell/test/web_shell_test_util.mm',
        'shell/test/web_view_matchers.h',
        'shell/test/web_view_matchers.mm',
      ],
      'postbuilds': [
        {
          'postbuild_name': 'Copy OCHamcrest to TEST_HOST',
          'action': [
            'ditto',
            '${BUILT_PRODUCTS_DIR}/OCHamcrest.framework',
            '${BUILT_PRODUCTS_DIR}/<(_target_name).app/Frameworks/OCHamcrest.framework',
          ],
        },
        {
          'postbuild_name': 'Copy EarlGrey to TEST_HOST',
          'action': [
            'ditto',
            '${BUILT_PRODUCTS_DIR}/EarlGrey.framework',
            '${BUILT_PRODUCTS_DIR}/<(_target_name).app/Frameworks/EarlGrey.framework',
          ],
        },
      ],
    },
  ],
}
