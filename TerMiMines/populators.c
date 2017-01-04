/*
* Copyright 2016-2017 Riccardo Paolo Bestetti <riccardo [dt] kyogre [at] live [dt] it>
*
* This file is part of TerMiMines.
*
* TerMiMines is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* TerMiMines is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with TerMiMines.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>

#include "populators.h"
#include "random.h"

/*
 * todo: make this into a plugin system.
 * should be a hybrid between static plugins (i.e. detected during the main build & compiled
 * into the main executable) and dynamic plugins (i.e. .dll's on windows and .so's or whatever they
 * are on linux).
 * the plugin system could have additional capabilities such as different UIs, a way to record games
 * with timing (and streaming games possibly?), and all the ridiculous stuff i can come up with. :D
 */

void populators_test(MinesCell **board, unsigned int width, unsigned int height, int seed, int param)
{
	unsigned int x, y;

	/*
	 * smart compilers will probably move the switch outside the cycles for performance
	 * but I'm gonna leave it like this for readability, especially because we don't vitally need
	 * this much optimization for a test populator
	 */
	for (x = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			board[x][y].state = Closed;

			switch (param)
			{
			case 0:
				board[x][y].content = 0;
				break;
			case 1:
				board[x][y].content = 1;
				break;
			case 2:
				board[x][y].content = ((x % 2) + (y % 2)) % 2;
				break;
			case 3:
				board[x][y].content = (y % 2);
				break;
			}
		}
	}
}

void populators_pseudorandom(MinesCell **board, unsigned int width, unsigned int height, int seed, int param)
{
	unsigned int x, y;

	random_seed(seed);

	for (x = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			board[x][y].state = Closed;
			board[x][y].content = random_uint(0, 1);
		}
	}
}

/*
 * this selects a cell at random by drawing a random number in the range [0, width * height - 1]
 * and mapping it to the board. it then checks if the cell already contains a mine. if it does, it tries
 * all the consecutive cells until it finds a free one. this is done to solve two problems:
 * 1) the algorithm would place less bombs if two equal random numbers were drawn in the same run of the
 *    algorithm
 * 2) the algorithm would run for an indeterminate amount of time if we'd just keep trying to draw new random
 *    numbers until a free cell is found
 * the solution is not perfect, because it obviously does not uniformely distribute the mines but favours
 * distributing them in an horizontal fashion. a better solution would be to to generate a random number n in the
 * interval [0, width * height - 1 - mines_already_placed] and to walk cell-by-cell to the n-th free cell and
 * place the mine there. maybe another day.
 */
void populators_nummines(MinesCell **board, unsigned int width, unsigned int height, int seed, int param)
{
	unsigned int mine, x, y;
	const unsigned int max = width * height - 1;

	if ((unsigned int)param > max) /* this avoids infinite looping :) */
	{
		param = max;
	}

	random_seed(seed);

	while (param--)
	{
		mine = random_uint(0, max);

		while (1)
		{
			x = mine % width;
			y = mine / width;

			if (board[x][y].content) /* if there is a mine already */
			{
				mine++; /* try next cell */
				mine %= max;
			}
			else
			{
				board[x][y].content = 1; /* place the mine and break the cycle */
				break;
			}
		}
	}
}

