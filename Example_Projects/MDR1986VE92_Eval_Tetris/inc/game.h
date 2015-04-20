/*
 * game.h
 *
 *  Created on: 12.12.2012
 *      Author: e.khairullin
 */

#ifndef GAME_H_
#define GAME_H_

#define BLOCK_DIV 4
#define BLOCKS_NUM 4

#define FIGS_NUM 11

#define MAXIM_X (MAX_X+1)/BLOCK_DIV
#define MAXIM_Y (MAX_Y+1)/BLOCK_DIV

#define UPD_PERIOD 300

typedef uint8_t uchar;
typedef uint16_t ushort;

typedef struct {
    uchar x;
    uchar y;
} Block;

typedef enum{
    LINE_H = 0, // ----
    LINE_V,     // |

    TRI_H_U,    // .|.
    TRI_H_D,    // `|`
    TRI_V_R,    // |-
    TRI_V_L,    // -|

	G_V_R,  // |_
	G_H_D,  // ,--
	G_V_L,  // `|
	G_H_U,  // __i

    CUBE    // BB
} Figure_Type;

typedef struct {
    Block blocks[BLOCKS_NUM];
} Figure;

void SysTick_Handler();

void Delay(uint32_t ms);

void InitSystick();

void DrawBlock(uchar x, uchar y);
void DrawFigure(Figure_Type fig, uchar x, uchar y);
//void DrawBlockB(Block* block);
// Инициализация
void Init();
// Исполнение
void Run();
//
void Update();
void Draw();
Bool Collision();

void NewFig(Figure_Type fig);
void FigToField();
Bool CollisionX();
void ControlUpdate();
Bool CollisionYL();
Bool CollisionYR();
void Clean();
void Logic();
Bool IsRowFull(uchar row);
void ShiftRow(uchar row, uchar shift);
void ClearRow(uchar row);
void RotateFig();
Bool IsRotatable();
Bool IsCollideFieldX();
Bool IsCollideFieldY();
Figure_Type RandomFig();
Bool IsCubeContained(uchar n, short x, short y);
Bool IsCubeEmpty(uchar n, uchar x, uchar y);
Bool IsGameOver();
void GameOver();

#endif /* GAME_H_ */

