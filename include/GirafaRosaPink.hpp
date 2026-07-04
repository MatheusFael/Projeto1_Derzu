#pragma once

#include "EntradaEstoque.hpp"
#include "Produto.hpp"
#include "Venda.hpp"

#include <map>
#include <memory>
#include <queue>
#include <string>
#include <vector>

class GirafaRosaPink {
public:
    GirafaRosaPink();

    void executar();

private:
    std::vector<Produto> produtos;
    std::queue<std::shared_ptr<Movimentacao>> movimentacoes;
    std::map<std::string, std::string> credenciais;
    std::string usuarioLogado;
    DataHora inicioSessao;
    std::string arquivoDados;

    void realizarLogin();
    void mostrarMenu() const;
    void inserirProduto();
    void listarProdutos() const;
    void exibirProduto();
    void alterarProduto();
    void removerProduto();
    void registrarVenda();
    void registrarEntrada();
    void relatorioGeral() const;
    void relatorioMensal() const;

    void carregarDados();
    void salvarDados() const;

    std::vector<int> pesquisarIndices(const std::string& nome) const;
    int pesquisarProduto(const std::string& nome, bool permitirSelecao = true) const;
    Produto* encontrarProdutoPorIndice(int indice);
    const Produto* encontrarProdutoPorIndice(int indice) const;

    static std::string normalizarTexto(const std::string& texto);
    static std::string lerLinha(const std::string& prompt);
    static int lerInteiroObrigatorio(const std::string& prompt);
    static double lerDoubleObrigatorio(const std::string& prompt);
    static std::string extrairCampo(const std::string& linha, std::size_t posicao);
    static std::vector<std::string> dividirLinha(const std::string& linha);
    static std::shared_ptr<Movimentacao> desserializarMovimentacao(const std::vector<std::string>& partes);
};
