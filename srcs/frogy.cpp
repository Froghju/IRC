#include "../libs/main.hpp"

frogy::frogy() : _Name("Frogy")
{
	_Frogsave[0] = "libs/frogy/Save_Frog";
	_Frogsave[1] = "libs/frogy/Save_Frog_ballon";
	_Frogsave[2] = "libs/frogy/Save_Frog_barrel";
	_Frogsave[3] = "libs/frogy/Save_Frog_butterfly";
	_Frogsave[4] = "libs/frogy/Save_Frog_car";
	_Frogsave[5] = "libs/frogy/Save_Frog_ghost";
	_Frogsave[6] = "libs/frogy/Save_Frog_mole_cricket";
	_Frogsave[7] = "libs/frogy/Save_Frog_restroom";
	_Frogsave[8] = "libs/frogy/Save_Frog_restroom_club_titibo";
	_Frogsave[9] = "libs/frogy/Save_Frog_sign";
	_Frogsave[10] = "libs/frogy/Save_Frog_snake";
	_Frogsave[11] = "libs/frogy/Save_Frog_snorkel";
	_Frogsave[12] = "libs/frogy/Save_Frog_stuffed";
	_Frogsave[13] = "libs/frogy/Save_Frog_umbrella";
	_Frogsave[14] = "libs/frogy/Save_Frog_vat";
	_Frogsave[15] = "libs/frogy/Save_Frog_wheelchair";
	_Frogsave[16] = "libs/frogy/Save_Frog_whirloop";
	_Fact = initDataBot();
}

frogy::~frogy()
{

}

void frogy::hello(channel ch)
{
	std::string str_nc = "Hye, i'm frogy the frog, I like to draw and talk about frog";
	ch.FrogSendToAll(str_nc);
}

void frogy::fact(channel ch)
{
	int r = rand() % _Fact.size();
	std::cerr << _Fact[r] << std::endl;
	ch.FrogSendToAll(_Fact[r]);
}

void frogy::frogsave(channel ch)
{
	int r = rand() % 17;
	//std::cerr << r << "frog check" << std::endl;
	std::ifstream file(_Frogsave[r].c_str());
	//std::cerr << "frog check2" << std::endl;
	std::string tmp, str_nc;
	while (std::getline(file, tmp))
		str_nc += tmp + "\n";
	//std::cerr << str_nc << std::endl;
	ch.FrogSendToAll(str_nc);
}