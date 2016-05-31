#pragma once

#include "PlayerModel.h"

class PlayerAnim : public SGeode {
private:
	bool isVisible;
	PlayerModel *model;
	int id;

	//Animation time
	float prevTime = 0;
	float currAnimTime = 0;

public:
	PlayerAnim(PlayerModel *model, int id);
	~PlayerAnim();

	virtual void update(UpdateData&);
	virtual void draw(DrawData&);
	void setVisible(bool visible);
	PlayerModel* getPlayerModel();

};