#pragma
#include "Player.h"

/*
	Creates a Player
*/
Player::Player(int hp, int mana, int attack) : Creature(hp, mana, attack)
{
	comboState = COMBO_NONE;
	actionTime = 0;
}
/*
	Process the Player Combo State for the given pressedKey
*/

void Player::processCombo(unsigned int pressedKey)
{
	if (pressedKey == SWORD_ATTACK_KEY) {

		if (comboState == COMBO_NONE) {

			comboState = COMBO_ATTACK;

		} else if (comboState == COMBO_ATTACK) {
			comboState = COMBO_ATTACK_ATTACK;
			// Start Combo Here - TO_DO
		}

	} else if (pressedKey == MAGIC_ATTACK_KEY) {

		if (comboState == COMBO_NONE) {

			comboState = COMBO_MAGIC;

		} else if (comboState == COMBO_MAGIC) {
			comboState = COMBO_MAGIC_MAGIC;
			// Start Combo Here - TO_DO
		}

	} else if (pressedKey == S_KEY) { // Down Key

		if (comboState == COMBO_NONE) {

			comboState = COMBO_CROUCH;

		} else if (comboState == COMBO_CROUCH) {
			comboState = COMBO_CROUCH_CROUCH;
			// Start Combo Here - TO_DO
		}
	}
}

/*
	Updates the Sprite of this Player and make the needed changes when some animation stops
*/

void Player::updateSprite()
{

	if (!isDead()) { // is dead if statement

	bool isFlying = getPhysicalProperties()->getBuoyancy();

	if (isFlying) {

		if (isAttacking()) {
			// Jump Attack

			if (isFacingRight()) setCurrentState(JUMPING_STATE_RIGHT);
			else setCurrentState(JUMPING_STATE_LEFT);

		} else {
			if (isFacingRight()) setCurrentState(JUMPING_STATE_RIGHT);
			else setCurrentState(JUMPING_STATE_LEFT);
		}

	} else {
		// Idle Attacking or Moving

		if (getPhysicalProperties()->getVelocityX() != 0) {
			// Moving

			if (isFacingRight()) setCurrentState(MOVING_RIGHT_STATE);
			else setCurrentState(MOVING_LEFT_STATE);

		} else if (isAttacking()) {

			if (isFacingRight()) setCurrentState(ATTACKING_RIGHT_STATE);
			else setCurrentState(ATTACKING_LEFT_STATE);

		} else if (isCharging) {

			if (isFacingRight()) setCurrentState(CHARGING_STATE_RIGHT);
			else setCurrentState(CHARGING_STATE_LEFT);
		
		
		} else { // Is Idle

			if (isFacingRight()) setCurrentState(IDLE_STATE_RIGHT);
			else setCurrentState(IDLE_STATE_LEFT);

		}
	}

	if (isAttacking() && actionTime == 0) {
		actionTime = 20;
	}

	if (actionTime == 1 && isAttacking())
		stopAttack();

	} else if (actionTime == 0 || isAttacking()) { // is dead endif statement
		stopAttack();

		if (isFacingRight())
			setCurrentState(DYING_STATE_RIGHT);
		else setCurrentState(DYING_STATE_LEFT);

		actionTime = 200;
	}

	if (actionTime > 0) {
		if((isDead() && actionTime > 1)) { actionTime--;
		} else if (!isDead()) { actionTime--; }
	}

	AnimatedSprite::updateSprite();
}