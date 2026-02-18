#include <iostream>

void exec()
{
    //poll;
}

int main(int ac,char **av)
{
    if (ac == 3)
    {
        /*try {
            //TEST LA CONNEXION
        }
        catch (const std::exeption& e)
        {
            std::cerr << e.what() << 'n';
        }*/
    }
    else
        std::cout << "You need 2 arguments: 'port' & 'password'\n"; return(1);
    return(0);
}