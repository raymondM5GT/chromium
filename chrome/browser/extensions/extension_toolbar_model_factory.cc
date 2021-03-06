// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/extensions/extension_toolbar_model_factory.h"

#include "chrome/browser/extensions/extension_toolbar_model.h"
#include "chrome/browser/profiles/profile.h"
#include "components/browser_context_keyed_service/browser_context_dependency_manager.h"
#include "extensions/browser/extension_prefs.h"
#include "extensions/browser/extension_prefs_factory.h"
#include "extensions/browser/extensions_browser_client.h"

// static
ExtensionToolbarModel* ExtensionToolbarModelFactory::GetForProfile(
    Profile* profile) {
  return static_cast<ExtensionToolbarModel*>(
      GetInstance()->GetServiceForBrowserContext(profile, true));
}

// static
ExtensionToolbarModelFactory* ExtensionToolbarModelFactory::GetInstance() {
  return Singleton<ExtensionToolbarModelFactory>::get();
}

ExtensionToolbarModelFactory::ExtensionToolbarModelFactory()
    : BrowserContextKeyedServiceFactory(
        "ExtensionToolbarModel",
        BrowserContextDependencyManager::GetInstance()) {
  DependsOn(extensions::ExtensionPrefsFactory::GetInstance());
}

ExtensionToolbarModelFactory::~ExtensionToolbarModelFactory() {}

BrowserContextKeyedService*
    ExtensionToolbarModelFactory::BuildServiceInstanceFor(
        content::BrowserContext* context) const {
  return new ExtensionToolbarModel(
      Profile::FromBrowserContext(context),
      extensions::ExtensionPrefsFactory::GetForBrowserContext(context));
}

content::BrowserContext* ExtensionToolbarModelFactory::GetBrowserContextToUse(
    content::BrowserContext* context) const {
  return extensions::ExtensionsBrowserClient::Get()->
      GetOriginalContext(context);
}

bool ExtensionToolbarModelFactory::ServiceIsCreatedWithBrowserContext() const {
  return true;
}

bool ExtensionToolbarModelFactory::ServiceIsNULLWhileTesting() const {
  return true;
}
