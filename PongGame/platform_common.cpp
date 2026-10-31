struct Button_State {
	bool is_Down;
	bool changed;
};

enum 
{
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,

	BUTTON_COUNT, //Should always be last
};

struct Input
{
	Button_State buttons[BUTTON_COUNT];
};