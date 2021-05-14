struct Button_State
{
    bool is_down;
    bool changed;
};

enum
{
    BUTTON_A,
    BUTTON_D,
    BUTTON_W,
    BUTTON_ENTER,
    BUTTON_ESCAPE,
    BUTTON_SPACE,
    BUTTON_SHIFT,
    BUTTON_COUNT,
};

struct Input
{
    Button_State buttons[BUTTON_COUNT];
};