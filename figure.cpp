#include "header.hpp"

int figure::get_next_counter()
{
	static int counter = 0;
	int ret = counter;
	if (counter % 8 == 1)
		counter += 5;
	else
		counter++;
	return (ret);
}

bool figure::can_reach_horvert(move_info &mv)
{
	figure* (*board)[8][8] = mv.board;
	if (mv.x == mv.i || mv.y == mv.l)
	{
		int plus = (mv.x == mv.i ? 1 : 8);
		int dyn = (mv.x == mv.i ? mv.y : mv.x);
		int dynmax = (mv.x == mv.i ? mv.l : mv.i);
		int mult = (dyn < dynmax ? 1 : -1);
		for (figure **cur = (&(*board)[mv.x][mv.y] + mult * plus);
			cur != &(*board)[mv.i][mv.l];
			cur += mult * plus)
		{
			if (mv.figmute && cur == mv.figmute)
				continue;
			if (mv.figunmute && cur == mv.figunmute)
				return (false);
			if (*cur != NULL)
				return (false);
		}
		return (true);
	}
	return (false);
}

bool figure::can_reach_diag(move_info &mv)
{
	figure* (*board)[8][8] = mv.board;
	if (abs(mv.x - mv.i) == abs(mv.y - mv.l))
	{
		int plus = (mv.x < mv.i ? 8 : -8);
		plus += (mv.y < mv.l ? 1 : -1);
		for (figure **cur = (&(*board)[mv.x][mv.y] + plus); cur != &(*board)[mv.i][mv.l]; cur += plus)
		{
			if (mv.figmute && cur == mv.figmute)
				continue;
			if (mv.figunmute && cur == mv.figunmute)
				return (false);
			if (*cur != NULL)
				return (false);
		}
		return (true);
	}
	return (false);
}

void figure::calc_way_step(figure* (&board)[8][8], const int startx, const int starty, int stepx, int stepy)
{
	for (int curx = startx + stepx, cury = starty + stepy;
		curx < 8 && curx >= 0 && cury < 8 && cury >= 0; curx += stepx, cury += stepy)
	{
		if (board[curx][cury] == NULL)
		{
			listx->push(curx);
			listy->push(cury);
			continue;
		}
		else
		{
			if (board[curx][cury]->get_side() != side)
			{
				listx->push(curx);
				listy->push(cury);
			}
			break;
		}
	}
}

void figure::calc_one_step(figure* (&board)[8][8], int startx, int starty, int plusx, int plusy)
{
	int curx = startx + plusx;
	int cury = starty + plusy;
	if (IN_BOUNDS(curx) && IN_BOUNDS(cury))
	{
		if (!board[curx][cury] || board[curx][cury]->get_side() != side)
		{
			listx->push(curx);
			listy->push(cury);
		}
	}
}

void figure::calc_moves_horvert(figure* (&board)[8][8], const int startx, const int starty)
{
	calc_way_step(board, startx, starty, -1, 0);
	calc_way_step(board, startx, starty, 1, 0);
	calc_way_step(board, startx, starty, 0, -1);
	calc_way_step(board, startx, starty, 0, 1);
}

void figure::calc_moves_diag(figure* (&board)[8][8], const int startx, const int starty)
{
	calc_way_step(board, startx, starty, -1, -1);
	calc_way_step(board, startx, starty, -1, 1);
	calc_way_step(board, startx, starty, 1, -1);
	calc_way_step(board, startx, starty, 1, 1);
}

figure::figure()
{

}

figure::figure(bool _side, Texture &txt)
{
	side = _side;
	sprite = new Sprite(txt);
	int cur_count = get_next_counter();
	int i = cur_count / 8;
	int l = cur_count % 8;
	sprite->setPosition(Vector2f(CELL * float(i), CELL * float(l)));
}

void figure::get_next_move(move_info &mv)
{
	if (listx->empty())
	{
		mv.i = -1;
		return;
	}
	mv.i = listx->top();
	mv.l = listy->top();
	listx->pop();
	listy->pop();
}