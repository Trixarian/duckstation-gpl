// SPDX-FileCopyrightText: 2019-2023 Connor McLaughlin <stenzek@gmail.com>
// SPDX-License-Identifier: (GPL-3.0 OR CC-BY-NC-ND-4.0)

#include <string_view>

#include <Cocoa/Cocoa.h>

namespace CocoaTools {
  NSString* StringViewToNSString(const std::string_view& str);
}

