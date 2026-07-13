#pragma once

// std
#include <atomic>
#include <cstdint>

// 1st-party
#include <common.hpp>

namespace Async {

/** @brief Uma trava para sincronização inter-threads em um ambiente assíncrono */
class Trava {
private:
  class Awaiter;
  inline static Awaiter *const VAZIA = nullptr;
  inline static Awaiter *const TRAVADA_SEM_NOVOS_AWAITERS = rCast<Awaiter *>(std::uintptr_t{1});
  /**
   * @brief Representa o estado da trava ao mesmo tempo que aponta para uma lista
   * que contém awaiters que serão adicionados à lista principal quando esta última
   * ficar vazia. Ela está ordenada do mais recente até o mais antigo.
   * 
   * `VAZIA`: A trava está livre, e pode ser adquirida.
   * 
   * `TRAVADA_SEM_NOVOS_AWAITERS`: A trava possui um dono, e os próximos que tentarem
   * possuí-la serão inseridos nesta fila de novos awaiters.
   * 
   * Qualquer outro valor indica que a trava está bloqueada e há pelo menos um item
   * nesta lista. Quando `_lista_awaiters` estiver vazia, esta lista será transferida
   * para `_lista_awaiters` no próximo `liberar()`, e será invertida no processo.
   */
  std::atomic<Awaiter *> _lista_novos_awaiters;
  /**
   * @brief Lista dos próximos awaiters que serão enfileirados para retomada.
   * Itens serão adicionados nesta lista a partir de `_lista_novos_awaiters`.
   * Esta lista está na ordem de mais antigo para mais recente, tornandeo esta
   * trava FIFO.
   */
  Awaiter *_lista_awaiters;
public:
  /** @brief Construtor */
  explicit Trava() noexcept;

  /** @brief Destrutor */
  ~Trava() noexcept = default;

  COLETOR_DELETE_MOVE_COPY(Trava)

  /**
   * @brief Tenta adquirir a trava. Se a corrotina caller falhar em obter a trava,
   * ela será suspensa.
   */
  Awaiter adquirir() noexcept;

  /**
   * @brief Libera a trava e enfileira a próxima corrotina que será a detentora desta
   * trava, se houver.
   */
  void liberar() noexcept;
};

}

#include "Trava/Awaiter.hpp"