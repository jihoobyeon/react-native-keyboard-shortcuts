import { TurboModuleRegistry, type TurboModule } from 'react-native';

export interface Spec extends TurboModule {
	shortcut(keys: string[], callback: () => void): void;
	Shift: string;
	Control: string;
	Alt: string;
	Command: string;
	Tab: string;
}

export default TurboModuleRegistry.getEnforcing<Spec>('KeyboardShortcuts');
