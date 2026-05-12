#ifndef FROGY_HPP
#define FROGY_HPP

#include "../main.hpp"

class channel;

class frogy
{
	private :
		std::vector<std::string>	_Fact;
		std::string _Frogsave[17];
		std::string _Name;
	public :
		frogy(/* args */);
		~frogy();
		void hello(channel ch, std::vector<std::string> &content);
		//void draw(std::vector<struct pollfd> *vec);
		void fact(channel ch, std::vector<std::string> &content);
		void frogsave(channel ch, std::vector<std::string> &content);
};

#endif