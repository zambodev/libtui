#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <iostream>
#include <cstring>
#include <functional>
#include <map>
#include <vector>
#include <array>
#include <tuple>
#include <fcntl.h>
#ifdef __linux__
#include <sys/ioctl.h>
#include <unistd.h>
#include <clocale>
#elif _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif


class Window
{
	private:
		enum : wchar_t
		{
			hbar 	= u'\u2501',
			vbar 	= u'\u2503',
			tlcrn 	= u'\u250f',
			trcrn 	= u'\u2513',
			blcrn 	= u'\u2517',
			brcrn 	= u'\u251b',
			cint  	= u'\u254b',
			lint 	= u'\u2523',
			rint 	= u'\u252b',
			tint 	= u'\u2533',
			bint 	= u'\u253B'
		};

		class Box {
			private:
				int x1, y1, x2, y2;
				wchar_t *copy = nullptr;
				std::string title;
				std::vector<std::string> text;

			public:
				Box(int, int, int, int, std::string = "");
				~Box();
				void draw(void);
				void move(int, int, int = -1, int = -1);
				void write(std::vector<std::string>);
				void clear_text(void);
				void clear(void);
		};

		class Selectable {
			private:
				struct Option
				{
					Option(std::string name, std::function<void()> func) : name(name), func(func) {}
					std::string name;
					std::function<void()> func;
				};

				int x, y;
				bool is_row;
				std::map<int, Option *> options;

			public:
				Selectable(int, int, bool, std::vector<std::string>, std::vector<std::function<void()>>);
				~Selectable();
				void draw(void);
				void clear(void);
				void select(int);
		};

		inline static int cols = -1, rows = -1;
		inline static wchar_t *buffer = nullptr;
		std::map<std::string, Box *> boxes;
		std::map<std::string, Selectable *> selecs;

	public:
		Window(std::string = "");
		~Window();
		void refresh(void);
		std::array<int, 2> get_size(void);
		void write(int x, int y, char c);

		void box_create(std::string, int, int, int, int, std::string = "");
		void box_delete(std::string);
		Box * get_box(std::string);
		
		void selec_create(std::string, int, int, bool, std::vector<std::string>, std::vector<std::function<void()>>);
		void selec_delete(std::string);
		Selectable * get_selec(std::string);
};


#endif
