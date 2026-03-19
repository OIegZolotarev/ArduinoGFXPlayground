#pragma once

#include <algorithm>
#include "ui_widget.h"

class OnScreenKeyboard: public UIWidget
{
public:
    OnScreenKeyboard(char *buffer, size_t bufLen);

    bool isFinished() const;
    
    bool handlePhysicalButton(PhysicalButtons btn) override;

    // Ввод символа текущей клавиши (вызов из внешней кнопки "OK")
    void pressCurrentKey();

    
    virtual void render(ApplicationPlatform* platformInstance) override;


    void setupTopLevelButtons(struct functionalButton_s* buttons) override;

private:

    Arduino_GFX* gfx;
    
    char *buffer;
    size_t bufLen;

    bool finished = false;

    enum LayoutMode {
        ABC_LOWER,
        ABC_UPPER,
        SYM
    } mode = ABC_LOWER;

    int cursorX = 0;
    int cursorY = 0;

    // ---------- РАСКЛАДКИ ----------
    const char *layoutLower[3] = {
        "qwertyuiop",
        "asdfghjkl",
        "zxcvbnm"
    };

    const char *layoutUpper[3] = {
        "QWERTYUIOP",
        "ASDFGHJKL",
        "ZXCVBNM"
    };

    const char *layoutSym[3] = {
        "1234567890",
        "@#$%&*-+=/",
        ".,!?():;\"'"
    };

    // Ряд специальных клавиш
    // [Shift] [ABC/SYM] [Space] [Backspace] [Enter]
    const char *specialNames[5] = {
        "Shift",
        "Sym",
        "Space",
        "<-",
        "OK"
    };

    // ========== ЛОГИКА ==========

    const char** getCurrentLayout();

    int currentRows() const;      // 3 строки + спец
    int specialRow() const;
    int currentCols() const;

    const char* getCurrentRow() const;

    void toggleShiftOrSym();

    void handleSpecialKey(int x);

    void insertChar(char c);

    void backspace();

    // ========== ОТРИСОВКА ==========

    void drawKeyboard();
    void drawKeyChar(int x, int y, int w, int h, char c, bool active);
    void drawKeyLabel(int x, int y, int w, int h, const char *label, bool active);
};
