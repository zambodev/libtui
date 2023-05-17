#ifndef TUI_HPP
#define TUI_HPP

#include <thread>
#include <atomic>
#include "Window.hpp"


class Tui
{
	private:
		std::jthread *thd;
		std::atomic<bool> locked;
		static Tui *instance;
		Window *window;
		Tui(std::string title);
		~Tui();

	public:
		void operator=(const Tui &) = delete;
		Tui(Tui &) = delete;
		static Tui * get_instance(std::string title);

		void refresh(void);
		/* Box */
		void create_box(std::string, int, int, int, int, std::string = "", std::string = "");
		void delete_box(std::string);
		void move_box(std::string, int, int, int = -1, int = -1);
		void write_box(std::string, std::string);
		void clear_text_box(std::string);
};

#endif