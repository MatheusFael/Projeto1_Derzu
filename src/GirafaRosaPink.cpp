#include "GirafaRosaPink.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <stdexcept>

namespace {

std::string trimTexto(const std::string& texto)
{
    const auto inicio = texto.find_first_not_of(" \t\r\n");
    if (inicio == std::string::npos) {
        return {};
    }

    const auto fim = texto.find_last_not_of(" \t\r\n");
    return texto.substr(inicio, fim - inicio + 1);
}

std::string toLowerTexto(std::string texto)
{
    std::transform(texto.begin(), texto.end(), texto.begin(), [](unsigned char caractere) {
        return static_cast<char>(std::tolower(caractere));
    });
    return texto;
}

} // namespace

GirafaRosaPink::GirafaRosaPink()
    : usuarioLogado()
    , inicioSessao(DataHora::agora())
    , arquivoDados("data/vcs_alpha.txt")
{
    credenciais.emplace("Raphael", "raphael2026");
    credenciais.emplace("Angelo", "angelo2026");
    credenciais.emplace("Thamyris", "thamyris2026");
    credenciais.emplace("Tony", "tony2026");
    credenciais.emplace("Socorro", "socorro2026");

    carregarDados();
}

void GirafaRosaPink::executar()
{
    realizarLogin();

    bool rodando = true;
    while (rodando) {
        try {
            mostrarMenu();
            const std::string opcaoTexto = lerLinha("Escolha uma opcao: ");
            if (opcaoTexto.empty()) {
                throw std::runtime_error("Opcao vazia.");
            }

            const char opcao = static_cast<char>(std::toupper(static_cast<unsigned char>(opcaoTexto.front())));
            switch (opcao) {
            case 'A':
                inserirProduto();
                break;
            case 'B':
                registrarVenda();
                break;
            case 'C':
                registrarEntrada();
                break;
            case 'D':
                listarProdutos();
                break;
            case 'E':
                exibirProduto();
                break;
            case 'F':
                alterarProduto();
                break;
            case 'G':
                removerProduto();
                break;
            case 'R':
                relatorioGeral();
                break;
            case 'M':
                relatorioMensal();
                break;
            case 'H':
                salvarDados();
                rodando = false;
                break;
            default:
                throw std::runtime_error("Opcao invalida.");
            }
        } catch (const std::exception& erro) {
            std::cerr << "[ERRO] " << erro.what() << '\n';
        }
    }
}

void GirafaRosaPink::realizarLogin()
{
    std::cout << "=== Sistema VCS-alpha ===\n";
    std::cout << "Login com usuarios predefinidos.\n";

    while (true) {
        const std::string usuario = lerLinha("Usuario: ");
        const std::string senha = lerLinha("Senha: ");

        const auto iterador = credenciais.find(usuario);
        if (iterador != credenciais.end() && iterador->second == senha) {
            usuarioLogado = usuario;
            inicioSessao = DataHora::agora();
            std::cout << "Login aprovado para " << usuarioLogado << " em " << inicioSessao.toTexto() << "\n\n";
            return;
        }

        std::cout << "Credenciais invalidas. Tente novamente.\n";
    }
}

void GirafaRosaPink::mostrarMenu() const
{
    std::cout << "\n=== MENU VCS-alpha ===\n"
              << "A - Cadastrar novo tipo de produto\n"
              << "B - Registrar venda\n"
              << "C - Registrar entrada de estoque\n"
              << "D - Listar todos\n"
              << "E - Exibir um\n"
              << "F - Alterar\n"
              << "G - Remover\n"
              << "R - Exibir relatorio geral\n"
              << "M - Relatorio mensal\n"
              << "H - Sair\n";
}

void GirafaRosaPink::inserirProduto()
{
    std::cout << "\n=== Cadastro de produto ===\n";
    const int indice = lerInteiroObrigatorio("Indice: ");
    if (encontrarProdutoPorIndice(indice) != nullptr) {
        throw std::runtime_error("Ja existe um produto com esse indice.");
    }

    const std::string nome = lerLinha("Nome: ");
    const double preco = lerDoubleObrigatorio("Preco unitario: ");
    const int quantidadeAtual = lerInteiroObrigatorio("Quantidade atual: ");
    const int quantidadeInicial = lerInteiroObrigatorio("Quantidade inicial: ");

    produtos.emplace_back(indice, nome, preco, quantidadeAtual, quantidadeInicial, 0, DataHora::agora());
    std::cout << "Produto cadastrado com sucesso.\n";
}

void GirafaRosaPink::listarProdutos() const
{
    std::cout << "\n=== Lista de produtos ===\n";
    if (produtos.empty()) {
        std::cout << "Nenhum produto cadastrado.\n";
        return;
    }

    for (const auto& produto : produtos) {
        std::cout << produto.resumoLinha() << '\n';
    }
}

void GirafaRosaPink::exibirProduto()
{
    const std::string nome = lerLinha("Pesquisar por nome: ");
    const int indice = pesquisarProduto(nome, true);
    const Produto* produto = encontrarProdutoPorIndice(indice);
    if (produto == nullptr) {
        throw std::runtime_error("Produto nao encontrado.");
    }

    std::cout << "\n=== Produto encontrado ===\n" << produto->resumoLinha() << '\n';
    if (produto->estoqueBaixo()) {
        std::cout << "Prazo para reposicao considerando o login atual: " << inicioSessao.adicionarDias(2).toTexto() << '\n';
    }
}

void GirafaRosaPink::alterarProduto()
{
    const std::string nome = lerLinha("Pesquisar por nome para alterar: ");
    const int indiceEncontrado = pesquisarProduto(nome, true);
    Produto* produto = encontrarProdutoPorIndice(indiceEncontrado);
    if (produto == nullptr) {
        throw std::runtime_error("Produto nao encontrado.");
    }

    std::cout << "Dados atuais: " << produto->resumoLinha() << '\n';

    const std::string novoIndiceTexto = lerLinha("Novo indice (enter para manter): ");
    if (!novoIndiceTexto.empty()) {
        const int novoIndice = std::stoi(novoIndiceTexto);
        if (novoIndice != produto->getIndice() && encontrarProdutoPorIndice(novoIndice) != nullptr) {
            throw std::runtime_error("Ja existe um produto com esse novo indice.");
        }
        produto->setIndice(novoIndice);
    }

    const std::string novoNome = lerLinha("Novo nome (enter para manter): ");
    if (!novoNome.empty()) {
        produto->setNome(novoNome);
    }

    const std::string novoPrecoTexto = lerLinha("Novo preco (enter para manter): ");
    if (!novoPrecoTexto.empty()) {
        produto->setPrecoUnitario(std::stod(novoPrecoTexto));
    }

    const std::string novaQuantidadeAtualTexto = lerLinha("Nova quantidade atual (enter para manter): ");
    if (!novaQuantidadeAtualTexto.empty()) {
        produto->setQuantidadeAtual(std::stoi(novaQuantidadeAtualTexto));
    }

    const std::string novaQuantidadeInicialTexto = lerLinha("Nova quantidade inicial (enter para manter): ");
    if (!novaQuantidadeInicialTexto.empty()) {
        produto->setQuantidadeInicial(std::stoi(novaQuantidadeInicialTexto));
    }

    produto->setUltimaAtualizacao(DataHora::agora());
    std::cout << "Produto alterado com sucesso.\n";
}

void GirafaRosaPink::removerProduto()
{
    const std::string nome = lerLinha("Pesquisar por nome para remover: ");
    const int indiceEncontrado = pesquisarProduto(nome, true);

    const auto iterador = std::find_if(produtos.begin(), produtos.end(), [indiceEncontrado](const Produto& produto) {
        return produto.getIndice() == indiceEncontrado;
    });

    if (iterador == produtos.end()) {
        throw std::runtime_error("Produto nao encontrado.");
    }

    std::cout << "Removendo: " << iterador->resumoLinha() << '\n';
    produtos.erase(iterador);
    std::cout << "Produto removido com sucesso.\n";
}

void GirafaRosaPink::registrarVenda()
{
    const std::string nome = lerLinha("Pesquisar produto para venda: ");
    const int indiceEncontrado = pesquisarProduto(nome, true);
    Produto* produto = encontrarProdutoPorIndice(indiceEncontrado);
    if (produto == nullptr) {
        throw std::runtime_error("Produto nao encontrado.");
    }

    const int quantidade = lerInteiroObrigatorio("Quantidade vendida: ");
    const int quantidadeAntes = produto->getQuantidadeAtual();
    produto->registrarVenda(quantidade);
    const int quantidadeDepois = produto->getQuantidadeAtual();

    auto movimento = std::make_shared<Venda>(produto->getIndice(), produto->getNome(), quantidade, produto->getPrecoUnitario(), usuarioLogado, DataHora::agora());
    movimentacoes.push(movimento);

    std::cout << "Venda registrada. Estoque antes: " << quantidadeAntes << " | depois: " << quantidadeDepois << '\n';
}

void GirafaRosaPink::registrarEntrada()
{
    const std::string nome = lerLinha("Pesquisar produto para entrada: ");
    const int indiceEncontrado = pesquisarProduto(nome, true);
    Produto* produto = encontrarProdutoPorIndice(indiceEncontrado);
    if (produto == nullptr) {
        throw std::runtime_error("Produto nao encontrado.");
    }

    const int quantidade = lerInteiroObrigatorio("Quantidade recebida: ");
    const int quantidadeAntes = produto->getQuantidadeAtual();
    produto->registrarEntrada(quantidade);
    const int quantidadeDepois = produto->getQuantidadeAtual();

    auto movimento = std::make_shared<EntradaEstoque>(produto->getIndice(), produto->getNome(), quantidade, produto->getPrecoUnitario(), usuarioLogado, DataHora::agora());
    movimentacoes.push(movimento);

    std::cout << "Entrada registrada. Estoque antes: " << quantidadeAntes << " | depois: " << quantidadeDepois << '\n';
}

void GirafaRosaPink::relatorioGeral() const
{
    std::cout << "\n=== Relatorio geral ===\n";
    std::cout << "Quantidade de Girafas armazenadas (produtos): " << produtos.size() << '\n';
    std::cout << "Quantidade de VacasLoucas armazenadas (movimentacoes): " << movimentacoes.size() << '\n';

    if (produtos.empty()) {
        std::cout << "Nao ha produtos para resumir.\n";
        return;
    }

    double valorTotalEstoque = 0.0;
    int quantidadeTotalAtual = 0;
    std::map<std::string, double> faturamentoPorMes;
    std::map<std::string, int> vendasPorProduto;
    std::map<std::string, std::vector<std::string>> datasPorProduto;
    std::queue<std::shared_ptr<Movimentacao>> copia = movimentacoes;

    while (!copia.empty()) {
        const auto movimento = copia.front();
        copia.pop();

        if (movimento->tipoMovimentacao() == "VENDA") {
            faturamentoPorMes[movimento->getDataHora().mesChave()] += movimento->valorMovimento();
            vendasPorProduto[movimento->getNomeProduto()] += movimento->getQuantidade();
            datasPorProduto[movimento->getNomeProduto()].push_back(movimento->getDataHora().toTexto());
        }
    }

    for (const auto& produto : produtos) {
        valorTotalEstoque += produto.getQuantidadeAtual() * produto.getPrecoUnitario();
        quantidadeTotalAtual += produto.getQuantidadeAtual();
        std::cout << produto.resumoLinha() << '\n';

        const auto vendaEncontrada = vendasPorProduto.find(produto.getNome());
        if (vendaEncontrada != vendasPorProduto.end()) {
            std::cout << "  Total vendido: " << vendaEncontrada->second << '\n';
            std::cout << "  Datas de venda: ";
            const auto datasEncontradas = datasPorProduto.find(produto.getNome());
            if (datasEncontradas != datasPorProduto.end()) {
                for (std::size_t indice = 0; indice < datasEncontradas->second.size(); ++indice) {
                    if (indice > 0) {
                        std::cout << ", ";
                    }
                    std::cout << datasEncontradas->second[indice];
                }
            }
            std::cout << '\n';
        }
    }

    std::cout << "\nTotal em estoque: " << quantidadeTotalAtual << '\n';
    std::cout << "Valor total do estoque: R$ " << std::fixed << std::setprecision(2) << valorTotalEstoque << '\n';

    if (!faturamentoPorMes.empty()) {
        const auto melhorMes = std::max_element(faturamentoPorMes.begin(), faturamentoPorMes.end(), [](const auto& esquerda, const auto& direita) {
            return esquerda.second < direita.second;
        });

        std::cout << "Mes de maior faturamento: " << melhorMes->first << " com R$ " << melhorMes->second << '\n';
    }

    std::cout << "Prazo geral de reposicao baseado no login atual: " << inicioSessao.adicionarDias(2).toTexto() << '\n';
}

void GirafaRosaPink::relatorioMensal() const
{
    const std::string mesAtual = DataHora::agora().mesChave();
    std::cout << "\n=== Relatorio mensal: " << mesAtual << " ===\n";

    std::map<std::string, int> vendasPorProduto;
    std::map<std::string, std::vector<std::string>> datasPorProduto;
    std::map<std::string, int> entradasPorProduto;
    double faturamentoMes = 0.0;
    int totalSaidas = 0;
    int totalEntradas = 0;

    std::queue<std::shared_ptr<Movimentacao>> copia = movimentacoes;
    while (!copia.empty()) {
        const auto movimento = copia.front();
        copia.pop();

        if (movimento->getDataHora().mesChave() != mesAtual) {
            continue;
        }

        if (movimento->tipoMovimentacao() == "VENDA") {
            vendasPorProduto[movimento->getNomeProduto()] += movimento->getQuantidade();
            datasPorProduto[movimento->getNomeProduto()].push_back(movimento->getDataHora().toTexto());
            faturamentoMes += movimento->valorMovimento();
            totalSaidas += movimento->getQuantidade();
        } else if (movimento->tipoMovimentacao() == "ENTRADA") {
            entradasPorProduto[movimento->getNomeProduto()] += movimento->getQuantidade();
            totalEntradas += movimento->getQuantidade();
        }
    }

    if (vendasPorProduto.empty() && entradasPorProduto.empty()) {
        std::cout << "Nao ha movimentacoes neste mes.\n";
        return;
    }

    std::cout << "Quantidade total de entrada: " << totalEntradas << '\n';
    std::cout << "Quantidade total de saida: " << totalSaidas << '\n';
    std::cout << "Balanço de vendas no mes: R$ " << std::fixed << std::setprecision(2) << faturamentoMes << '\n';
    std::cout << "Saldo fisico do mes (entradas - saidas): " << (totalEntradas - totalSaidas) << '\n';

    if (!vendasPorProduto.empty()) {
        std::cout << "\nVendas por produto:\n";
        for (const auto& par : vendasPorProduto) {
            std::cout << "- " << par.first << ": " << par.second << " unidade(s)\n";
            std::cout << "  Datas: ";
            const auto iterDatas = datasPorProduto.find(par.first);
            if (iterDatas != datasPorProduto.end()) {
                for (std::size_t indice = 0; indice < iterDatas->second.size(); ++indice) {
                    if (indice > 0) {
                        std::cout << ", ";
                    }
                    std::cout << iterDatas->second[indice];
                }
            }
            std::cout << '\n';
        }
    }

    if (!entradasPorProduto.empty()) {
        std::cout << "\nEntradas por produto:\n";
        for (const auto& par : entradasPorProduto) {
            std::cout << "- " << par.first << ": " << par.second << " unidade(s)\n";
        }
    }
}

void GirafaRosaPink::carregarDados()
{
    std::ifstream arquivoEntrada(arquivoDados);
    if (!arquivoEntrada.is_open()) {
        return;
    }

    produtos.clear();
    movimentacoes = std::queue<std::shared_ptr<Movimentacao>>{};

    std::string linha;
    while (std::getline(arquivoEntrada, linha)) {
        if (trimTexto(linha).empty()) {
            continue;
        }

        const std::vector<std::string> partes = dividirLinha(linha);
        if (partes.size() < 3 || partes.front() != "Vaca" || partes.back() != "Girafa") {
            continue;
        }

        const std::string tipo = partes[1];
        if (tipo == "PRODUTO") {
            if (partes.size() < 10) {
                continue;
            }
            const int indice = std::stoi(partes[2]);
            const std::string nome = partes[3];
            const double preco = std::stod(partes[4]);
            const int quantidadeAtual = std::stoi(partes[5]);
            const int quantidadeInicial = std::stoi(partes[6]);
            const int quantidadeVendidaTotal = std::stoi(partes[7]);
            const DataHora ultimaAtualizacao = DataHora::fromTexto(partes[8]);
            produtos.emplace_back(indice, nome, preco, quantidadeAtual, quantidadeInicial, quantidadeVendidaTotal, ultimaAtualizacao);
        } else if (tipo == "VENDA" || tipo == "ENTRADA") {
            if (auto movimentacao = desserializarMovimentacao(partes)) {
                movimentacoes.push(movimentacao);
            }
        }
    }
}

void GirafaRosaPink::salvarDados() const
{
    std::ofstream arquivoSaida(arquivoDados, std::ios::trunc);
    if (!arquivoSaida.is_open()) {
        throw std::runtime_error("Nao foi possivel salvar os dados.");
    }

    for (const auto& produto : produtos) {
        arquivoSaida << "Vaca|PRODUTO|" << produto.getIndice() << '|'
                     << produto.getNome() << '|' << std::fixed << std::setprecision(2) << produto.getPrecoUnitario() << '|' << produto.getQuantidadeAtual() << '|' << produto.getQuantidadeInicial() << '|' << produto.getQuantidadeVendidaTotal() << '|' << produto.getUltimaAtualizacao().toTexto() << "|Girafa\n";
    }

    std::queue<std::shared_ptr<Movimentacao>> copia = movimentacoes;
    while (!copia.empty()) {
        arquivoSaida << copia.front()->serializar() << '\n';
        copia.pop();
    }
}

std::vector<int> GirafaRosaPink::pesquisarIndices(const std::string& nome) const
{
    std::vector<int> encontrados;
    const std::string chave = normalizarTexto(nome);

    for (std::size_t indice = 0; indice < produtos.size(); ++indice) {
        const std::string nomeProduto = normalizarTexto(produtos[indice].getNome());
        if (nomeProduto.rfind(chave, 0) == 0) {
            encontrados.push_back(static_cast<int>(indice));
        }
    }

    return encontrados;
}

int GirafaRosaPink::pesquisarProduto(const std::string& nome, bool permitirSelecao) const
{
    const std::vector<int> encontrados = pesquisarIndices(nome);
    if (encontrados.empty()) {
        throw std::runtime_error("Nenhum produto encontrado para o nome informado.");
    }

    if (encontrados.size() == 1 || !permitirSelecao) {
        return encontrados.front();
    }

    std::cout << "\nForam encontrados mais de um produto:\n";
    for (int indice : encontrados) {
        const Produto* produto = encontrarProdutoPorIndice(indice);
        if (produto != nullptr) {
            std::cout << "Indice " << indice << " -> " << produto->getNome() << '\n';
        }
    }

    const int selecionado = lerInteiroObrigatorio("Digite o indice desejado: ");
    const bool valido = std::any_of(encontrados.begin(), encontrados.end(), [selecionado](int valor) {
        return valor == selecionado;
    });
    if (!valido) {
        throw std::runtime_error("Indice nao pertence aos produtos encontrados.");
    }

    return selecionado;
}

Produto* GirafaRosaPink::encontrarProdutoPorIndice(int indice)
{
    const auto iterador = std::find_if(produtos.begin(), produtos.end(), [indice](const Produto& produto) {
        return produto.getIndice() == indice;
    });
    if (iterador == produtos.end()) {
        return nullptr;
    }
    return &(*iterador);
}

const Produto* GirafaRosaPink::encontrarProdutoPorIndice(int indice) const
{
    const auto iterador = std::find_if(produtos.begin(), produtos.end(), [indice](const Produto& produto) {
        return produto.getIndice() == indice;
    });
    if (iterador == produtos.end()) {
        return nullptr;
    }
    return &(*iterador);
}

std::string GirafaRosaPink::normalizarTexto(const std::string& texto)
{
    return toLowerTexto(trimTexto(texto));
}

std::string GirafaRosaPink::lerLinha(const std::string& prompt)
{
    std::cout << prompt;
    std::string entrada;
    if (!std::getline(std::cin, entrada)) {
        throw std::runtime_error("Nao foi possivel ler a entrada do teclado.");
    }
    return trimTexto(entrada);
}

int GirafaRosaPink::lerInteiroObrigatorio(const std::string& prompt)
{
    while (true) {
        const std::string texto = lerLinha(prompt);
        try {
            std::size_t processado = 0;
            const int valor = std::stoi(texto, &processado);
            if (processado != texto.size()) {
                throw std::invalid_argument("Entrada invalida.");
            }
            return valor;
        } catch (const std::exception&) {
            std::cout << "Valor inteiro invalido. Tente novamente.\n";
        }
    }
}

double GirafaRosaPink::lerDoubleObrigatorio(const std::string& prompt)
{
    while (true) {
        const std::string texto = lerLinha(prompt);
        try {
            std::size_t processado = 0;
            const double valor = std::stod(texto, &processado);
            if (processado != texto.size()) {
                throw std::invalid_argument("Entrada invalida.");
            }
            return valor;
        } catch (const std::exception&) {
            std::cout << "Valor numerico invalido. Tente novamente.\n";
        }
    }
}

std::vector<std::string> GirafaRosaPink::dividirLinha(const std::string& linha)
{
    std::vector<std::string> partes;
    std::string parte;
    std::istringstream entrada(linha);
    while (std::getline(entrada, parte, '|')) {
        partes.push_back(parte);
    }
    return partes;
}

std::shared_ptr<Movimentacao> GirafaRosaPink::desserializarMovimentacao(const std::vector<std::string>& partes)
{
    if (partes.size() < 9) {
        return nullptr;
    }

    const int indice = std::stoi(partes[2]);
    const std::string nome = partes[3];
    const int quantidade = std::stoi(partes[4]);
    const double preco = std::stod(partes[5]);
    const std::string usuario = partes[6];
    const DataHora dataHora = DataHora::fromTexto(partes[7]);

    if (partes[1] == "VENDA") {
        return std::make_shared<Venda>(indice, nome, quantidade, preco, usuario, dataHora);
    }

    if (partes[1] == "ENTRADA") {
        return std::make_shared<EntradaEstoque>(indice, nome, quantidade, preco, usuario, dataHora);
    }

    return nullptr;
}
