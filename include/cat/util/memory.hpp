#pragma once

#include <memory>

#include "thirdparty/obj_ptr.hpp"
#include "anyptr.hpp"	// IWYU pragma: export

/**
 * @module Memory
 * Define ponteiros seguros
 *    Unique
 *       - Possui a propriedade (ownership) do objeto de forma exclusiva
 *       - Desalocado assim que o escopo onde estiver declarado for encerrado
 *       - Não pode ser copiado, apenas movido
 *    Shared
 *       - Compartilha a propriedade (ownership) do objeto
 *       - Desalocado assim que o último Shared for destruído
 *    Watcher
 *       - NÃO possui a propriedade (ownership) do objeto
 *       - Pode ser criado a partir de um Unique, Shared ou raw pointer
 *       - Apenas acessa/observa o objeto, nunca o destrói
 *       - Alternativa mais segura ao raw pointer
 *    Weak
 *       - NÃO possui a propriedade (ownership) do objeto
 *       - Criado somente a partir de um Shared
 *       - Verifica a existência do objeto e recupera o Shared
 *       - Aplicação na implementação de caches
 *    AnyPtr
 *	 - NÃO possui a propriedade do objeto
 *	 - Alternativa mais segura ao void pointer
 *	 - Possui um overhead de memória (ocupa 16 bytes, comparado com os 8 bytes de um void pointer)
 */

namespace cat {

template<typename Tp>
using Unique = std::unique_ptr<Tp>;

template<typename Tp>
using Shared = std::shared_ptr<Tp>;

template<typename Tp>
using Watcher = jss::object_ptr<Tp>;

template<typename Tp>
using Weak = std::weak_ptr<Tp>;

}
