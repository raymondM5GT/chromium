// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/ime/input_method_initializer.h"

#if defined(TOOLKIT_VIEWS) || defined(USE_AURA)
#include "ui/base/ime/input_method_factory.h"
#endif

#if defined(OS_CHROMEOS)
#include "base/logging.h"
#include "ui/base/ime/chromeos/ibus_bridge.h"
#elif defined(USE_AURA) && defined(OS_LINUX)
#include "ui/base/ime/input_method_auralinux.h"
#include "ui/base/ime/linux/fake_input_method_context_factory.h"
#elif defined(OS_WIN)
#include "base/win/metro.h"
#include "ui/base/ime/win/tsf_bridge.h"
#endif

namespace {

#if !defined(OS_CHROMEOS) && defined(USE_AURA) && defined(OS_LINUX)
const ui::LinuxInputMethodContextFactory* g_linux_input_method_context_factory;
#endif

}  // namespace

namespace ui {

void InitializeInputMethod() {
#if defined(OS_CHROMEOS)
  chromeos::IBusBridge::Initialize();
#elif defined(USE_AURA) && defined(OS_LINUX)
  InputMethodAuraLinux::Initialize();
#elif defined(OS_WIN)
  if (base::win::IsTSFAwareRequired())
    TSFBridge::Initialize();
#endif
}

void ShutdownInputMethod() {
#if defined(TOOLKIT_VIEWS) || defined(USE_AURA)
  InputMethodFactory::ClearInstance();
#endif
#if defined(OS_CHROMEOS)
  chromeos::IBusBridge::Shutdown();
#elif defined(OS_WIN)
  internal::DestroySharedInputMethod();
  if (base::win::IsTSFAwareRequired())
    TSFBridge::Shutdown();
#endif
}

void InitializeInputMethodForTesting() {
#if defined(OS_CHROMEOS)
  chromeos::IBusBridge::Initialize();
#elif defined(USE_AURA) && defined(OS_LINUX)
  if (!g_linux_input_method_context_factory)
    g_linux_input_method_context_factory = new FakeInputMethodContextFactory();
  const LinuxInputMethodContextFactory* factory =
      LinuxInputMethodContextFactory::instance();
  CHECK(!factory || factory == g_linux_input_method_context_factory)
      << "LinuxInputMethodContextFactory was already initialized somewhere "
      << "else.";
  LinuxInputMethodContextFactory::SetInstance(
      g_linux_input_method_context_factory);
#elif defined(OS_WIN)
  if (base::win::IsTSFAwareRequired()) {
    // Make sure COM is initialized because TSF depends on COM.
    CoInitialize(NULL);
    TSFBridge::Initialize();
  }
#endif
}

void ShutdownInputMethodForTesting() {
#if defined(TOOLKIT_VIEWS) || defined(USE_AURA)
  InputMethodFactory::ClearInstance();
#endif
#if defined(OS_CHROMEOS)
  chromeos::IBusBridge::Shutdown();
#elif defined(USE_AURA) && defined(OS_LINUX)
  const LinuxInputMethodContextFactory* factory =
      LinuxInputMethodContextFactory::instance();
  CHECK(!factory || factory == g_linux_input_method_context_factory)
      << "An unknown LinuxInputMethodContextFactory was set.";
  LinuxInputMethodContextFactory::SetInstance(NULL);
  delete g_linux_input_method_context_factory;
  g_linux_input_method_context_factory = NULL;
#elif defined(OS_WIN)
  internal::DestroySharedInputMethod();
  if (base::win::IsTSFAwareRequired()) {
    TSFBridge::Shutdown();
    CoUninitialize();
  }
#endif
}

}  // namespace ui
