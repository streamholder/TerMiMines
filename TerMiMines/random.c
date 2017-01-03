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

#include "random.h"

void random_seed(int seed)
{
	srand(seed); /* this casts signed -> unsigned but it will still keep the full range of values
				    i.e. the seed is still guaranted to have UINT_MAX possible values */
}

unsigned int random_uint(unsigned int min, unsigned int max)
{
	/*
	 * what we want to do here is to divide the RAND_MAX range into the larger possible number of
	 * (max - min + 1)-sized wells, and to keep getting random numbers from rand() until one of them
	 * falls into one of the wells. this way a truly uniform distribution is guaranteed.
	 * this is especially useful if RAND_MAX is small (it is guaranteed to only be as big as 32767).
	 * this is also definitely not useful for a poorly implemented minesweeper clone... :)
	 */

	int candidate;
	const unsigned int range = max - min + 1;
	const int wells = RAND_MAX / range; /* this integer division is the key to everything */
	const int limit = wells * range;

	while (1) /* this is more explicit than do while to the fact that we have to be careful about infinite loops */
	{
		candidate = rand();

		if (candidate < limit) /* break when we find a suitable candidate */
		{
			break;
		}
	}

	/* the division brings the candidate into the range [0, max - min], the addition translates it to [min, max] */
	return min + (candidate / wells);
}
