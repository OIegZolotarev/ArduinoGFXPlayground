#define TFT_W 480
#define TFT_H 272


typedef struct
{
    int x,y;
} vec2i;

typedef struct functionalButton_s
{
    const char* description;
    void (*handler)();

    float timer;

    uint16_t text_color;
    uint16_t outline_color;

    uint8_t flags = 0;

}functionalButton_t;

#define FB_HIGHLIGHT (1<<0)

enum class UIFunctionsState
{
    Gauges,
    Tune,
    Media,
    Settings
};

enum class PhysicalButtons
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    FUNC1,
    FUNC2,
    FUNC3,
    FUNC4
};

class UIState
{
    UIFunctionsState state = UIFunctionsState::Gauges;
    functionalButton_t buttons[4];

    void selectFunction(int funcId);

    void stateMedia();
    void stateGauges();

    // Gauges
    struct 
    {
        uint8_t brakeLevel = 0;
        uint16_t rpm = 0;
    }gaugesState;

    // Media:
    struct 
    {
        char trackName[64];
        int trackLength;
        int trackPosition;
    }mediaState;
    

public:

    UIState();

    functionalButton_t* getFunctionalButtons() { return buttons; }
    UIFunctionsState currentState() { return state; }

    void handlePhysicalButton(PhysicalButtons btnId);
    
    void render();

    void updateTrackState(const char* trackName, int trackPos, int trackLength);

    void setBrakeLevel(int brakeLevel);
    void setRPM(int rpm);

};

extern UIState* g_uiState;