#include "ui_controller.h"
#include "ui_tune_widget.h"



TuneWidget::TuneWidget()
{

}

TuneWidget::~TuneWidget()
{

}

void TuneWidget::render(ApplicationPlatform* platformInstance)
{
	auto gfx = platformInstance->gfxInstance();

	appInstance->setTextSize(2);
	gfx->setCursor(0, 100);

	
	if (!bTouching)
		gfx->print("Not touching");
	else
	{
		char temp[128];
		snprintf(temp, 128, "Touching at: [%d, %d]", touchPos.x, touchPos.y);
		gfx->print(temp);

		gfx->drawCircle(touchPos.x, touchPos.y, 32, RGB565_LIGHTGREEN);
	}
}

bool TuneWidget::handlePhysicalButton(PhysicalButtons btnId)
{
	return false;
}

void TuneWidget::handleTouchEvent(TouchEvents event, vec2i pt)
{
	switch (event)
	{
	case TouchEvents::Down:
		bTouching = true;
		touchPos = pt;
		break;
	case TouchEvents::Drag:		
		touchPos = pt;
		break;
	case TouchEvents::Up:
		bTouching = false;
		touchPos = pt;
		break;

	}
}
