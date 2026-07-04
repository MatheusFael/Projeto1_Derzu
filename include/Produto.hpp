#pragma once

#include "DataHora.hpp"

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

class Produto {
public:
    Produto() = default;

    Produto(int indice, std::string nome, double precoUnitario, int quantidadeAtual, int quantidadeInicial, int quantidadeVendidaTotal = 0, const DataHora& ultimaAtualizacao = DataHora::agora())
        : indice(indice)
        , nome(std::move(nome))
        , precoUnitario(precoUnitario)
        , quantidadeAtual(quantidadeAtual)
        , quantidadeInicial(quantidadeInicial)
        , quantidadeVendidaTotal(quantidadeVendidaTotal)
        , ultimaAtualizacao(ultimaAtualizacao)
    {
    }

    int getIndice() const { return indice; }
    const std::string& getNome() const { return nome; }
    double getPrecoUnitario() const { return precoUnitario; }
    int getQuantidadeAtual() const { return quantidadeAtual; }
    int getQuantidadeInicial() const { return quantidadeInicial; }
    int getQuantidadeVendidaTotal() const { return quantidadeVendidaTotal; }
    const DataHora& getUltimaAtualizacao() const { return ultimaAtualizacao; }

    void setIndice(int valor) { indice = valor; }
    void setNome(const std::string& valor) { nome = valor; }
    void setPrecoUnitario(double valor) { precoUnitario = valor; }
    void setQuantidadeAtual(int valor) { quantidadeAtual = valor; }
    void setQuantidadeInicial(int valor) { quantidadeInicial = valor; }
    void setQuantidadeVendidaTotal(int valor) { quantidadeVendidaTotal = valor; }
    void setUltimaAtualizacao(const DataHora& valor) { ultimaAtualizacao = valor; }

    void registrarVenda(int quantidade)
    {
        if (quantidade <= 0) {
            throw std::runtime_error("A quantidade da venda precisa ser maior que zero.");
        }
        if (quantidade > quantidadeAtual) {
            throw std::runtime_error("Estoque insuficiente para a venda.");
        }
        quantidadeAtual -= quantidade;
        quantidadeVendidaTotal += quantidade;
        ultimaAtualizacao = DataHora::agora();
    }

    void registrarEntrada(int quantidade)
    {
        if (quantidade <= 0) {
            throw std::runtime_error("A quantidade da entrada precisa ser maior que zero.");
        }
        quantidadeAtual += quantidade;
        ultimaAtualizacao = DataHora::agora();
    }

    int limiteReposicao() const
    {
        return static_cast<int>(quantidadeInicial * 0.20);
    }

    bool estoqueBaixo() const
    {
        return quantidadeAtual <= limiteReposicao();
    }

    std::string resumoLinha() const
    {
        std::ostringstream saida;
        saida << "Indice: " << indice
              << " | Nome: " << nome
              << " | Preco: R$ " << std::fixed << std::setprecision(2) << precoUnitario
              << " | Atual: " << quantidadeAtual
              << " | Inicial: " << quantidadeInicial
              << " | Vendido: " << quantidadeVendidaTotal
              << " | Atualizado: " << ultimaAtualizacao.toTexto();
        if (estoqueBaixo()) {
            saida << " | ATENCAO: Estoque baixo";
        }
        return saida.str();
    }

private:
    int indice{0};
    std::string nome;
    double precoUnitario{0.0};
    int quantidadeAtual{0};
    int quantidadeInicial{0};
    int quantidadeVendidaTotal{0};
    DataHora ultimaAtualizacao{};
};
