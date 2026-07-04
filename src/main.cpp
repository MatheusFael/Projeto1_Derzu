#include "GirafaRosaPink.hpp"

#include <exception>
#include <iostream>

int main()
{
    try {
        GirafaRosaPink sistema;
        sistema.executar();
    } catch (const std::exception& erro) {
        std::cerr << "Falha fatal: " << erro.what() << '\n';
        return 1;
    }

    return 0;
}
