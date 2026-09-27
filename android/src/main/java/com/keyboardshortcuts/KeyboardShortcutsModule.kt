package com.keyboardshortcuts

import com.facebook.react.bridge.ReactApplicationContext

class KeyboardShortcutsModule(reactContext: ReactApplicationContext) :
  NativeKeyboardShortcutsSpec(reactContext) {

  override fun multiply(a: Double, b: Double): Double {
    return a * b
  }

  companion object {
    const val NAME = NativeKeyboardShortcutsSpec.NAME
  }
}
