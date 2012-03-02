#pragma
#include "Player.h"

Player::Player(unsigned int hp, unsigned int attack) : Creature(hp, attack)
{
	comboState = COMBO_NONE;
	actionTime = 0;
}

void Player::processCombo(unsigned int pressedKey)
{

}

void Player::updateSprite()
{
	if (isAttacking() && actionTime == 0) {
		actionTime = 20;
		
		if (isFacingRight()) setCurrentState(ATTACKING_RIGHT_STATE);
		else setCurrentState(ATTACKING_LEFT_STATE);
	}

	if (actionTime == 1) {
		stopAttack();

		if (isFacingRight()) setCurrentState(IDLE_STATE_RIGHT);
		else setCurrentState(IDLE_STATE_LEFT);
	}

	if (actionTime > 0) actionTime--;

	AnimatedSprite::updateSprite();
}