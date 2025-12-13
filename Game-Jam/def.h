#pragma once

#define WINDOW_WIDTH      1920
#define WINDOW_HEIGHT     1080
#define WINDOW_FRAME_RATE 60
//#define WINDOW_FULL_SCREEN

#define PIXELS_PER_UNIT   10

#define PLAYER_SPEED_X    10.0f
#define PLAYER_SPEED_Y    10.0f
#define PLAYER_SPEED      sqrt(PLAYER_SPEED_X * PLAYER_SPEED_X + PLAYER_SPEED_Y * PLAYER_SPEED_Y)

#define PLAYGROUND_MARGIN_LEFT    303
#define PLAYGROUND_MARGIN_RIGHT   243
#define PLAYGROUND_MARGIN_TOP     135
#define PLAYGROUND_MARGIN_BOTTOM  145

#define PLAYER_WIDTH              160
#define PLAYER_HEIGHT             182
#define WALL_VERTICAL_WIDTH       50
#define WALL_VERTICAL_HEIGHT      119
#define WALL_HORIZONTAL_WIDTH     178
#define WALL_HORIZONTAL_HEIGHT    50
#define KEY_WIDTH                 71
#define KEY_HEIGHT                82
#define GATE_WIDTH                202
#define GATE_HEIGHT               298

#define WALL_SPEED_INC_MULTIPLIER 2.5f
#define WALL_SPEED_INC_TIME       5.0f