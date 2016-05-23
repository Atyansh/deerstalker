#pragma once

#include "PlayerModel.h"

class PlayerAnim : public SGeode {
private:
	bool isVisible;
	PlayerModel *model;

	//Animation time
	float prevTime = 0;
	float currAnimTime = 0;

public:
	PlayerAnim(PlayerModel *model);
	~PlayerAnim();

	virtual void update(UpdateData&);
	virtual void draw(DrawData&);
	void setVisible(bool visible);
	PlayerModel* getPlayerModel();

};