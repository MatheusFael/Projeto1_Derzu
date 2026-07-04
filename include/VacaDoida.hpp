#pragma once

#include <string>

class VacaDoida {
public:
    virtual ~VacaDoida() = default;

    virtual std::string exibeVacaLouca() const
    {
        return "VacaDoida";
    }
};
