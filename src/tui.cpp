#include "../include/tui.hpp"


Tui * Tui::instance = nullptr;

Tui::Tui(std::string title)
{
	window = new Window("title");
}

Tui::~Tui()
{
	delete [] window;
	window = nullptr;
}

Tui * Tui::get_instance(std::string title)
{
	if(!instance)
		instance = new Tui(title);

	return instance;
}

void Tui::refresh(void)
{
	thd = new std::jthread([this]()
	{
		locked.wait(true);
		locked = true;
		
		window->refresh();
		
		locked = false;
		locked.notify_all();
	});
}

void Tui::create_box(std::string id, int x1, int y1, int x2, int y2, std::string title, std::string text)
{
	thd = new std::jthread([this, id, x1, y1, x2, y2, title, text]()
	{
		locked.wait(true);
		locked = true;
		
		window->create_box(id, x1, y1, x2, y2, title, text);
		
		locked = false;
		locked.notify_all();
	});
}

void Tui::delete_box(std::string idx)
{
	thd = new std::jthread([this, idx]()
	{
		locked.wait(true);
		locked = true;
		
		window->delete_box(idx);
		
		locked = false;
		locked.notify_all();
	});
}

void Tui::move_box(std::string id, int x1, int y1, int x2, int y2)
{
	thd = new std::jthread([this, id, x1, y1, x2, y2]()
	{
		locked.wait(true);
		locked = true;
		
		window->get_box(id)->move(x1, y1, x2, y2);
		
		locked = false;
		locked.notify_all();
	});
}

void Tui::write_box(std::string id, std::string text)
{
	thd = new std::jthread([this, id, text]()
	{
		locked.wait(true);
		locked = true;
		
		window->get_box(id)->write(text);
		
		locked = false;
		locked.notify_all();
	});
}

void Tui::clear_text_box(std::string id)
{
	thd = new std::jthread([this, id]()
	{
		locked.wait(true);
		locked = true;
		
		window->get_box(id)->clear_text();
		
		locked = false;
		locked.notify_all();
	});
}

void Tui::create_selec(std::string id, int x, int y, std::vector<std::string> options, std::vector<std::function<void(void)>> funcs)
{
	thd = new std::jthread([this, id, x, y, options, funcs]()
	{
		locked.wait(true);
		locked = true;
		
		window->create_selec(id, x, y, options, funcs);
		
		locked = false;
		locked.notify_all();
	});
}

void Tui::delete_selec(std::string id)
{
	thd = new std::jthread([this, id]()
	{
		locked.wait(true);
		locked = true;
		
		window->delete_selec(id);

		locked = false;
		locked.notify_all();
	});
}

void Tui::input_selec(std::string id)
{
	thd = new std::jthread([this, id]()
	{
		locked.wait(true);
		locked = true;

		unsigned char value;
#ifdef __linux__
		struct termios old_tio, new_tio;

		tcgetattr(STDIN_FILENO, &old_tio);
		new_tio = old_tio;
		new_tio.c_lflag &=(~ICANON & ~ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

		do
		{
			value = getchar();
		}
		while(value < '1' || value > '9');

		tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
#elif _WIN32
		do
		{
			value = getch();
		}
		while(value < '1' || value > '9');
#endif
		window->get_selec(id)->select(value - '0');

		locked = false;
		locked.notify_all();
	});
}