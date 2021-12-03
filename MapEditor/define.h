#ifndef __define_h__
#define __define_h__

#define CLASS_NAME "MapEditor"

const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 768;
const int COLOR_DEPTH = 32;

const int DEFAULT_BLOCK_SIZE = 32;
const int BLOCK_X = 100;
const int BLOCK_Y = 100;

const int TOTAL_ARROW = 100;
const int TOTAL_SNOWBALL = 4;
const int TOTAL_ENEMY = 3;

const int TOTAL_BLOCK_SPRITE = 110;
const int TOTAL_BLOCK_SPRITE_X = 23;
const int TOTAL_BLOCK_SPRITE_Y = 20;

const int WORLDMAP_WIDTH = DEFAULT_BLOCK_SIZE * BLOCK_X;
const int WORLDMAP_HEIGHT = DEFAULT_BLOCK_SIZE * BLOCK_Y;

const int WORLDMAP_DATA_SIZE = WORLDMAP_WIDTH * WORLDMAP_HEIGHT * 3;

const int SCREEN_MAX_WIDTH = BLOCK_X * DEFAULT_BLOCK_SIZE - SCREEN_WIDTH;
const int SCREEN_MAX_HEIGHT = BLOCK_Y * DEFAULT_BLOCK_SIZE - SCREEN_HEIGHT;

const int COLOR_KEY = 0;
#endif