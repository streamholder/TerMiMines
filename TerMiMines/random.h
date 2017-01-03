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

#pragma once

/*
 * sets the seed for the prng. please note that the current implementation uses
 * stdlib's prng, so this call will change its internal state
 */
void random_seed(int seed);

/*
 * uniformely get a random unsigned integer in range [min, max].
 * this only works for max - min + 1 <= RAND_MAX i.e. it is only guaranteed to work for
 * ranges smaller than 32767. it will return max + 1 if max - min + 1 > RAND_MAX
 */
unsigned int random_uint(unsigned int min, unsigned int max);
