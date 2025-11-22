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

enum class TrackLabelStates
{
    StayingAtStart = 0,
    GoingRight,
    StayingAtEnd,
    GoingLeft
};

class UIController
{
    UIFunctionsState state = UIFunctionsState::Gauges;
    functionalButton_t buttons[4];
    
    int textSize = 0;

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
        unsigned long update_millis;

        int trackNameLength = 0;        

        TrackLabelStates labelState = TrackLabelStates::StayingAtStart;
        float labelScrollPosition = 0;        
        unsigned long labelNextUpdate = 0;


    }mediaState;
    
    void setTextSize(int size);

    int printTimeFormatted(int timeSeconds);

    void drawMusicTrackName(int x,int y);

    TrackLabelStates nextTrackLabelState(TrackLabelStates current);
public:

    UIController();

    functionalButton_t* getFunctionalButtons() { return buttons; }
    UIFunctionsState currentState() { return state; }

    void handlePhysicalButton(PhysicalButtons btnId);
    
    void render();

    void updateTrackState(const char* trackName, int trackPos, int trackLength);

    void setBrakeLevel(int brakeLevel);
    void setRPM(int rpm);

};

extern UIController* g_uiController;

