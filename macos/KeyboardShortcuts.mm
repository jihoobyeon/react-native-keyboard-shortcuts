#import "KeyboardShortcuts.h"
#import <AppKit/AppKit.h>
#import <React/RCTLog.h>

@implementation KeyboardShortcuts {
	NSMutableDictionary<NSNumber *, id> *_monitors;
	NSInteger _nextShortcutId;
}

- (instancetype)init
{
	self = [super init];

	if (self) {
		_monitors = [NSMutableDictionary dictionary];
		_nextShortcutId = 1;
	}

	return self;
}

- (NSNumber *)addShortcut:(NSArray *)keys
{
	__block NSEventModifierFlags requiredModifiers = 0;
	__block NSString *requiredKey = nil;

	for (id key in keys) {
		if (![key isKindOfClass:[NSString class]])
			continue;

		NSString *value = (NSString *)key;

		if ([value isEqualToString:@"command"]) {
			requiredModifiers |= NSEventModifierFlagCommand;
		}
		else if ([value isEqualToString:@"control"]) {
			requiredModifiers |= NSEventModifierFlagControl;
		}
		else if ([value isEqualToString:@"option"]) {
			requiredModifiers |= NSEventModifierFlagOption;
		}
		else if ([value isEqualToString:@"shift"]) {
			requiredModifiers |= NSEventModifierFlagShift;
		}
		else if ([value isEqualToString:@"tab"]) {
			requiredKey = @"tab";
		}
		else { // real keys
			requiredKey = value.lowercaseString;
		}
	}

	if (!requiredKey) {
		RCTLogError(@"Keyboard shortcut must contain a non-modifier key.");
		return @(-1);
	}

	NSNumber *shortcutId = @(_nextShortcutId++);

	// compare with Command / Control / Option / Shift
	NSEventModifierFlags modifierMask =
		NSEventModifierFlagCommand |
		NSEventModifierFlagControl |
		NSEventModifierFlagOption |
		NSEventModifierFlagShift;

	__weak KeyboardShortcuts *weakSelf = self;

	id monitor =
		[NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskKeyDown
		                                     handler:^NSEvent *(NSEvent *event) {

		KeyboardShortcuts *strongSelf = weakSelf;

		if (!strongSelf)
			return event;

		if (event.isARepeat)
			return event;

		NSEventModifierFlags modifiers =
			event.modifierFlags & modifierMask;

		if (modifiers != requiredModifiers)
			return event;

		if ([requiredKey isEqualToString:@"tab"] && event.keyCode != 48)
			return event;

		[strongSelf emitOnShortcut:@{
			@"id": shortcutId
		}];

		// consume original key event.
		return nil;
	}];

	if (monitor) {
		_monitors[shortcutId] = monitor;
	}
	else {
		return @(-1);
	}

	return shortcutId;
}

- (void)removeShortcut:(double)idx
{
	NSNumber *shortcutId = @(idx);
	id monitor = _monitors[shortcutId];

	if (!monitor) return;

	[NSEvent removeMonitor:monitor];
	[_monitors removeObjectForKey:shortcutId];
}

- (void)dealloc
{
	for (id monitor in _monitors) {
		[NSEvent removeMonitor:monitor];
	}
}

- (facebook::react::ModuleConstants<JS::NativeKeyboardShortcuts::Constants::Builder>)constantsToExport
{
	return facebook::react::typedConstants<JS::NativeKeyboardShortcuts::Constants::Builder>({
		.Shift = @"shift",
		.Control = @"control",
		.Alt = @"option",
		.Command = @"command",
		.Tab = @"tab"
	});
}

- (facebook::react::ModuleConstants<JS::NativeKeyboardShortcuts::Constants::Builder>)getConstants
{
	return [self constantsToExport];
}

- (std::shared_ptr<facebook::react::TurboModule>)getTurboModule:
    (const facebook::react::ObjCTurboModule::InitParams &)params
{
    return std::make_shared<facebook::react::NativeKeyboardShortcutsSpecJSI>(params);
}

+ (NSString *)moduleName
{
  return @"KeyboardShortcuts";
}

@end

