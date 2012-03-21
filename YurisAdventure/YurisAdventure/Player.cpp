#pragma
#include "Player.h"

/*
	Creates a Player
*/
Player::Player(int hp, int mana, int attack) : Creature(hp, mana, attack)
{
	comboState = COMBO_NONE;
	actionTime = 0;
	buffTimer = 0;
	comboImageShower = NULL;
}
/*
	Process the Player Combo State for the given pressedKey
*/

void Player::processCombo(unsigned int pressedKey)
{

	if (comboState == COMBO_NONE) {

		switch(pressedKey) {
		case SWORD_ATTACK_KEY: comboState = COMBO_ATTACK; break;
		case MAGIC_ATTACK_KEY: comboState = COMBO_MAGIC; break;
		case S_KEY: comboState = COMBO_CROUCH; break;
		case ACTION_KEY: comboState = COMBO_NONE; break;
		default: break;
		}

	} else if (comboState == COMBO_ATTACK) {

		switch(pressedKey) {
		case SWORD_ATTACK_KEY: comboState = COMBO_ATTACK_ATTACK; break;
		case MAGIC_ATTACK_KEY: comboState = COMBO_NONE; break;
		case S_KEY: comboState = COMBO_NONE; break;
		case ACTION_KEY: comboState = COMBO_NONE; break;
		default: comboState = COMBO_NONE; break;
		}

	} else if (comboState == COMBO_MAGIC) {

		switch(pressedKey) {
		case SWORD_ATTACK_KEY: comboState = COMBO_NONE; break;
		case MAGIC_ATTACK_KEY: comboState = COMBO_MAGIC_MAGIC; break;
		case S_KEY: comboState = COMBO_NONE; break;
		case ACTION_KEY: comboState = COMBO_NONE; break;
		default: comboState = COMBO_NONE; break;
		}

	} else if (comboState == COMBO_CROUCH) {

		switch(pressedKey) {
		case SWORD_ATTACK_KEY: comboState = COMBO_NONE; break;
		case MAGIC_ATTACK_KEY: comboState = COMBO_NONE; break;
		case S_KEY: comboState = COMBO_CROUCH_CROUCH; break;
		case ACTION_KEY: comboState = COMBO_NONE; break;
		default: comboState = COMBO_NONE; break;
		}

	} else if (comboState == COMBO_ATTACK_ATTACK) {

		switch(pressedKey) {
		case SWORD_ATTACK_KEY: comboState = COMBO_NONE; break;
		case MAGIC_ATTACK_KEY: comboState = COMBO_NONE; break;
		case S_KEY: comboState = COMBO_NONE; break;
		case ACTION_KEY:
			// Increase Attack
			if (getCurrentMana() >= 30) {
				comboImageShower->showImage(ATTACK_INCREASED_IMAGE);
				setAttack(PLAYER_ATTACK * 2);
				useMana(30);
				buffTimer = 1000;
			}

			comboState = COMBO_NONE; break;
		default: comboState = COMBO_NONE; break;
		}

	} else if (comboState == COMBO_MAGIC_MAGIC) {

		switch(pressedKey) {
		case SWORD_ATTACK_KEY: comboState = COMBO_NONE; break;
		case MAGIC_ATTACK_KEY: comboState = COMBO_NONE; break;
		case S_KEY: comboState = COMBO_NONE; break;
		case ACTION_KEY:
			
			// Magic Penetration
			if (getCurrentMana() >= 30) {
				comboImageShower->showImage(MAGIC_PENETRATION_IMAGE);
				//setAttack(PLAYER_ATTACK * 2);
				useMana(30);
				buffTimer = 1000;
			}
			
			comboState = COMBO_NONE; break;
		default: comboState = COMBO_NONE; break;
		}

	} else if (comboState == COMBO_CROUCH_CROUCH) {

		switch(pressedKey) {
		case SWORD_ATTACK_KEY: comboState = COMBO_NONE; break;
		case MAGIC_ATTACK_KEY: comboState = COMBO_NONE; break;
		case S_KEY: comboState = COMBO_NONE; break;
		case ACTION_KEY:
			
			// Increase Defense
			if (getCurrentMana() >= 30) {
				comboImageShower->showImage(DEFENSE_INCREASE_IMAGE);
				setDefense(50);
				useMana(30);
				buffTimer = 1000;
			}
			
			comboState = COMBO_NONE; break;
		default: comboState = COMBO_NONE; break;
		}

	}
}

/*
	Updates the Sprite of this Player and make the needed changes when some animation stops
*/

void Player::updateSprite()
{

	// Updates Buffs
	updateBuffs();

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
		
		
		} else if (isCasting) {

			if (isFacingRight()) setCurrentState(CASTING_STATE_RIGHT);
			else setCurrentState(CASTING_STATE_LEFT);

			if (actionTime == 0)
				isCasting = false;
		
		
		} else{ // Is Idle

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
void Player::updateBuffs()
{

	if (buffTimer > 0) {


		buffTimer--;

	} else {
		setAttack(PLAYER_ATTACK);
		setDefense(0);
	}

}