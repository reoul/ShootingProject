#pragma once

enum class EMod { Intro, Game, Editor, Quit }; //지금 인트로 화면인지,게임화면인지, 맵에디터인지 구별

enum class EDirection { Left = 1, Right, Up, Down, LeftUp, RightUp, LeftDown, RightDown }; //방향을 설명

enum class EAction { Idle, Move, Run, Roll, Attack, Sleep, Dead, Skill, Faint }; //무슨 행동을 취하는지

enum class EObjectType { Player, Arrow, Boss, Snowball, Block, EditorBlock, Enemy };

enum class EEditState { Boss, Forest, BossEdit, ForestEdit };

enum class EEditWindow { BlockWindow1, BlockWindow2 };
