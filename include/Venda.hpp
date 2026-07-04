#pragma once

#include "Movimentacao.hpp"

#include <iomanip>
#include <sstream>
#include <string>

class Venda : public Movimentacao {
public:
    using Movimentacao::Movimentacao;

    std::string tipoMovimentacao() const override
    {
        return "VENDA";
    }

    double valorMovimento() const override
    {
        return static_cast<double>(quantidade) * precoUnitario;
    }

    std::string serializar() const override
    {
        std::ostringstream saida;
        saida << "Vaca|VENDA|" << indiceProduto << '|'
              << nomeProduto << '|' << quantidade << '|' << std::fixed << std::setprecision(2) << precoUnitario << '|' << usuario << '|' << dataHora.toTexto() << "|Girafa";
        return saida.str();
    }

    std::string exibeVacaLouca() const override
    {
        std::ostringstream saida;
        saida << "Venda de " << quantidade << " unidade(s) de " << nomeProduto
              << " por R$ " << std::fixed << std::setprecision(2) << valorMovimento()
              << " em " << dataHora.toTexto();
        return saida.str();
    }
};
