#pragma once

class OnScreenKeyboard
{
public:
    OnScreenKeyboard(Arduino_GFX *gfx, char *buffer, size_t bufLen)
        : gfx(gfx), buffer(buffer), bufLen(bufLen)
    {
        buffer[0] = '\0';
    }

    bool isFinished() const { return finished; }

    // Обработка кнопок устройства
    void handlePhysicalButton(PhysicalButtons btn)
    {
        if (finished) return;

        switch (btn)
        {
            case PhysicalButtons::LEFT:
                if (cursorX > 0) cursorX--;
                break;

            case PhysicalButtons::RIGHT:
                if (cursorX + 1 < currentCols()) cursorX++;
                break;

            case PhysicalButtons::UP:
                if (cursorY > 0) {
                    cursorY--;
                    cursorX = std::min(cursorX, currentCols());
                }
                break;

            case PhysicalButtons::DOWN:
                if (cursorY + 1 < currentRows()) {
                    cursorY++;
                    cursorX = std::min(cursorX, currentCols());
                }
                break;

            case PhysicalButtons::FUNC1:  // Backspace
                backspace();
                break;

            case PhysicalButtons::FUNC2:  // Shift / ABC/SYM
                toggleShiftOrSym();
                break;

            case PhysicalButtons::FUNC3:  // Space
                pressCurrentKey();
                break;

            case PhysicalButtons::FUNC4:  // Enter
                finished = true;
                break;
        }

        // Ввод текущего символа кнопкой "стрелка" мы НЕ делаем.
        // Ввод символа — отдельная функция системы.
        // Если нужно — можно включить, скажи.
    }

    // Ввод символа текущей клавиши (вызов из внешней кнопки "OK")
    void pressCurrentKey()
    {
        if (cursorY == specialRow()) {
            // нажаты спец-клавиши
            handleSpecialKey(cursorX);
        } else {
            char c = getCurrentLayout()[cursorY][cursorX];
            if (c != 0 && c != ' ')
                insertChar(c);
        }
    }

    // Отрисовка клавиатуры
    void draw()
    {
        
        gfx->setTextColor(RGB565_WHITE);
        gfx->setTextSize(2);

        // ----- 1) ФУНКЦИОНАЛЬНЫЕ КНОПКИ -----


        // ----- 2) Строка ввода (смещена вниз) -----
        gfx->setCursor(10, 40);
        gfx->print("Input: ");
        gfx->print(buffer);

        // ----- 3) Клавиатура -----
        drawKeyboard();
    }

private:
    Arduino_GFX *gfx;
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

    const char** getCurrentLayout() {
        switch (mode) {
            case ABC_LOWER: return layoutLower;
            case ABC_UPPER: return layoutUpper;
            case SYM:       return layoutSym;
        }
        return layoutLower;
    }

    int currentRows() const { return 4; }      // 3 строки + спец
    int specialRow() const { return 3; }
    int currentCols() const {
        if (cursorY == specialRow()) return 5;
        int len = strlen(getCurrentRow());
        return len;
    }

    const char* getCurrentRow() const {
        switch (mode) {
            case ABC_LOWER: return layoutLower[cursorY];
            case ABC_UPPER: return layoutUpper[cursorY];
            case SYM:       return layoutSym[cursorY];
        }
        return "";
    }

    void toggleShiftOrSym()
    {
        if (mode == SYM) {
            mode = ABC_LOWER;
        }
        else if (mode == ABC_LOWER) {
            mode = ABC_UPPER;
        }
        else if (mode == ABC_UPPER) {
            mode = ABC_LOWER;
        }
    }

    void handleSpecialKey(int x)
    {
        switch (x)
        {
            case 0: toggleShiftOrSym(); break;
            case 1: mode = (mode == SYM ? ABC_LOWER : SYM); break;
            case 2: insertChar(' '); break;
            case 3: backspace(); break;
            case 4: finished = true; break;
        }
    }

    void insertChar(char c)
    {
        size_t len = strlen(buffer);
        if (len + 1 < bufLen) {
            buffer[len] = c;
            buffer[len+1] = '\0';
        }
    }

    void backspace()
    {
        int n = strlen(buffer);
        if (n > 0) buffer[n-1] = '\0';
    }

    // ========== ОТРИСОВКА ==========

    void drawKeyboard()
    {
        const int keyH = 40;
        const int keyMargin = 4;

        int y = 90;  // смещение вниз, чтобы не перекрывать функциональные кнопки

        // ====== 3 строк обычных клавиш ======
        for (int r = 0; r < 3; r++)
        {
            const char *row = getCurrentLayout()[r];
            int cols = strlen(row);
            int totalW = cols * 40 + (cols - 1) * keyMargin;
            int x = (TFT_W - totalW) / 2;

            for (int c = 0; c < cols; c++)
            {
                bool active = (cursorY == r && cursorX == c);
                drawKeyChar(x, y, 40, keyH, row[c], active);
                x += 40 + keyMargin;
            }
            y += keyH + keyMargin;
        }

        // ====== 1 строка специальных кнопок ======
        int cols = 5;
        int keyW = 80;

        int totalW = cols * keyW + (cols - 1) * keyMargin;
        int x = (TFT_W - totalW) / 2;

        for (int c = 0; c < cols; c++)
        {
            bool active = (cursorY == specialRow() && cursorX == c);
            drawKeyLabel(x, y, keyW, keyH, specialNames[c], active);
            x += keyW + keyMargin;
        }
    }

    void drawKeyChar(int x, int y, int w, int h, char c, bool active)
    {
        uint16_t bg = active ? RGB565_BLUE : RGB565_GRAY;
        uint16_t fg = RGB565_WHITE;

        gfx->fillRect(x, y, w, h, bg);
        gfx->drawRect(x, y, w, h, RGB565_WHITE);

        gfx->setTextColor(fg);
        gfx->setCursor(x + w/2 - 6, y + h/2 - 8);
        gfx->write(c);
    }

    void drawKeyLabel(int x, int y, int w, int h, const char *label, bool active)
    {
        uint16_t bg = active ? RGB565_BLUE : RGB565_BLACK;
        uint16_t fg = RGB565_YELLOW;

        gfx->fillRect(x, y, w, h, bg);
        gfx->drawRect(x, y, w, h, RGB565_WHITE);

        gfx->setTextColor(fg);
        gfx->setCursor(x + (w - strlen(label) * 12) / 2, y + h/2 - 8);
        gfx->print(label);
    }
};
