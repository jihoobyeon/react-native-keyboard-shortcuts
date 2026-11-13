#pragma once

#include "pch.h"
#include "resource.h"

#if __has_include("codegen/NativeKeyboardShortcutsDataTypes.g.h")
  #include "codegen/NativeKeyboardShortcutsDataTypes.g.h"
#endif
// Note: The following lines use Mustache template syntax which will be processed during
// project generation to produce standard C++ code. If existing codegen spec files are found,
// use the actual filename; otherwise use conditional includes.
#if __has_include("codegen/NativeKeyboardShortcutsSpec.g.h")
  #include "codegen/NativeKeyboardShortcutsSpec.g.h"
#endif

#include "NativeModules.h"

namespace winrt::KeyboardShortcuts
{
  REACT_TURBO_MODULE(KeyboardShortcuts)
  struct KeyboardShortcuts
  {
    using ModuleSpec = KeyboardShortcutsCodegen::KeyboardShortcutsSpec;

    REACT_INIT(Initialize)
    void Initialize(React::ReactContext const& reactContext) noexcept;

    REACT_SYNC_METHOD(addShortcut)
    double addShortcut(std::vector<std::string> keys) noexcept;

    REACT_METHOD(removeShortcut)
    void removeShortcut(double id) noexcept;

    REACT_CONSTANT_PROVIDER(Constants)
    void Constants(React::ReactConstantProvider& constants) noexcept;

    REACT_EVENT(onShortcut)
    std::function<void(KeyboardShortcutsCodegen::KeyboardShortcutsSpec_ShortcutEvent)> onShortcut;

  private:

    struct Shortcut
    {
      UINT key = 0;
      UINT modifiers = 0;
    };

    React::ReactContext m_context;

    // 최상위 HWND
    HWND m_hwnd = nullptr;

    int m_nextShortcutId = 1;

    // JS 스레드와 UI 스레드 사이의 동시 접근 보호
    std::mutex m_shortcutsMutex;

    std::map<int, Shortcut> m_shortcuts;

    // UI 스레드에서 관리하는 Subclass 등록 HWND 목록
    std::unordered_set<HWND> m_subclassedHwnds;

    static LRESULT CALLBACK SubclassProc(
      HWND hwnd,
      UINT msg,
      WPARAM wParam,
      LPARAM lParam,
      UINT_PTR subclassId,
      DWORD_PTR refData
    );

    void install() noexcept;

    bool installSubclass(HWND hwnd) noexcept;

    bool isShortcutKey(
      UINT key,
      UINT modifiers,
      int& matchedId
    ) noexcept;

  };

} // namespace winrt::KeyboardShortcuts