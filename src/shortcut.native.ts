import KeyboardShortcuts from './NativeKeyboardShortcuts';
import { useEffect } from 'react';

export function useShortcut(keys: string[], callback: () => void): void {
	const callbacks = new Map<number, () => void>();
	KeyboardShortcuts.onShortcut(({ id }) => { callbacks.get(id)?.(); });

	useEffect(() => {
		const id = KeyboardShortcuts.addShortcut(keys);
		callbacks.set(id, callback);
		return () => KeyboardShortcuts.removeShortcut(id);
	}, [keys, callback]);
}

const { Shift, Control, Alt, Command, Tab } = KeyboardShortcuts.getConstants();
export { Shift, Control, Alt, Command, Tab };