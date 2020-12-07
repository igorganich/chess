#include "header.hpp"

int king::move(Vector2i &oldnp, Vector2i &np)
{
	first_move = false;
	if (abs(oldnp.x - np.x) == 2)
		return (MOVE_CASTLING);
	return (MOVE_DEFAULT);
}

bool king::can_reach(move_info &mv)
{
	if (first_move && mv.y == mv.l && abs(mv.x - mv.i) == 2)// Если рокировка
	{
		figure *(*board)[8][8] = mv.board;
		int plus = (mv.x < mv.i ? 1 : -1);
		if (!(*board)[mv.x + plus][mv.y] && !(*board)[mv.i][mv.y])
		{
			move_info temp;
			temp.x = mv.x + plus;
			temp.y = mv.y;
			temp.i = 7 * (mv.x < mv.i);
			temp.l = mv.y;
			temp.board = board;
			temp.mute = NULL;
			temp.unmute = NULL;
			if ((*board)[temp.i][mv.y] &&
				!(*board)[temp.i][mv.y]->is_moved() &&
				(*board)[temp.i][mv.y]->can_reach(temp))
			{
				mv.castling = true;
				return (true);
			}
		}
		return (false);
	}
	return (abs(mv.x - mv.i) < 2 && abs(mv.y - mv.l) < 2);
}

void king::calc_moves(figure* (&board)[8][8], int startx, int starty)
{
	figure::calc_moves(board, startx, starty);
	calc_one_step(board, startx, starty, -1, -1);
	calc_one_step(board, startx, starty, -1, 0);
	calc_one_step(board, startx, starty, -1, 1);
	calc_one_step(board, startx, starty, 0, 1);
	calc_one_step(board, startx, starty, 1, 1);
	calc_one_step(board, startx, starty, 1, 0);
	calc_one_step(board, startx, starty, 1, -1);
	calc_one_step(board, startx, starty, 0, -1);
}