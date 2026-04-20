#ifndef FROGY_HPP
#define FROGY_HPP

#include "../main.hpp"

class frogy
{
	private :
		std::string	_Fact[42];
		std::string _Frogsave[17];
		std::string _Name;
	public :
		frogy(/* args */);
		~frogy();
		void hello(std::vector<struct pollfd> *vec);
		//void draw(std::vector<struct pollfd> *vec);
		void fact(std::vector<struct pollfd> *vec);
		void frogsave(std::vector<struct pollfd> *vec);
};

#endif