#include "header.hpp"

using namespace std;
using namespace sf;

int main()
{
	game *chess = new game();

	RenderWindow window(VideoMode(448, 448), "Test Chess", sf::Style::Close | sf::Style::Titlebar);
	ConnectToEngine("stockfish12.exe");// Создаём соединение со Стокфишем

	bool isMove = false;// Двигаем ли мы фигуру
	Sprite *temp = NULL;// Спрайт двигаемой фигуры
	float dx = 0, dy = 0;
	figure *cur = NULL;// Указатель на текущую двигаемую фигуру 
	Vector2i oldnp;// Предыдущее поле двигаемой фигуры
	Vector2f oldPos, newPos;// Координаты спрайта в окне
	int prevret = 0;// Переменная для фикса бага с превращением пешки 
	bool game_end = false;// Закончилась ли игра
	chess->is_three_times_pos();// Вызываем даный метод чтобы сохранить стартовую позицию для правила 3х повторений

	string allmoves = "";// Тут хранятся все хода

	while (window.isOpen() && !game_end)
	{
		Vector2i pos = Mouse::getPosition(window);
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();
			if (e.type == Event::MouseButtonPressed)
				if (e.key.code == Mouse::Left)
				{
					oldnp = Vector2i(int(pos.x / CELL), int(pos.y / CELL));
					if (IN_BOUNDS(oldnp.x) && IN_BOUNDS(oldnp.y))
					{
						cur = chess->get_figure(oldnp);
						if (cur)
						{
							isMove = true;
							temp = cur->get_sprite();
							dx = pos.x - temp->getPosition().x;
							dy = pos.y - temp->getPosition().y;
							oldPos = temp->getPosition();
						}
					}
				}
					
			if (e.type == Event::MouseButtonReleased)
				if (e.key.code == Mouse::Left && temp && cur)
				{
					isMove = false;
					Vector2f p = temp->getPosition() + Vector2f(CELL / 2, CELL / 2);
					Vector2i np(int(p.x / CELL), int(p.y / CELL));
					if (chess->verify(oldnp, np, prevret))// Если ход не противоречит правилам
					{
						char transpawn = '\0';
						prevret = chess->move(oldnp, np, transpawn);// Двигаем фигуры
						allmoves += 'a' + oldnp.x;
						allmoves += '0' + int(oldnp.y + 1);
						allmoves += 'a' + np.x;
						allmoves += '0' + int(np.y + 1);
						if (transpawn)
							allmoves += transpawn;// Добавляем ход в лог
						allmoves += ' ';
						newPos = Vector2f(CELL * ((float)np.x), CELL * ((float)np.y));
						temp->setPosition(newPos);
						temp = NULL;
						if (chess->is_game_end())
							game_end = true;
					}
					else
					{
						temp->setPosition(oldPos);
					}
				}

			if (isMove)
				temp->setPosition(pos.x - dx, pos.y - dy);
		}

		if (Keyboard::isKeyPressed(Keyboard::Space))// Обработка ИИ
		{
			string strret = getNextMove(allmoves);
			if (strret != "error")
			{
				oldnp.x = strret[0] - 'a';
				oldnp.y = strret[1] - '1';
				Vector2i np;
				np.x = strret[2] - 'a';
				np.y = strret[3] - '1';
				if (chess->verify(oldnp, np, prevret))
				{
					cur = chess->get_figure(oldnp);
					temp = cur->get_sprite();
					prevret = chess->move(oldnp, np, strret[4]);
					allmoves += strret + " ";
					newPos = Vector2f(CELL * ((float)np.x), CELL * ((float)np.y));
					temp->setPosition(newPos);
					temp = NULL;
					if (chess->is_game_end())
						game_end = true;
				}
			}
		}

		window.clear();
		window.draw(*chess->get_board_sprite());
		for (int i = 0; i < 8; i++)
		{
			for (int l = 0; l < 8; l++)
			{
				Vector2i draw = Vector2i(i, l);
				figure *drawcell = chess->get_figure(draw);
				if (drawcell)
				{
					window.draw(*drawcell->get_sprite());
				}
			}
		}
		window.display();
	}
	delete(chess);
	cout << "game moves:\n" << allmoves << endl;
	cin >> isMove;
	return (0);
}