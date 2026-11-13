# react-native-keyboard-shortcuts

custom keyboard shortcut module for React Native(Desktop)Keyboard

## Installation


```sh
yarn add react-native-keyboard-shortcuts

// Windows
npx react-native autolink-windows
// macOS
npx pod-install macos
```


## Usage


```js
import { Control, useShortcut } from 'react-native-keyboard-shortcuts';

export default function App() {
	useShortcut([Control, 'a'], Alert.alert('TEST', 'This is a test.'));
	return <View />;
};
```
We have 5 special keys: Shift, Control, Alt, Tab, and Command(which is 'Win' key in Windows)

## Contributing

- [Development workflow](CONTRIBUTING.md#development-workflow)
- [Sending a pull request](CONTRIBUTING.md#sending-a-pull-request)
- [Code of conduct](CODE_OF_CONDUCT.md)

## License

MIT

---

Made with [create-react-native-library](https://github.com/callstack/react-native-builder-bob)
