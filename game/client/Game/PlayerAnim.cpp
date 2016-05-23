#include "PlayerAnim.h"

#include <time.h>  

PlayerAnim::PlayerAnim(PlayerModel *model) : SGeode()
{
	isVisible = false;
	this->model = model;
}
	

PlayerAnim::~PlayerAnim()
{
	
}

void PlayerAnim::draw(DrawData& data) {
	if (isVisible) {
		float time = clock() / float(CLOCKS_PER_SEC);
		float delta = time - prevTime;
		prevTime = time;

		currAnimTime += min(delta, 0.05f);

		data.animTime = currAnimTime;
		model->draw(data);
	}
}

void PlayerAnim::update(UpdateData& data) {
	if (isVisible) {
		model->update(data);
	}
}

void PlayerAnim::setVisible(bool visible) {
	this->isVisible = visible;
	if (!visible) {
		/*currAnimTime = 0;
		prevTime = 0; */
	}
}

PlayerModel* PlayerAnim::getPlayerModel() {
	return this->model;
}