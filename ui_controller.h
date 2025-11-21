#define TFT_W 480
#define TFT_H 272

void DrawUI();


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

}functionalButton_t;


