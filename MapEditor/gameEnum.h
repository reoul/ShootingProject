#ifndef __game_enum_h__
#define __game_enum_h__

enum MOD { MOD_INTRO = 0, MOD_GAME, MOD_EDITOR, MOD_QUIT };		//���� ��Ʈ�� ȭ������,����ȭ������, �ʿ��������� ����

enum DISTANCE_STATE { LEFT = 1, RIGHT, UP, DOWN, LEFTUP, RIGHTUP, LEFTDOWN, RIGHTDOWN };	//������ ����

enum ACTION_STATE { IDLE = 0, MOVE, RUN, ROLL, ATTACK, SLEEP, DEAD, SKILL, FAINT};	//���� �ൿ�� ���ϴ���

enum OBJECT_STATE { PLYAER = 0, ARROW, BOSS, SNOWBALL, BLOCK, EDITOR_BLOCK, ENEMY };

enum EDIT_STATE { EDIT_BOSS = 0, EDIT_FOREST, EDIT_BOSS_EDIT, EDIT_FOREST_EDIT };

enum EDIT_WINDOW { EDIT_BLOCK_WINDOW1,EDIT_BLOCK_WINDOW2};

#endif