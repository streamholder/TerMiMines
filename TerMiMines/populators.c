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
* along with TerMiMines.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>

#include "populators.h"

int populators_test(MinesCell *cell, unsigned int x, unsigned int y, int seed)
{
	cell->state = Closed;

	switch (seed)
	{
	case 0:
		cell->content = 0;
		break;
	case 1:
		cell->content = 1;
		break;
	case 2:
		cell->content = ((x % 2) + (y % 2)) % 2;
		break;
	case 3:
		cell->content = (y % 2);
		break;
	}

	return seed;
}

int populators_pseudorandom(MinesCell *cell, unsigned int x, unsigned int y, int seed)
{
	cell->state = Closed;
	cell->content = ((unsigned int)rand() / (RAND_MAX / 2)) % 2;
	return seed;
}
