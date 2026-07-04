#pragma once

#include <chrono>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

class DataHora {
public:
    DataHora()
        : valor(std::chrono::system_clock::now()) {}

    explicit DataHora(const std::chrono::system_clock::time_point& ponto)
        : valor(ponto) {}

    static DataHora agora()
    {
        return DataHora(std::chrono::system_clock::now());
    }

    static DataHora fromTexto(const std::string& texto)
    {
        std::tm tm{};
        std::istringstream entrada(texto);
        entrada >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        if (entrada.fail()) {
            throw std::runtime_error("Data invalida: use YYYY-MM-DD HH:MM:SS");
        }

        const std::time_t tempo = std::mktime(&tm);
        if (tempo == -1) {
            throw std::runtime_error("Nao foi possivel converter a data.");
        }

        return DataHora(std::chrono::system_clock::from_time_t(tempo));
    }

    std::string toTexto() const
    {
        const std::time_t tempo = std::chrono::system_clock::to_time_t(valor);
        std::tm tm{};
        if (std::tm* tempoLocal = std::localtime(&tempo)) {
            tm = *tempoLocal;
        } else {
            throw std::runtime_error("Nao foi possivel converter a data.");
        }
        char buffer[32]{};
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);
        return buffer;
    }

    std::string mesChave() const
    {
        const std::time_t tempo = std::chrono::system_clock::to_time_t(valor);
        std::tm tm{};
        if (std::tm* tempoLocal = std::localtime(&tempo)) {
            tm = *tempoLocal;
        } else {
            throw std::runtime_error("Nao foi possivel converter a data.");
        }
        char buffer[16]{};
        std::strftime(buffer, sizeof(buffer), "%Y-%m", &tm);
        return buffer;
    }

    DataHora adicionarDias(int dias) const
    {
        return DataHora(valor + std::chrono::hours(24LL * dias));
    }

    bool mesmoMesQue(const DataHora& outro) const
    {
        return mesChave() == outro.mesChave();
    }

private:
    std::chrono::system_clock::time_point valor;
};
