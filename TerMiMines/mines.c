/*
* Copyright 2016 Riccardo Paolo Bestetti <riccardo [dt] kyogre [at] live [dt] it>
*
* This file is part of TerMiMines.
*
* TerMiMines is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Nome-Programma is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with TerMiMines.  If not, see <http:/*www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>

#include "mines.h"

int mines_init_board(unsigned int width, unsigned int height, MinesRuleset *ruleset, MinesBoard *dest)
{
	unsigned int i;
	MinesCell **m;

	/* all this could be done in a single malloc, but I don't care because virual memory space :) */

	m = malloc(width * sizeof(MinesCell *)); /* space for column pointers board[x] */

	if (m == NULL)
	{
		fprintf(stderr, "mines_init_board: failed to allocate space for column pointers");
		return 0;
	}

	/* set some fields of the MinesBoard struct to allow mines_destroy_board to work in case of an error */
	dest->width = width;
	dest->height = height;
	dest->board = m;

	for (i = 0; i < width; i++) /* space for elements board[x][y] */
	{
		m[i] = malloc(height * sizeof(MinesCell));

		if (m[i] == NULL)
		{
			fprintf(stderr, "mines_init_board: failed to allocate space for row %ui", i);
			mines_destroy_board(dest);
			return 0;
		}
	}

	/* set all the other fields */
	dest->ruleset = ruleset;
	dest->state = New_Game;
	dest->marks = 0;
	dest->flags = 0;
	dest->mines = 0;

	return 1;
}

int mines_destroy_board(MinesBoard *b)
{
	unsigned int i;

	if (b->board == NULL)
	{
		return 1;
	}

	/* try all rows skipping NULLs */
	for (i = 0; i < b->width; i++)
	{
		if (b->board[i] != NULL)
		{
			free(b->board[i]);
		}
	}

	free(b->board);

	return 1;
}

void mines_populate_board(MinesBoard * b, int seed, MinesBoardPopulator populator)
{
	unsigned int x, y;

	/* simple 2d iteration, all the magic happens in the MinesBoardPopulator typedef */
	for (x = 0; x < b->width; x++)
	{
		for (y = 0; y < b->height; y++)
		{
			seed = populator(&b->board[x][y], x, y, seed);
		}
	}
}

void mines_compute_board(MinesBoard *b)
{
	unsigned int mines;
	unsigned int x, y;
	unsigned int xl, yl;
	char acc;

	mines = 0;

	/* simple 2d iteration, all the magic happens in the MinesBoardPopulator typedef */
	for (x = 0; x < b->width; x++)
	{
		for (y = 0; y < b->height; y++)
		{
			acc = 0;
			
			/* this seems to be the least intricate way to iterate on all valid cells, i.e. I'm open to suggestions */
			for (xl = (x == 0 ? 0 : x - 1); xl <= (x == b->width - 1 ? x : x + 1); xl++)
			{
				for (yl = (y == 0 ? 0 : y - 1); yl <= (y == b->height - 1 ? y : y + 1); yl++)
				{
					if (x == xl && y == yl) continue;
					acc += b->board[xl][yl].content; /* slight optimization, bomb present => content == 1 */
				}
			}

			b->board[x][y].neighbours = acc;

			if (b->board[x][y].content)
			{
				mines++;
			}
		}
	}
}

void mines_set_cell(MinesBoard *b, unsigned int x, unsigned int y, MinesCell *state)
{
	b->board[x][y] = *state;
}

void mines_get_cell(MinesBoard *b, unsigned int x, unsigned int y, MinesCell *state)
{
	*state = b->board[x][y];
}

unsigned int _mines_open_cell_floodfill(MinesBoard *b, unsigned int x, unsigned int y, unsigned int count)
{
	if (b->board[x][y].state != Closed)
	{
		return count;
	}

	/* *state == closed */

	b->board[x][y].state = Open;
	count++;

	if (b->board[x][y].neighbours != 0) /* stop here if we are around a bomb */
	{
		return count;
	}

	/* process all valid neighbour cells */

	if (x > 0) /* flood fill to the left */
	{
		count = _mines_open_cell_floodfill(b, x - 1, y, count);
	}

	if (x < b->width - 1) /* flood fill to the right */
	{
		count = _mines_open_cell_floodfill(b, x + 1, y, count);
	}

	if (y > 0) /* flood fill upwards */
	{
		count = _mines_open_cell_floodfill(b, x, y - 1, count);
	}

	if (y < b->height - 1) /* flood fill downwards */
	{
		count = _mines_open_cell_floodfill(b, x, y + 1, count);
	}

	return count;
}

unsigned int mines_open_cell(MinesBoard *b, unsigned int x, unsigned int y)
{
	if (b->board[x][y].content)
	{
		b->state = Game_Lost;
		b->board[x][y].state = Open;
		return 1;
	}

	/* 
	 * this could be re-implemented with a queue (linked list? pre-allocated worst-case width*height-wide array?)
	 * recursion here should not be a problem as long as the board isn't very big, but I'd like to rethink this anyway.
	 */
	return _mines_open_cell_floodfill(b, x, y, 0);
}

void mines_replay_board(MinesBoard *b)
{
	unsigned int x, y;

	for (x = 0; x < b->width; x++)
	{
		for (y = 0; y < b->height; y++)
		{
			b->board[x][y].state = Closed;
		}
	}
}