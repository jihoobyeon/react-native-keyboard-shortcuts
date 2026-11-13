
#include "pch.h"
#include "KeyboardShortcuts.h"

namespace winrt::KeyboardShortcuts
{
  namespace
  {
    UINT getModifiers()
    {
      UINT modifiers = 0;

      if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
        modifiers |= MOD_SHIFT;

      if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
        modifiers |= MOD_CONTROL;

      if (GetAsyncKeyState(VK_MENU) & 0x8000)
        modifiers |= MOD_ALT;

      if (
        (GetAsyncKeyState(VK_LWIN) & 0x8000) ||
        (GetAsyncKeyState(VK_RWIN) & 0x8000)
        )
      {
        modifiers |= MOD_WIN;
      }

      return modifiers;
    }

    UINT getVirtualKey(const std::string& key)
    {
      if (key == "tab")
        return VK_TAB;

      if (key.size() == 1)
      {
        SHORT vk = VkKeyScanA(key[0]);

        if (vk != -1)
          return LOBYTE(vk);
      }

      if (
        key.size() >= 2 &&
        key[0] == 'f'
        )
      {
        try
        {
          int number = std::stoi(key.substr(1));

          if (number >= 1 && number <= 24)
            return VK_F1 + number - 1;
        }
        catch (...)
        {
        }
      }

      return 0;
    }

    HWND getHwnd()
    {
      struct ProcessWindow
      {
        DWORD pid;
        HWND hwnd;
        HWND fallback;
      };

      ProcessWindow pw{};
      pw.pid = GetCurrentProcessId();

      EnumWindows(
        [](HWND hwnd, LPARAM lp) -> BOOL
        {
          auto* pw = reinterpret_cast<ProcessWindow*>(lp);

          DWORD pid = 0;
          GetWindowThreadProcessId(hwnd, &pid);

          if (pid != pw->pid)
            return TRUE;

          if (GetWindow(hwnd, GW_OWNER) == nullptr)
          {
            if (!pw->fallback)
              pw->fallback = hwnd;

            if (IsWindowVisible(hwnd))
            {
              pw->hwnd = hwnd;
              return FALSE;
            }
          }

          return TRUE;
        },
        reinterpret_cast<LPARAM>(&pw)
      );

      return pw.hwnd ? pw.hwnd : pw.fallback;
    }
  }

  void KeyboardShortcuts::Initialize(
    React::ReactContext const& reactContext
  ) noexcept
  {
    m_context = reactContext;
  }

  // HWND 하나에 Subclass 설치
  bool KeyboardShortcuts::installSubclass(
    HWND hwnd
  ) noexcept
  {
    if (!hwnd || !IsWindow(hwnd))
      return false;

    if (m_subclassedHwnds.find(hwnd) != m_subclassedHwnds.end())
      return true;

    UINT_PTR subclassId =
      reinterpret_cast<UINT_PTR>(this);

    BOOL result = SetWindowSubclass(
      hwnd,
      SubclassProc,
      subclassId,
      reinterpret_cast<DWORD_PTR>(this)
    );

    if (result)
    {
      m_subclassedHwnds.insert(hwnd);
    }

    return result != FALSE;
  }

  void KeyboardShortcuts::install() noexcept
  {
    m_context.UIDispatcher().Post([this]()
      {
        if (m_hwnd && IsWindow(m_hwnd))
          return;

        HWND hwnd = getHwnd();

        if (!hwnd)
          return;

        if (!installSubclass(hwnd))
          return;

        m_hwnd = hwnd;

        // 설치 시점에 이미 자식 HWND에 포커스가 있다면
        // 해당 HWND에도 Subclass를 설치한다.
        HWND focus = GetFocus();

        if (
          focus &&
          IsWindow(focus) &&
          GetAncestor(focus, GA_ROOT) == m_hwnd
          )
        {
          installSubclass(focus);
        }
      });
  }

  double KeyboardShortcuts::addShortcut(
    std::vector<std::string> keys
  ) noexcept
  {
    Shortcut shortcut{};
    bool hasKey = false;

    for (const auto& key : keys)
    {
      if (key == "shift")
      {
        shortcut.modifiers |= MOD_SHIFT;
      }
      else if (key == "control")
      {
        shortcut.modifiers |= MOD_CONTROL;
      }
      else if (key == "option" || key == "alt")
      {
        shortcut.modifiers |= MOD_ALT;
      }
      else if (key == "win")
      {
        shortcut.modifiers |= MOD_WIN;
      }
      else
      {
        shortcut.key = getVirtualKey(key);
        hasKey = shortcut.key != 0;
      }
    }

    if (!hasKey)
      return -1;

    int id;

    {
      std::lock_guard<std::mutex> lock(m_shortcutsMutex);

      id = m_nextShortcutId++;

      m_shortcuts[id] = shortcut;
    }

    install();

    return static_cast<double>(id);
  }

  void KeyboardShortcuts::removeShortcut(
    double id
  ) noexcept
  {
    std::lock_guard<std::mutex> lock(m_shortcutsMutex);

    m_shortcuts.erase(static_cast<int>(id));
  }

  bool KeyboardShortcuts::isShortcutKey(
    UINT key,
    UINT modifiers,
    int& matchedId
  ) noexcept
  {
    std::lock_guard<std::mutex> lock(m_shortcutsMutex);

    for (const auto& [id, shortcut] : m_shortcuts)
    {
      if (
        shortcut.key == key &&
        shortcut.modifiers == modifiers
        )
      {
        matchedId = id;
        return true;
      }
    }

    return false;
  }

  LRESULT CALLBACK KeyboardShortcuts::SubclassProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam,
    UINT_PTR subclassId,
    DWORD_PTR refData
  )
  {
    auto self =
      reinterpret_cast<KeyboardShortcuts*>(refData);

    if (!self)
    {
      return DefSubclassProc(
        hwnd,
        msg,
        wParam,
        lParam
      );
    }

    // 포커스가 다른 자식 HWND로 이동하면
    // 해당 HWND에도 Subclass를 설치한다.
    if (msg == WM_KILLFOCUS)
    {
      HWND nextHwnd =
        reinterpret_cast<HWND>(wParam);

      if (
        nextHwnd &&
        IsWindow(nextHwnd) &&
        self->m_hwnd &&
        GetAncestor(nextHwnd, GA_ROOT) == self->m_hwnd
        )
      {
        self->installSubclass(nextHwnd);
      }
    }

    // 단축키 처리
    if (
      msg == WM_KEYDOWN ||
      msg == WM_SYSKEYDOWN
      )
    {
      UINT key = static_cast<UINT>(wParam);

      // 이미 눌린 키의 자동 반복 메시지는 무시
      // lParam bit 30: 이전 키 상태
      bool isKeyRepeat =
        (static_cast<ULONG_PTR>(lParam) & (1ULL << 30)) != 0;

      if (!isKeyRepeat)
      {
        UINT modifiers = getModifiers();

        int matchedId = -1;

        if (
          self->isShortcutKey(
            key,
            modifiers,
            matchedId
          )
          )
        {
          // 현재 SubclassProc 실행이 끝난 뒤 이벤트 전달
          self->m_context.UIDispatcher().Post(
            [self, matchedId]()
            {
              self->onShortcut({
                static_cast<double>(matchedId)
                });
            }
          );

          // 매칭된 단축키만 소비
          return 0;
        }
      }
    }

    // HWND 파괴 시 해당 Subclass만 정리
    if (msg == WM_NCDESTROY)
    {
      RemoveWindowSubclass(
        hwnd,
        SubclassProc,
        subclassId
      );

      self->m_subclassedHwnds.erase(hwnd);

      if (hwnd == self->m_hwnd)
      {
        self->m_hwnd = nullptr;
      }

      return DefSubclassProc(
        hwnd,
        msg,
        wParam,
        lParam
      );
    }

    return DefSubclassProc(
      hwnd,
      msg,
      wParam,
      lParam
    );
  }

  void KeyboardShortcuts::Constants(
    React::ReactConstantProvider& constants
  ) noexcept
  {
    constants.Add(L"Shift", L"shift");
    constants.Add(L"Control", L"control");
    constants.Add(L"Alt", L"option");
    constants.Add(L"Command", L"win");
    constants.Add(L"Tab", L"tab");
  }

} // namespace winrt::KeyboardShortcuts
