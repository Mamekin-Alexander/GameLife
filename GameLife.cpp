#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
using namespace sf;
bool** allocate(const int w, const int h)
{
	bool** a = new bool* [h];
	for (int i = 0; i < h; i++)
		a[i] = new bool[w];
	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
			a[i][j] = false;
	return a;
}
void sync(bool** a, const int& w, const int& h)
{
	for (int i = 1; i <= h; i++)
	{
		a[i][0] = a[i][w];
		a[i][w + 1] = a[i][1];
	}
	for (int i = 1; i <= w; i++)
	{
		a[0][i] = a[h][i];
		a[h + 1][i] = a[1][i];
	}
}
void move(bool** a, const int& w, const int& h)
{
	bool** tmp = allocate(w + 2, h + 2);
	for (int i = 0; i < h + 2; i++)
		for (int j=0; j < w + 2; j++)
			tmp[i][j] = a[i][j];
	for (int i=1; i<=h; i++)
		for (int j = 1; j <= w; j++)
		{
			int count = 0;
			if (tmp[i - 1][j - 1]) count++;
			if (tmp[i - 1][j]) count++;
			if (tmp[i - 1][j + 1]) count++;
			if (tmp[i][j - 1]) count++;
			if (tmp[i][j + 1]) count++;
			if (tmp[i + 1][j - 1]) count++;
			if (tmp[i + 1][j]) count++;
			if (tmp[i + 1][j + 1]) count++;
			if (tmp[i][j] && (count < 2 || count>3))
				a[i][j] = false;
			if (!tmp[i][j] && count == 3)
				a[i][j] = true;
		}
	sync(a, w, h);

}
void Draw_field (RenderWindow& window,bool** Field, RectangleShape** draw_field, const int& width, const int& height, const int& cell_size)
{
	window.clear(Color(0, 0, 0));

	//Lines
	for (int i = 1; i < width; i++)
	{
		Vertex Line[] = { Vertex(Vector2f(i * cell_size,0)), Vertex(Vector2f(i * cell_size, cell_size*height)) };
		Line->color = Color::White;
		window.draw(Line, 2, Lines);
	}
	for (int i = 1; i < height; i++)
	{
		Vertex Line[] = { Vertex(Vector2f(0,i * cell_size)), Vertex(Vector2f(cell_size*width,i * cell_size)) };
		Line->color = Color::White;
		window.draw(Line, 2, Lines);
	}

	//Cells
	for (int i = 1; i <= height; i++)
		for (int j = 1; j <= width; j++)
			if (Field[i][j])
				window.draw(draw_field[i - 1][j - 1]);
	window.display();
}

int main()
{
	const int cell_size = 20;
	const int max_height = 1920/cell_size, max_width = 1080/cell_size;
	int width, height;

	/*Reading size of field*/
	while (true)
	{
		std::cout << "Enter width and height of field from 10 to " << max_width << " and " << max_height <<" accordingly"<< '\n';
		std::cin >> width >> height;
		if ((width<=max_width && width >=10) && (height<=max_height &&height>=10))
			break;
	}

	//Creeating window
	const int window_h = cell_size * height;
	const int window_w = cell_size * width;
	RenderWindow window(VideoMode(window_w, window_h), "Game life");
	
	//Allocating memory for field and initializing
	bool** Field = allocate(width + 2, height + 2);

	//Allocating memory for field of rectangle and initializing
	RectangleShape** draw_field = new RectangleShape* [height];
	for (int i = 0; i < height; i++)
		draw_field[i] = new RectangleShape[width];
	for (int i=0; i<height; i++)
		for (int j = 0; j < width; j++)
		{
			draw_field[i][j].setFillColor(Color(43, 255, 0));
			draw_field[i][j].setSize(Vector2f(cell_size, cell_size));
			draw_field[i][j].setPosition(Vector2f(j * cell_size, i*(cell_size)));
		}
	
	//Entering start cells
	std::cout << "Click on cells to change their status, press ENTER to start the game";
	bool game_run = false;
	while (window.isOpen() && (game_run == false))
	{
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();
				break;
			case Event::KeyPressed:
				if (event.key.code == Keyboard::Enter)
					game_run = true;
				break;
			case Event::MouseButtonPressed:
				if (event.mouseButton.button == Mouse::Button::Left)
				{
					int i = event.mouseButton.y / cell_size + 1;
					int j = event.mouseButton.x / cell_size + 1;
					Field[i][j] = !Field[i][j];
				}
				break;
			}
		}

		Draw_field(window, Field, draw_field, width, height, cell_size);
	}

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
			if (event.type==Event::Closed)
				window.close();
		
		move(Field, width, height);
		Draw_field(window, Field, draw_field, width, height, cell_size);
		Sleep(200);
	}
	return 0;
}

