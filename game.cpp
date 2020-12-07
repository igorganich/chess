#include "header.hpp"

long long game::compute_hash(string const& s)
{
	const int p = 31;
	const int m = int(1e9 + 9);
	long long hash_value = 0;
	long long p_pow = 1;
	for (char c : s)
	{
		hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
		p_pow = (p_pow * p) % m;
	}
	return hash_value;
}

game::game() // init start sprites
{
	boardtxt.loadFromFile("textures/board.png");
	sBoard = Sprite(boardtxt);
	figures.loadFromFile("textures/figures.png");
	figure *newboard[8][8] =
	{
	ROOK(0),	PAWN(0), 0, 0, 0, 0, PAWN(1), ROOK(1),
	HORSE(0),	PAWN(0), 0, 0, 0, 0, PAWN(1), HORSE(1),
	BISHOP(0),	PAWN(0), 0, 0, 0, 0, PAWN(1), BISHOP(1),
	QUEEN(0),	PAWN(0), 0, 0, 0, 0, PAWN(1), QUEEN(1),
	KING(0),	PAWN(0), 0, 0, 0, 0, PAWN(1), KING(1),
	BISHOP(0),	PAWN(0), 0, 0, 0, 0, PAWN(1), BISHOP(1),
	HORSE(0),	PAWN(0), 0, 0, 0, 0, PAWN(1), HORSE(1),
	ROOK(0),	PAWN(0), 0, 0, 0, 0, PAWN(1), ROOK(1),
	};
	for (int i = 0; i < 8; i++)
	{
		for (int l = 0; l < 8; l++)
		{
			board[i][l] = newboard[i][l];
		}
	}
}

int game::beat_reachable(move_info &mv)
{
	if (IN_BOUNDS(mv.x) && IN_BOUNDS(mv.y))
	{
		mv.figmute = mv.mute ? &board[mv.mute->x][mv.mute->y] : NULL;
		mv.figunmute = mv.unmute ? &board[mv.unmute->x][mv.unmute->y] : NULL;
		if (mv.unmute && Vector2i(mv.x, mv.y) == *mv.unmute)
			return (RETURN_STOP_FAIL);
		if (!board[mv.x][mv.y])
			return (RETURN_CONTINUE);
		if (mv.mute && Vector2i(mv.x, mv.y) == *mv.mute)
			return (RETURN_CONTINUE);
		if (board[mv.x][mv.y]->get_side() == turn)
			return (RETURN_STOP_FAIL);
		if (board[mv.x][mv.y]->can_beat(mv))
			return (RETURN_STOP_SUCCESS);
		else
			return(RETURN_STOP_FAIL);
	}
	return (RETURN_CONTINUE);
}

bool game::is_check_horse(move_info &mv)
{
	for (int i = -2; i < 3; i++)
	{
		if (i == 0)
			continue;
		int l = (i % 2 == 0) ? 1 : 2;
		mv.x = mv.i - i;
		mv.y = mv.l - l;
		int ret = beat_reachable(mv);
		if (ret == RETURN_STOP_SUCCESS)
			return (true);
		l *= -1;
		mv.y = mv.l - l;
		ret = beat_reachable(mv);
		if (ret == RETURN_STOP_SUCCESS)
			return (true);
	}
	return (false);
}

bool game::is_check_horvert(move_info &mv)
{
	for (int mult = -1; mult <= 1; mult += 2)
	{
		for (int i = mv.i + 1 * mult; i < 8 && i > -1; i += 1 * mult)
		{
			mv.x = i;
			mv.y = mv.l;
			int ret = beat_reachable(mv);
			if (ret == RETURN_STOP_FAIL)
				break;
			else if (ret == RETURN_STOP_SUCCESS)
			{
				return (true);
			}
		}
		for (int i = mv.l + 1 * mult; i < 8 && i > -1; i += 1 * mult)
		{
			mv.x = mv.i;
			mv.y = i;
			int ret = beat_reachable(mv);
			if (ret == RETURN_STOP_FAIL)
				break;
			else if (ret == RETURN_STOP_SUCCESS)
				return (true);
		}
	}
	return (false);
}

bool game::is_check_diag(move_info &mv)
{
	for (int mult1 = -1; mult1 <= 1; mult1 += 2)
	{
		for (int mult2 = -1; mult2 <= 1; mult2 += 2)
		{
			for (int i = mv.i + 1 * mult1, l = mv.l + 1 * mult2;
				i < 8 && i > -1 && l < 8 && l > -1;
				i += 1 * mult1, l += 1 * mult2)
			{
				mv.x = i;
				mv.y = l;
				int ret = beat_reachable(mv);
				if (ret == RETURN_STOP_FAIL)
					break;
				else if (ret == RETURN_STOP_SUCCESS)
					return (true);
			}
		}
	}
	return (false);
}

bool game::is_check(Vector2i &kingid, move_info &mv)
{
	mv.i = kingid.x;
	mv.l = kingid.y;
	if (is_check_horse(mv))
		return (true);
	if (is_check_horvert(mv))
		return (true);
	if (is_check_diag(mv))
		return (true);
	return (false);
}

bool game::verify(Vector2i &oldnp, Vector2i &np, int prevret)
{
	if (oldnp.x == np.x && oldnp.y == np.y)// ���� ������ �� ��������
		return (false);
	if (!board[oldnp.x][oldnp.y])//������� �� ���������� �������� ������ ����� �� �� �� �������. �������� ��� �����
		return (false);
	move_info mv;
	mv.x = oldnp.x;
	mv.y = oldnp.y;
	mv.i = np.x;
	mv.l = np.y;
	mv.board = &board;
	mv.castling = false;
	mv.mute = NULL;
	mv.unmute = NULL;
	if (!(IN_BOUNDS(np.x)) || !(IN_BOUNDS(np.y)))
	{
		cout << "Wrong Move! Figure can not be out of bounds\n";
		return (false);
	}
	if (board[oldnp.x][oldnp.y]->get_side() != turn)
	{
		if (prevret != MOVE_TRANSFORM)//�� ����������� �� ����� �������� �������� ��� ���������. ������ ��������
			cout << "Wrong Move! Wrong side for move\n";
		return false;
	}
	// �������� �� ����������� "������" ��� ��������� ��������
	if (board[np.x][np.y])
	{
		if (board[np.x][np.y]->get_side() == turn)
		{
			cout << "Wrong Move! You can not beat your own figure\n";
			return (false);
		}
		else
		{
			if (!board[oldnp.x][oldnp.y]->can_beat(mv))
			{
				cout << "Wrong Move! Your figure can not beat this enemy figure\n";
				return (false);
			}
			else
				goto skip_check_reach;//���� ����� ���� ��� ������ ��������� ������� � ������������ ������ � �� ����� ������,
			//��� �� ����� � ������ �������� �� ��� ����, ���������� �������� �� ������������
		}
	}
	if (!board[oldnp.x][oldnp.y]->can_reach(mv))
	{
		//���� ������ �� ����� �������� �� ��� ���� "�� �����������", ������� 1 ������ ����� ����� �������� �� ��� ����: ����� � ������ �� �������
		if (!(pawn::is_bite_field(np.x, np.y, BITE_FIELD_GET) && board[oldnp.x][oldnp.y]->can_beat(mv)))
		{
			cout << "Wrong move! This figure can not reach this path\n";
			return (false);
		}
	}
	if (mv.castling == true)//���� ��� - ���������
	{
		Vector2i mid = Vector2i((oldnp.x + np.x) / 2, np.y);
		if (is_check(oldnp, mv) || is_check(mid, mv) || is_check(np, mv))// ��������� �� ��������� �� 3 ���� �� ������� ���������� ��������� ��� ����
		{
			cout << "Wrong move! You can not do castling through check!\n";
			return (false);
		}
		goto skip_check_check;//���� ������ �������� ����������, ������� ��������� ��������� �� ������ ��� �����
	}
skip_check_reach:
	mv.mute = &oldnp;
	mv.unmute = &np;
	if (is_check(oldnp == kings[turn] ? np : kings[turn], mv))
	{
		cout << "Wrong Move! Your king became under check\n";
		return (false);
	}
skip_check_check:
	return (true);
}

int game::move(Vector2i &oldnp, Vector2i &np, char &tform)
{
	pawn::count_moves(COUNT_INC);// �������� 1 � �������� �� ������� 50 �����
	int ret = board[oldnp.x][oldnp.y]->move(oldnp, np);//�������� ��� ����
	if (ret != FIRST_MOVE_PAWN)
		pawn::is_bite_field(-1, -1, BITE_FIELD_SET);// �������� ����� ����
	if (ret == PAWN_BEAT_BITE)//���� ��������� ������ �� �������
	{
		delete (board[np.x][oldnp.y]);
		board[np.x][oldnp.y] = NULL;
	}
	if (ret == MOVE_CASTLING)//������������ ���������
	{
		int rookx = (np.x == 6 ? 7 : 0);
		int newrookx = (np.x == 6 ? 5 : 3);
		board[rookx][np.y]->move(oldnp, np);
		board[newrookx][np.y] = board[rookx][np.y];
		board[rookx][np.y] = NULL;
		Vector2f newRookPos = Vector2f(CELL * (newrookx), CELL * (np.y));
		board[newrookx][np.y]->get_sprite()->setPosition(newRookPos);
	}
	if (board[np.x][np.y])
		pawn::count_moves(COUNT_RESET);//���� ����� ������ - �������� ������� ������� 50 �����
	delete (board[np.x][np.y]);// ����������� ������ ��������� ������ (���, ���� NULL, ����������)
	board[np.x][np.y] = board[oldnp.x][oldnp.y];
	board[oldnp.x][oldnp.y] = NULL;
	if (ret == MOVE_TRANSFORM)//���� ����� �����������
	{
		delete (board[np.x][np.y]);
		if (!tform)// ���� ���������� ����� ����
		{
			cout << "Enter figure to transformate pawn:\n";
			cout << "q - Queen, r - Rook, b - Bishop, h - horse\n";
			while (1)
			{
				cin >> tform;
				if (tform != 'q' && tform != 'r' && tform != 'b' && tform != 'h')
				{
					cout << "Wrong figure letter\n";
					continue;
				}
				break;
			}
		}
		if (tform == 'q')
			board[np.x][np.y] = QUEEN(turn);
		else if (tform == 'r')
			board[np.x][np.y] = ROOK(turn);
		else if (tform == 'b')
			board[np.x][np.y] = BISHOP(turn);
		else if (tform == 'h')
			board[np.x][np.y] = HORSE(turn);
		Vector2f newPos = Vector2f(CELL * (np.x), CELL * (np.y));
		board[np.x][np.y]->get_sprite()->setPosition(newPos);
	}
	if (oldnp == kings[turn])
		kings[turn] = np;// ���� ����� ������ - ��������� ��� ����� ���������
	turn = !turn;// ������ ����������� ����
	return (ret);
}

bool game::no_valid_move()
{
	move_info mv;
	mv.board = &board;
	Vector2i mute(0, 0);
	Vector2i unmute(0, 0);
	mv.mute = &mute;
	mv.unmute = &unmute;
	for (int i = 0; i < 8; i++)
	{
		for (int l = 0; l < 8; l++)
		{
			if (board[i][l] && board[i][l]->get_side() == turn)
			{
				mv.i = -1;
				mv.l = -1;
				mv.x = i;
				mv.y = l;
				board[i][l]->calc_moves(board, i, l);// ������� ��� ���� ���� ����� ������� ������� ������ 
				while (1)
				{
					board[i][l]->get_next_move(mv);// ������ ��������� ��� ������
					if (mv.i == -1)// ���� �� �������� ������������� ����� ������ - ��������� � ���������
						break;
					mute.x = i;
					mute.y = l;
					unmute.x = mv.i;
					unmute.y = mv.l;
					if (i == kings[turn].x && l == kings[turn].y)
					{
						if (!is_check(unmute, mv))
							return (false);// ���������� �������� ��� ������
					}
					else if (!is_check(kings[turn], mv))
						return (false); // ���������� �������� ���
				}
			}
		}
	}
	mv.mute = NULL;
	mv.unmute = NULL;
	mv.figmute = NULL;
	mv.figunmute = NULL;
	if (is_check(kings[turn], mv))// ���� ��� �������� ����� � ������ ��������� ��� ����� - ��������� ���
		cout << "Checkmate! Winner is " << (turn == 1 ? "white" : "black") << endl;
	else// ����� - ������� ��������
		cout << "Draw by stalemate!\n";
	return(true);
}

bool game::is_fifty_moves()
{
	if (pawn::count_moves(COUNT_GET) == 100)// ���� ������� ����� ��� ������ ��� ���� ����� ����� 100 - ��������� �����
	{
		cout << "Draw by 50 moves rule!\n";
		return (true);
	}
	return (false);
}

bool game::is_three_times_pos()
{
	string gamepos = "";
	gamepos += turn;// � ������� 3 ���������� ������� ����������� ����������� ����
	for (int i = 0; i < 8; i++)//�������� ���������� �� ������� ����
	{
		for (int l = 0; l < 8; l++)
		{
			if (!board[i][l])
				gamepos += "N";
			else
				gamepos += board[i][l]->get_pos_info();
		}
	}
	long long hash = compute_hash(gamepos);// ������� ��� �������
	auto pos = posmap.find(hash);
	if (pos == posmap.end())
	{
		posmap.insert(std::map<long long, int>::value_type(hash, 1));//���� ������� ����� - ������ ������ � map
	}
	else
	{
		pos->second++;
		if (pos->second == 3)//���� ������� ��� 3� ��� - ��������� �����
		{
			cout << "Draw by 3 times same pos!\n";
			return (true);
		}
	}
	return (false);
}

bool game::is_game_end()
{
	if (no_valid_move() || is_fifty_moves() || is_three_times_pos())// ����� �������� �������� �� ���������� ��������� ��� ����, �� ��� ������� ������ ������� 50 �����
		return (true);
	return false;
}