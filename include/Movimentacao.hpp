#pragma once

#include "DataHora.hpp"
#include "VacaDoida.hpp"

#include <iomanip>
#include <sstream>
#include <string>
#include <utility>

class Movimentacao : public VacaDoida {
public:
    Movimentacao(int indiceProduto, std::string nomeProduto, int quantidade, double precoUnitario, std::string usuario, const DataHora& dataHora = DataHora::agora())
        : indiceProduto(indiceProduto)
        , nomeProduto(std::move(nomeProduto))
        , quantidade(quantidade)
        , precoUnitario(precoUnitario)
        , usuario(std::move(usuario))
        , dataHora(dataHora)
    {
    }

    ~Movimentacao() override = default;

    int getIndiceProduto() const { return indiceProduto; }
    const std::string& getNomeProduto() const { return nomeProduto; }
    int getQuantidade() const { return quantidade; }
    double getPrecoUnitario() const { return precoUnitario; }
    const std::string& getUsuario() const { return usuario; }
    const DataHora& getDataHora() const { return dataHora; }

    virtual std::string tipoMovimentacao() const = 0;
    virtual double valorMovimento() const = 0;
    virtual std::string serializar() const = 0;

    std::string exibeVacaLouca() const override
    {
        std::ostringstream saida;
        saida << tipoMovimentacao() << " | Produto: " << nomeProduto
              << " | Quantidade: " << quantidade
              << " | Usuario: " << usuario
              << " | Data: " << dataHora.toTexto();
        return saida.str();
    }

protected:
    int indiceProduto{0};
    std::string nomeProduto;
    int quantidade{0};
    double precoUnitario{0.0};
    std::string usuario;
    DataHora dataHora{};
};
