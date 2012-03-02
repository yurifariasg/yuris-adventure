#pragma once

/*
	Enum to hold the current Player Combo State
	(It will work like a FSM)
*/

enum PlayerComboState
{
	COMBO_NONE,

	COMBO_ATTACK,
	COMBO_ATTACK_ATTACK,

	COMBO_MAGIC,
	COMBO_MAGIC_MAGIC,

	COMBO_CROUCH,
	COMBO_CROUCH_CROUCH
};