
/*
 * This file is auto-generated from a NativeModule spec file in js.
 *
 * This is a C++ Spec class that should be used with MakeTurboModuleProvider to register native modules
 * in a way that also verifies at compile time that the native module matches the interface required
 * by the TurboModule JS spec.
 */
#pragma once
// clang-format off

// #include "NativeKeyboardShortcutsDataTypes.g.h" before this file to use the generated type definition
#include <NativeModules.h>
#include <tuple>

namespace KeyboardShortcutsCodegen {

inline winrt::Microsoft::ReactNative::FieldMap GetStructInfo(KeyboardShortcutsSpec_Constants*) noexcept {
    winrt::Microsoft::ReactNative::FieldMap fieldMap {
        {L"Shift", &KeyboardShortcutsSpec_Constants::Shift},
        {L"Control", &KeyboardShortcutsSpec_Constants::Control},
        {L"Alt", &KeyboardShortcutsSpec_Constants::Alt},
        {L"Command", &KeyboardShortcutsSpec_Constants::Command},
        {L"Tab", &KeyboardShortcutsSpec_Constants::Tab},
    };
    return fieldMap;
}

inline winrt::Microsoft::ReactNative::FieldMap GetStructInfo(KeyboardShortcutsSpec_ShortcutEvent*) noexcept {
    winrt::Microsoft::ReactNative::FieldMap fieldMap {
        {L"id", &KeyboardShortcutsSpec_ShortcutEvent::id},
    };
    return fieldMap;
}

struct KeyboardShortcutsSpec : winrt::Microsoft::ReactNative::TurboModuleSpec {
  static constexpr auto methods = std::tuple{
      SyncMethod<double(std::vector<std::string>) noexcept>{0, L"addShortcut"},
      Method<void(double) noexcept>{1, L"removeShortcut"},
      EventEmitter<void(KeyboardShortcutsSpec_ShortcutEvent)>{2, L"onShortcut"},
  };

  template <class TModule>
  static constexpr void ValidateModule() noexcept {
    constexpr auto methodCheckResults = CheckMethods<TModule, KeyboardShortcutsSpec>();

    REACT_SHOW_METHOD_SPEC_ERRORS(
          0,
          "addShortcut",
          "    REACT_SYNC_METHOD(addShortcut) double addShortcut(std::vector<std::string> const & keys) noexcept { /* implementation */ }\n"
          "    REACT_SYNC_METHOD(addShortcut) static double addShortcut(std::vector<std::string> const & keys) noexcept { /* implementation */ }\n");
    REACT_SHOW_METHOD_SPEC_ERRORS(
          1,
          "removeShortcut",
          "    REACT_METHOD(removeShortcut) void removeShortcut(double id) noexcept { /* implementation */ }\n"
          "    REACT_METHOD(removeShortcut) static void removeShortcut(double id) noexcept { /* implementation */ }\n");
    REACT_SHOW_EVENTEMITTER_SPEC_ERRORS(
          2,
          "onShortcut",
          "    REACT_EVENT(onShortcut) std::function<void(KeyboardShortcutsSpec_ShortcutEvent)> onShortcut;\n");
  }
};

} // namespace KeyboardShortcutsCodegen
