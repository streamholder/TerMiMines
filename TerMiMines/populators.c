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

void populators_test(MinesCell **board, unsigned int width, unsigned int height, int seed)
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

			switch (seed)
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

void populators_pseudorandom(MinesCell **board, unsigned int width, unsigned int height, int seed)
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
