#include "../libs/main.hpp"

frogy::frogy() : _Name("Frogy")
{
	_Frogsave[0] = "Save_Frog";
	_Frogsave[1] = "Save_Frog_ballon";
	_Frogsave[2] = "Save_Frog_barrel";
	_Frogsave[3] = "Save_Frog_butterfly";
	_Frogsave[4] = "Save_Frog_car";
	_Frogsave[5] = "Save_Frog_ghost";
	_Frogsave[6] = "Save_Frog_mole_cricket";
	_Frogsave[7] = "Save_Frog_restroom";
	_Frogsave[8] = "Save_Frog_restroom_club_titibo";
	_Frogsave[9] = "Save_Frog_sign";
	_Frogsave[10] = "Save_Frog_snake";
	_Frogsave[11] = "Save_Frog_snorkel";
	_Frogsave[12] = "Save_Frog_stuffed";
	_Frogsave[13] = "Save_Frog_umbrella";
	_Frogsave[14] = "Save_Frog_vat";
	_Frogsave[15] = "Save_Frog_wheelchair";
	_Frogsave[16] = "Save_Frog_whirloop";
	_Fact = initDataBot();
}

frogy::~frogy()
{

}

void frogy::hello(std::vector<struct pollfd> *vec)
{
	std::string str_nc = "Hye, i'm frogy the frog, I like to draw and talk about frog";
	int i = 1;
    str_nc.append("\n\033[0m");
    str_nc.insert(0, _Name);
    str_nc.insert(_Name.size(), " : \033[35m");
    for (std::vector<struct pollfd>::iterator it = vec->begin(); it != vec->end(); it++)
	{
        send((*vec)[i].fd, str_nc.c_str(), str_nc.size(), 0);
		i++;
	}
}

/*void frogy::draw(std::vector<struct pollfd> *vec)
{

}*/
//a modif pour send channel ou mp priver
void frogy::fact(std::vector<struct pollfd> *vec)
{
	int r = rand() % _Fact.size();
	std::cerr << r << " check" << std::endl;
	std::ifstream file(_Frogsave[r].c_str());
	std::cerr << "check2" << std::endl;
	std::string tmp, str_nc;
	while (std::getline(file, tmp))
		str_nc += tmp + "\n";
	std::cerr << str_nc << std::endl; //pourquoi vide ?
	int i = 1;
    for (std::vector<struct pollfd>::iterator it = vec->begin(); it != vec->end(); it++)
	{
        send((*vec)[i].fd, str_nc.c_str(), str_nc.size(), 0);
		i++;
	}
}

void frogy::frogsave(std::vector<struct pollfd> *vec)
{
	int r = rand() % 17;
	std::cerr << r << " check" << std::endl;
	std::ifstream file(_Frogsave[r].c_str());
	std::cerr << "check2" << std::endl;
	std::string tmp, str_nc;
	while (std::getline(file, tmp))
		str_nc += tmp + "\n";
	std::cerr << str_nc << std::endl; //pourquoi vide ?
	int i = 1;
    for (std::vector<struct pollfd>::iterator it = vec->begin(); it != vec->end(); it++)
	{
        send((*vec)[i].fd, str_nc.c_str(), str_nc.size(), 0);
		i++;
	}
}