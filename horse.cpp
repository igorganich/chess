#include "header.hpp"

bool horse::can_reach(move_info &mv)
{
	int a = abs(mv.x - mv.i);
	int b = abs(mv.y - mv.l);
	if (abs(a - b) == 1 && a + b == 3)
	{
		return (true);
	}
	return (false);
}

void horse::calc_moves(figure* (&board)[8][8], int startx, int starty)
{
	figure::calc_moves(board, startx, starty);
	calc_one_step(board, startx, starty, -2, -1);
	calc_one_step(board, startx, starty, -1, -2);
	calc_one_step(board, startx, starty, -2, 1);
	calc_one_step(board, startx, starty, 1, -2);
	calc_one_step(board, startx, starty, 2, 1);
	calc_one_step(board, startx, starty, 1, 2);
	calc_one_step(board, startx, starty, 2, -1);
	calc_one_step(board, startx, starty, -1, 2);
}

