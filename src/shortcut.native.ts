import KeyboardShortcuts from './NativeKeyboardShortcuts';

export function shortcut(keys: string[], callback: () => void): void {
	return KeyboardShortcuts.shortcut(keys, callback);
}

export const Shift: string = KeyboardShortcuts.Shift;
export const Control: string = KeyboardShortcuts.Control;
export const Alt: string = KeyboardShortcuts.Alt;
export const Command: string = KeyboardShortcuts.Command;
export const Tab: string = KeyboardShortcuts.Tab;