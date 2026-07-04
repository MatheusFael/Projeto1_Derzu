#pragma once

#include "Movimentacao.hpp"

#include <iomanip>
#include <sstream>
#include <string>

class EntradaEstoque : public Movimentacao {
public:
    using Movimentacao::Movimentacao;

    std::string tipoMovimentacao() const override
    {
        return "ENTRADA";
    }

    double valorMovimento() const override
    {
        return static_cast<double>(quantidade) * precoUnitario;
    }

    std::string serializar() const override
    {
        std::ostringstream saida;
        saida << "Vaca|ENTRADA|" << indiceProduto << '|'
              << nomeProduto << '|' << quantidade << '|' << std::fixed << std::setprecision(2) << precoUnitario << '|' << usuario << '|' << dataHora.toTexto() << "|Girafa";
        return saida.str();
    }

    std::string exibeVacaLouca() const override
    {
        std::ostringstream saida;
        saida << "Entrada de " << quantidade << " unidade(s) de " << nomeProduto
              << " em " << dataHora.toTexto();
        return saida.str();
    }
};
