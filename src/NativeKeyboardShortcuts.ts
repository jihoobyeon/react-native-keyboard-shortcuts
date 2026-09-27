import { TurboModuleRegistry, type CodegenTypes, type TurboModule } from 'react-native';

export type Constants = {
	Shift: string;
	Control: string;
	Alt: string;
	Command: string;
	Tab: string;
};

export interface Spec extends TurboModule {
	addShortcut(keys: string[]): number;
	removeShortcut(id: number): void;
	readonly onShortcut: CodegenTypes.EventEmitter<{ id: number }>;
	getConstants(): Constants;
}

export default TurboModuleRegistry.getEnforcing<Spec>('KeyboardShortcuts');
