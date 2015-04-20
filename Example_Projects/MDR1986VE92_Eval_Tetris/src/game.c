/*
 * game.c
 *
 *  Created on: 12.12.2012
 *      Author: e.khairullin
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "types.h"
#include "lcd.h"
#include "gl.h"
#include "text.h"
#include "joystick.h"
#include "leds.h"
#include "menu.h"
#include "time.h"
#include "lowpower.h"
#include "Demo_Init.h"

#include <stdlib.h>

#include "game.h"

struct {
	ushort updTime;
	ushort updDiv;
	uchar state;
} game = {UPD_PERIOD, 1, 0};

//LINE_H, // ----
//LINE_V,     // |
//
//TRI_H_U,    // .|.
//TRI_H_D,    // `|`
//TRI_V_R,    // |-
//TRI_V_L,    // -|
//
//G_H_U,  // |__
//G_H_D,  // --,
//G_V_R,  // |`
//G_V_L,  // _|
//
//CUBE    // BB

Figure figs[FIGS_NUM] = {
	{{{0, 0}, {0, 1}, {0, 2}, {0, 3}}}, // ----
	{{{0, 0}, {1, 0}, {2, 0}, {3, 0}}}, // |

	{{{0, 1}, {1, 0}, {1, 1}, {1, 2}}}, // .|.
	{{{0, 0}, {0, 1}, {0, 2}, {1, 1}}}, // `|`
	{{{0, 1}, {1, 0}, {1, 1}, {2, 1}}}, // |-
	{{{0, 0}, {1, 0}, {1, 1}, {2, 0}}}, // -|

	{{{0, 1}, {1, 1}, {2, 0}, {2, 1}}}, // |_
	{{{0, 0}, {0, 1}, {0, 2}, {1, 2}}}, // ,--
	{{{0, 0}, {0, 1}, {1, 0}, {2, 0}}}, // `|
	{{{0, 0}, {1, 0}, {1, 1}, {1, 2}}},  // __i

	{{{0, 0}, {0, 1}, {1, 0}, {1, 1}}} // BB

};

Bool field[MAXIM_X][MAXIM_Y] = {FALSE};

Bool fieldChanged = TRUE;

struct {
	Figure_Type type;
	uchar x;
	uchar y;
} curFig = {LINE_H, 0, MAXIM_Y/2 - BLOCKS_NUM/2};

volatile uint32_t cur_ms;

void SysTick_Handler()
{
    cur_ms++;
}

void Delay(uint32_t ms)
{
    uint32_t last = cur_ms;
    while(cur_ms - last < ms);
}

void InitSystick()
{
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock/1000);
}

void DrawBlock(uchar x, uchar y)
{
	uchar i;
	uchar j;

	x *= BLOCK_DIV;
	y *= BLOCK_DIV;

	for(i = 0; i < BLOCK_DIV; i++)
	{
		for(j = 0; j < BLOCK_DIV; j++)
		{
			if(!((i == 1 && (j == 1 || j == 2))||(i == 2 && (j == 1 || j == 2))))
				LCD_PutPixel(x + i, y + j);
		}
	}
}

void DrawFigure(Figure_Type fig, uchar x, uchar y)
{
	uchar i;

	for(i = 0; i < BLOCKS_NUM; i++)
	{
		DrawBlock(figs[fig].blocks[i].x + x, figs[fig].blocks[i].y + y);
	}
}

// Инициализация
void Init()
{
	Demo_Init();
	LCD_CLS();
	InitSystick();
}
// Исполнение
void Run()
{
	while(IsGameOver() != TRUE)
	{
		Clean();
		Update();
		Logic();
		Draw();
		Delay(game.updTime/game.updDiv);
	}

	GameOver();
}

void GameOver()
{
	LCD_CLS();
	LCD_PUTS(36, 28, "GAME OVER");
}

Bool IsGameOver()
{
	Bool over = FALSE;
	uchar i;

	for(i = 0; i < MAXIM_Y; i++)
	{
		if(field[0][i] == TRUE)
		{
			over = TRUE;
			break;
		}
	}

	return over;
}

void Clean()
{
	// Clear figure
	CurrentMethod = MET_XOR;
	DrawFigure(curFig.type, curFig.x, curFig.y);
}

void Update()
{
	if(CollisionX() == FALSE)
	{
		curFig.x++;
		game.updDiv = 1;
		ControlUpdate();
	}
	else
	{
		FigToField();
		NewFig(RandomFig());
		fieldChanged = TRUE;
	}
}

Figure_Type RandomFig()
{
	srand(cur_ms);
	return rand() * (FIGS_NUM - 1) / RAND_MAX;
}

void Logic()
{
	short i;
	uchar shift = 0;

	for(i = MAXIM_X - 1; i >= 0; i--)
	{
		if(IsRowFull(i) == TRUE)
		{
			ClearRow(i);
			shift++;
		}
		else
		{
			if(shift != 0)
				ShiftRow(i, shift);
		}
	}

	if(shift != 0)
		fieldChanged = TRUE;
}

void ClearRow(uchar row)
{
	uchar i;

	for (i = 0; i < MAXIM_Y; i++) {
		field[row][i] = FALSE;
	}
}

Bool IsRowFull(uchar row)
{
	Bool full = TRUE;
	uchar i;

	for (i = 0; i < MAXIM_Y; i++) {
		if(field[row][i] == FALSE)
		{
			full = FALSE;
			break;
		}
	}

	return full;
}

void ShiftRow(uchar row, uchar shift)
{
	uchar i;

	for (i = 0; i < MAXIM_Y; i++) {
		field[row + shift][i] = field[row][i];
		field[row][i] = FALSE;
	}

}

void ControlUpdate()
{
	switch (GetKey()) {
		case UP: {
			if(CollisionYR() == FALSE)
				curFig.y--;
			break;
		}
		case DOWN: {
			if(CollisionYL() == FALSE)
				curFig.y++;
			break;
		}
		case RIGHT: {
			game.updDiv = 20;
			break;
		}
		case SEL: {
			RotateFig();
			break;
		}

		default:
			break;
	}
}

void RotateFig()
{
	if(IsRotatable() == TRUE)
	{
		switch (curFig.type) {
			case LINE_H: {
				curFig.type = LINE_V;
				curFig.x -= 2;
				curFig.y++;
				break;
			}
			case LINE_V: {
				curFig.type = LINE_H;
				curFig.x += 2;
				curFig.y--;
				break;
			}

			case TRI_H_D: {
				curFig.type = TRI_V_L;
				curFig.x--;
				curFig.y++;
				break;
			}
			case TRI_V_L: {
				curFig.type = TRI_H_U;
				curFig.y--;
				break;
			}
			case TRI_H_U: {
				curFig.type = TRI_V_R;
				break;
			}
			case TRI_V_R: {
				curFig.type = TRI_H_D;
				curFig.x++;
				break;
			}

			case G_H_D: {
				curFig.type = G_V_L;
				curFig.x--;
				curFig.y++;
				break;
			}
			case G_V_L: {
				curFig.type = G_H_U;
				curFig.y--;
				break;
			}
			case G_H_U: {
				curFig.type = G_V_R;
				break;
			}
			case G_V_R: {
				curFig.type = G_H_D;
				curFig.x++;
				break;
			}
			default:
				break;
		}
	}
}

Bool IsRotatable()
{
	Bool rotatable = FALSE;
	short x;
	short y;
	uchar n;

	switch (curFig.type) {
		case LINE_H: {
			x = curFig.x - 2;
			y = curFig.y;
			n = BLOCKS_NUM;
			break;
		}
		case LINE_V: {
			x = curFig.x;
			y = curFig.y - 1;
			n = BLOCKS_NUM;
			break;
		}
		case TRI_V_L:
		case G_V_L:
		{
			x = curFig.x;
			y = curFig.y - 1;
			n = BLOCKS_NUM - 1;
			break;
		}
		case TRI_H_D:
		case G_H_D:
		{
			x = curFig.x - 1;
			y = curFig.y;
			n = BLOCKS_NUM - 1;
			break;
		}
		case TRI_H_U:
		case TRI_V_R:
		case G_V_R:
		case G_H_U:
		{
			x = curFig.x;
			y = curFig.y;
			n = BLOCKS_NUM - 1;
			break;
		}

		default:
			break;
	}

	if(IsCubeContained(n, x, y) == TRUE)
		rotatable = IsCubeEmpty(n, x, y);

	return rotatable;
}

Bool IsCubeContained(uchar n, short x, short y)
{
	Bool contained = FALSE;

	if(y >= 0 && (y + n) <= MAXIM_Y && x >= 0 && (x + n) <= MAXIM_X)
		contained = TRUE;

	return contained;
}

Bool IsCubeEmpty(uchar n, uchar x, uchar y)
{
	Bool empty = TRUE;
	uchar i;
	uchar j;

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			if(field[x + i][y + j] == TRUE)
			{
				empty = FALSE;
				break;
			}
		}

		if(empty == FALSE)
			break;
	}

	return empty;
}

Bool CollisionYL()
{
	Bool collide = FALSE;
	uchar i = 0;
	uchar len;

	switch (curFig.type) {
		case LINE_H: {
			len = BLOCKS_NUM;
			break;
		}
		case LINE_V: {
			len = 1;
			break;
		}
		case TRI_H_D:
		case TRI_H_U:
		case G_H_D:
		case G_H_U:
		{
			len = 3;
			break;
		}
		case TRI_V_L:
		case TRI_V_R:
		case G_V_L:
		case G_V_R:
		case CUBE:
		{
			len = 2;
			break;
		}
		default:
			break;
	}

	if(curFig.y + len == MAXIM_Y)
	{
		collide = TRUE;
	}
	else
	{
		collide = IsCollideFieldY(TRUE);
	}

	return collide;
}

Bool CollisionYR()
{
	Bool collide = FALSE;

	if(curFig.y == 0)
	{
		collide = TRUE;
	}
	else
	{
		collide = IsCollideFieldY(FALSE);
	}

	return collide;
}

void NewFig(Figure_Type fig)
{
	curFig.x = 0;
	curFig.type = fig;

	switch(fig)
	{
		case LINE_H:
		case TRI_H_U:
		case TRI_H_D:
		{
			curFig.y = MAXIM_Y/2 - 4/2;
			break;
		}
		case LINE_V:
		case TRI_V_R:
		case TRI_V_L:
		case G_H_U:
		case G_H_D:
		case G_V_L:
		case G_V_R:
		case CUBE:
		{
			curFig.y = MAXIM_Y/2 - 1;
			break;
		}
	}
}

void FigToField()
{
	uchar i;
	Block* blocks = figs[curFig.type].blocks;

	for(i = 0; i < BLOCKS_NUM; i++)
	{
		field[blocks[i].x + curFig.x][blocks[i].y + curFig.y] = TRUE;
	}
}

Bool CollisionX()
{
	Bool collide = FALSE;
	uchar len;

	switch(curFig.type)
	{
		case LINE_H: {
			len = 1;
			break;
		}
		case LINE_V: {
			len = BLOCKS_NUM;
			break;
		}
		case TRI_H_D:
		case TRI_H_U:
		case G_H_D:
		case G_H_U:
		case CUBE:
		{
			len = 2;
			break;
		}
		case TRI_V_L:
		case TRI_V_R:
		case G_V_L:
		case G_V_R:
		{
			len = 3;
			break;
		}
	}

	if(curFig.x + len == MAXIM_X)
	{
		collide = TRUE;
	}
	else
	{
		collide = IsCollideFieldX();
	}

	return collide;
}

Bool IsCollideFieldX()
{
	Bool collide = FALSE;
	Figure* fig = &(figs[curFig.type]);
	uchar i;

	for(i = 0; i < BLOCKS_NUM; i++)
	{
		if(field[fig->blocks[i].x + curFig.x + 1][fig->blocks[i].y + curFig.y]
		                                          == TRUE)
		{
			collide = TRUE;
			break;
		}
	}

	return collide;
}

Bool IsCollideFieldY(Bool left)
{
	Bool collide = FALSE;
	Figure* fig = &(figs[curFig.type]);
	uchar i;
	uchar k;

	if(left == TRUE)
		k = 1;
	else k = -1;

	for(i = 0; i < BLOCKS_NUM; i++)
	{
		if(field[fig->blocks[i].x + curFig.x][fig->blocks[i].y + curFig.y + k]
		                                      == TRUE)
		{
			collide = TRUE;
			break;
		}
	}

	return collide;
}

void Draw()
{
	uchar i;
	uchar j;

	// Draw field
	if(fieldChanged == TRUE)
	{
		LCD_CLS();

		CurrentMethod = MET_OR;
		for(i = 0; i < MAXIM_X; i++)
		{
			for(j = 0; j < MAXIM_Y; j++)
			{
				if(field[i][j] != FALSE)
				{
					DrawBlock(i, j);
				}
			}
		}

		fieldChanged = FALSE;
	}

	// Draw current figure
	CurrentMethod = MET_OR;
	DrawFigure(curFig.type, curFig.x, curFig.y);
}

