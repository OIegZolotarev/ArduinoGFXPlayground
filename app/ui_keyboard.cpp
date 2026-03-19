#include "ui_controller.h"
#include "ui_keyboard.h"



OnScreenKeyboard::OnScreenKeyboard(Arduino_GFX* gfx, char* buffer, size_t bufLen) : gfx(gfx), buffer(buffer), bufLen(bufLen)
{
	buffer[0] = '\0';
}

bool OnScreenKeyboard::isFinished() const
{
	return finished;
}

void OnScreenKeyboard::handlePhysicalButton(PhysicalButtons btn)
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

	// ¬вод текущего символа кнопкой "стрелка" мы Ќ≈ делаем.
	// ¬вод символа Ч отдельна€ функци€ системы.
	// ≈сли нужно Ч можно включить, скажи.
}

void OnScreenKeyboard::pressCurrentKey()
{
	if (cursorY == specialRow()) {
		// нажаты спец-клавиши
		handleSpecialKey(cursorX);
	}
	else {
		char c = getCurrentLayout()[cursorY][cursorX];
		if (c != 0 && c != ' ')
			insertChar(c);
	}
}

void OnScreenKeyboard::draw()
{

	gfx->setTextColor(RGB565_WHITE);
	gfx->setTextSize(2);

	// ----- 1) ‘”Ќ ÷»ќЌјЋ№Ќџ≈  Ќќѕ » -----


	// ----- 2) —трока ввода (смещена вниз) -----
	gfx->setCursor(10, 40);
	gfx->print("Input: ");
	gfx->print(buffer);

	// ----- 3)  лавиатура -----
	drawKeyboard();
}

const char** OnScreenKeyboard::getCurrentLayout()
{
	switch (mode) {
	case ABC_LOWER: return layoutLower;
	case ABC_UPPER: return layoutUpper;
	case SYM:       return layoutSym;
	}
	return layoutLower;
}

int OnScreenKeyboard::currentRows() const
{
	return 4;
}

int OnScreenKeyboard::specialRow() const
{
	return 3;
}

int OnScreenKeyboard::currentCols() const
{
	if (cursorY == specialRow()) return 5;
	int len = strlen(getCurrentRow());
	return len;
}

const char* OnScreenKeyboard::getCurrentRow() const
{
	switch (mode) {
	case ABC_LOWER: return layoutLower[cursorY];
	case ABC_UPPER: return layoutUpper[cursorY];
	case SYM:       return layoutSym[cursorY];
	}
	return "";
}

void OnScreenKeyboard::toggleShiftOrSym()
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

void OnScreenKeyboard::handleSpecialKey(int x)
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

void OnScreenKeyboard::insertChar(char c)
{
	size_t len = strlen(buffer);
	if (len + 1 < bufLen) {
		buffer[len] = c;
		buffer[len + 1] = '\0';
	}
}

void OnScreenKeyboard::backspace()
{
	int n = strlen(buffer);
	if (n > 0) buffer[n - 1] = '\0';
}

void OnScreenKeyboard::drawKeyboard()
{
	const int keyH = 40;
	const int keyMargin = 4;

	int y = 90;  // смещение вниз, чтобы не перекрывать функциональные кнопки

	// ====== 3 строк обычных клавиш ======
	for (int r = 0; r < 3; r++)
	{
		const char* row = getCurrentLayout()[r];
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

void OnScreenKeyboard::drawKeyChar(int x, int y, int w, int h, char c, bool active)
{
	uint16_t bg = active ? RGB565_BLUE : RGB565_GRAY;
	uint16_t fg = RGB565_WHITE;

	gfx->fillRect(x, y, w, h, bg);
	gfx->drawRect(x, y, w, h, RGB565_WHITE);

	gfx->setTextColor(fg);
	gfx->setCursor(x + w / 2 - 6, y + h / 2 - 8);
	gfx->write(c);
}

void OnScreenKeyboard::drawKeyLabel(int x, int y, int w, int h, const char* label, bool active)
{
	uint16_t bg = active ? RGB565_BLUE : RGB565_BLACK;
	uint16_t fg = RGB565_YELLOW;

	gfx->fillRect(x, y, w, h, bg);
	gfx->drawRect(x, y, w, h, RGB565_WHITE);

	gfx->setTextColor(fg);
	gfx->setCursor(x + (w - strlen(label) * 12) / 2, y + h / 2 - 8);
	gfx->print(label);
}
