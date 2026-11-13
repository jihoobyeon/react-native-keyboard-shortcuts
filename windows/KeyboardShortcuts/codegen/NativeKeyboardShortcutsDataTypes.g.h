
/*
 * This file is auto-generated from a NativeModule spec file in js.
 *
 * This is a C++ Spec class that should be used with MakeTurboModuleProvider to register native modules
 * in a way that also verifies at compile time that the native module matches the interface required
 * by the TurboModule JS spec.
 */
#pragma once
// clang-format off

#include <string>
#include <optional>
#include <functional>
#include <vector>

namespace KeyboardShortcutsCodegen {

struct KeyboardShortcutsSpec_Constants {
    std::string Shift;
    std::string Control;
    std::string Alt;
    std::string Command;
    std::string Tab;
};

struct KeyboardShortcutsSpec_ShortcutEvent {
    double id;
};

} // namespace KeyboardShortcutsCodegen
