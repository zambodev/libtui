#include "../include/window.hpp"


Window::Window(std::string title)
{
#ifdef DEBUG
	std::cerr << "Window(): Creating window object\n";
#endif

#ifdef __linux__
	setlocale(LC_ALL, "");

	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	cols = w.ws_col;
	rows = w.ws_row;
#elif _WIN32
	_setmode(_fileno(stdout), _O_U16TEXT);

	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#endif

	buffer = (wchar_t *)malloc(cols * rows * sizeof(wchar_t));

	// Fill with blank
	for(int i = 0; i < cols * rows; ++i)
		buffer[i] = L' ';

#ifdef DEBUG
	std::cerr << "Window(): inserting new box\n";
#endif
	boxes.insert({"main", new Box(0, 0, cols-1, rows-1, title)});
#ifdef DEBUG
	std::cerr << "Widnow(): drawing 'main' box\n";
#endif
	boxes.at("main")->draw();
}

Window::~Window()
{
	/* Destroy boxes */
	std::map<std::string, Box *>::iterator itr = boxes.begin();
	if (itr != boxes.end())
	{
		delete itr->second;
		boxes.erase(itr);
	}

	/* Destory selecs */
	std::map<std::string, Selectable *>::iterator itr2 = selecs.begin();
	if (itr2 != selecs.end())
	{
		delete itr2->second;
		selecs.erase(itr2);
	}

	refresh();
}

void Window::refresh(void)
{
#ifdef DEBUG
	std::cerr << "refresh()\n";
#endif

	wprintf(L"\x1b[s\x1b[0;0H");

	for(int i = 0; i < cols * rows; ++i)
		wprintf(L"%lc", buffer[i]);

	wprintf(L"\x1b[u");
	fflush(stdout);
}

std::array<int, 2> Window::get_size()
{
#ifdef DEBUG
	std::cerr << "get_size()\n";
#endif

	return std::array<int, 2>{cols - 1, rows - 1};
}

void Window::write(int x, int y, char c)
{
#ifdef DEBUG
	std::cerr << "write(): writing into the buffer\n";
#endif

	buffer[cols * (y - 1) + (x - 1)] = c;
}

void Window::box_create(std::string id, int x1, int y1, int x2, int y2, std::string title)
{
#ifdef DEBUG
	std::cerr << "box_create(): emplacing new box\n";
#endif

	boxes.try_emplace(id, new Box(x1, y1, x2, y2, title));
}

void Window::box_delete(std::string id)
{
#ifdef DEBUG
	std::cerr << "box_delete(): deleting new box\n";
#endif

	boxes[id]->~Box();
	boxes.erase(id);
}


Window::Box * Window::get_box(std::string id)
{
#ifdef DEBUG
	std::cerr << "get_box()\n";
#endif

	return boxes.at(id);
}

void Window::selec_create(std::string id, int x, int y, bool is_row, std::vector<std::string> options, std::vector<std::function<void(void)>> funcs)
{
#ifdef DEBUG
	std::cerr << "selec_craete(): emplacing new selectable\n";
#endif

	selecs.try_emplace(id, new Selectable(x, y, is_row, options, funcs));
}

void Window::selec_delete(std::string id)
{
#ifdef DEBUG
	std::cerr << "selec_delete()\n";
#endif

	selecs.at(id)->clear();
	selecs.erase(id);
}

Window::Selectable * Window::get_selec(std::string id)
{
#ifdef DEBUG
	std::cerr << "get_selec()\n";
#endif

	try
	{
		return selecs.at(id);
	}
	catch(std::out_of_range)
	{
		return nullptr;
	}
}
