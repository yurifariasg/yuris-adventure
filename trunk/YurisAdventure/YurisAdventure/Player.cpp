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
	attackingTime = 0;
	comboImageShower = NULL;
	isPenetrationActive = false;
	isTakingDamage = false;
	isCrouched = false;
	aura = NULL;
	comboAnimationTimer = 0;
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
			if (getCurrentMana() >= ATTACK_COMBO_MANA && !hasBuffActives()) {
				comboImageShower->showImage(ATTACK_INCREASED_IMAGE);
				setAttack(PLAYER_ATTACK * 2);
				useMana(ATTACK_COMBO_MANA);
				buffTimer = BUFF_TIME;
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
			if (getCurrentMana() >= MAGIC_COMBO_MANA && !hasBuffActives()) {
				comboImageShower->showImage(MAGIC_PENETRATION_IMAGE);
				isPenetrationActive = true;
				useMana(MAGIC_COMBO_MANA);
				buffTimer = BUFF_TIME;
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
			if (getCurrentMana() >= DEFENSE_COMBO_MANA && !hasBuffActives()) {
				comboImageShower->showImage(DEFENSE_INCREASE_IMAGE);
				setDefense(INCREASE_DEFENSE_PERCENTAGE);
				useMana(DEFENSE_COMBO_MANA);
				buffTimer = BUFF_TIME;
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

			if (attackingTime == 0)
				isCasting = false;
		
		
		} else if (isCrouched) {
		
			if (isFacingRight()) setCurrentState(CROUCH_STATE_RIGHT);
			else setCurrentState(CROUCH_STATE_LEFT);
			isCrouched = false;
		
		} else if (isTakingDamage) {

			if (actionTime == 0 || isDead() ||
				(getCurrentState() != TAKING_DAMAGE_LEFT &&
				getCurrentState() != TAKING_DAMAGE_RIGHT)) isTakingDamage = false;
			actionTime--;

		} else { // is idle

			if (isFacingRight()) setCurrentState(IDLE_STATE_RIGHT);
			else setCurrentState(IDLE_STATE_LEFT);

		}
	}

	if (isAttacking() && attackingTime == 0) {
		attackingTime = TIME_FOR_ATTACK_ANIMATION;
	}

	if (attackingTime == 1 && isAttacking())
		stopAttack();

	} else if (attackingTime == 0 || isAttacking()) { // is dead endif statement
		stopAttack();

		if (isFacingRight()) setCurrentState(DYING_STATE_RIGHT);
		else setCurrentState(DYING_STATE_LEFT);

		attackingTime = TIME_FOR_DEATH_ANIMATION;
	}

	if (attackingTime > 0) {
		if((isDead() && attackingTime > 1)) { attackingTime--;
		} else if (!isDead()) { attackingTime--; }
	}

	AnimatedSprite::updateSprite();
}
void Player::updateBuffs()
{

	if (buffTimer > 0) {
		// Sets aura !

		if (aura != NULL) {
			if (comboAnimationTimer < TIME_FOR_COMBO_ANIMATION) {
				aura->setAlpha(255);
				aura->getPhysicalProperties()->setX(
					getPhysicalProperties()->getX()- 65);

				aura->getPhysicalProperties()->setY(
					getPhysicalProperties()->getY() - 70);
			} else {
				aura->setAlpha(0);
			}

		}

		comboAnimationTimer++;
		buffTimer--;

	} else {
		setAttack(PLAYER_ATTACK);
		setDefense(0);
		isPenetrationActive = false;
		comboAnimationTimer = 0;
		if (aura != NULL) {
			aura->setAlpha(0);

		}
	}

}

void Player::crouch()
{
	isCrouched = true;
}