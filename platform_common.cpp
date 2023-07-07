struct ButtonState {
	bool isDown;
	bool changed;
};

enum {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,

	BUTTON_COUNT // Should be last in enum
};

struct Input {
	ButtonState buttons[BUTTON_COUNT];
};