#include "ui_widget.h"

class TuneWidget : public UIWidget
{
	bool bTouching = false;
	vec2i touchPos;

public:
	TuneWidget();
	~TuneWidget();

	

	void render(ApplicationPlatform* platformInstance) override;
	bool handlePhysicalButton(PhysicalButtons btnId) override;


	void handleTouchEvent(TouchEvents event, vec2i pt) override;

};