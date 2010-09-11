#include "GameState.h"

violetland::GameState::GameState() {
	TimeOfDay = 1.0;
	Begun = false;
	Works = true;
	Lost = false;
	Time = 0;
	HighScore = false;
	PlayerName = "";
}

void violetland::GameState::start(GameMode mode) {
	reset();

	switch (mode) {
	case GAMEMODE_SURVIVAL:
		Hardness = 9995.0;
		Lost = false;
		Paused = false;
		Begun = true;
		Time = 0;
		break;
	case GAMEMODE_SCENARIO:
		break;
	}
}

void violetland::GameState::end() {
	Works = false;
}

violetland::LifeForm* violetland::GameState::getLifeForm(string id) {
	return lifeForms.at(id);
}

vector<violetland::Blood> violetland::GameState::processExplosion(float x,
		float y, float damage, float range, bool affectPlayer) {
	vector<Blood> vBlood;

	if (lifeForms.empty())
		return vBlood;

	map<string, LifeForm*>::const_iterator iter;
	for (iter = lifeForms.begin(); iter != lifeForms.end(); ++iter) {
		LifeForm* lifeForm = iter->second;
		if (lifeForm->Type == LIFEFORM_PLAYER && !affectPlayer)
			continue;

		// Damage life forms in range
		float distance = Object::calculateDistance(x, y, lifeForm->X,
				lifeForm->Y) - lifeForm->HitR * lifeForm->Scale
				* lifeForm->getWidth();

		if (distance < range) {
			float dmg = (range - distance) / range * damage;
			lifeForm->setHealth(lifeForm->getHealth() - dmg);

			if (lifeForm->getHealth() == 0) {
				lifeForm->State = LIFEFORM_STATE_BURST;
			}

			// Blood

			Blood blood;

			blood.angle
					= Object::calculateAngle(lifeForm->X, lifeForm->Y, x, y);
			blood.x = lifeForm->X;
			blood.y = lifeForm->Y;
			blood.scale = lifeForm->Scale;
			blood.poisoned = lifeForm->Poisoned;

			vBlood.push_back(blood);
		}
	}

	return vBlood;
}

void violetland::GameState::process(int deltaTime) {
	if (!Lost) {
		Hardness -= deltaTime * 0.00012;
		Time += deltaTime;
	}
}

void violetland::GameState::reset() {
	clearMap<string, LifeForm*> (&lifeForms);
	clearVector<Powerup*> (&powerups);
	clearVector<Bullet*> (&bullets);
}

violetland::GameState::~GameState() {
	reset();
}