#pragma once

#include <optional>
#include <typeindex>
#include <cat/error.hpp>
#include <cat/config.hpp>

/*
 *    AnyPtr
 *		 - NÃO possui a propriedade do objeto
 *		 - Alternativa mais segura ao void pointer
 *		 - Possui um overhead de memória (ocupa 16 bytes, comparado com os 8 bytes de um void pointer)
 */

namespace cat
{
class AnyPtr
{
public:
	// construtores vazios
	AnyPtr() = default;
	AnyPtr(std::nullptr_t) { }

	//! @brief constrói um AnyPtr a partir de um ponteiro bruto
	template<typename T>
	AnyPtr(T* ptr)
	{
		m_src = ptr;
		m_type = typeid(T);
	}

	//! @brief construtor de cópia
	AnyPtr(const AnyPtr& other)
	: m_src(other.m_src)
	, m_type(other.m_type)
	{}

	//! @brief operador de cópia
	template<typename T>
	AnyPtr& operator=(T* ptr)
	{
		m_src = ptr;
		m_type = typeid(T);
		return *this;
	}

	AnyPtr& operator=(const AnyPtr& other)
	{
		m_src = other.m_src;
		m_type = other.m_type;
		return *this;
	}

	//! @brief verifica se um tipo é compativel com o Ponteiro
	template<typename T>
	bool check_type() const
	{
		return m_type == typeid(T);
	}

	//! @brief extrai um optional<T*> a partir do AnyPtr, caso não seja possível converter o ponteiro, retorna nullopt
	template<typename T>
	std::optional<T*> get()
	{
		if (m_type != typeid(T)) {
			return std::nullopt;
		}

		return reinterpret_cast<T*>(m_src);
	}

	//! @brief tenta extrair o ponteiro do AnyPtr, caso não seja possível retorna nullptr
	template<typename T>
	T* getv()
	{
		if (m_type != typeid(T)) {
			return nullptr;
		}

		return reinterpret_cast<T*>(m_src);
	}

	// converte um AnyPtr em um T*, lança uma excessão caso não seja possível converter
	template<typename T>
	static T* cast(AnyPtr const& p) THROWS
	{
		if (p.m_type != typeid(T))
		{
			throw Exception{eErrorCode::BAD_CAST};
		}

		return reinterpret_cast<T*>(p.m_src);
	}

	//! @brief operador booleano, retorna falso se m_src == nullptr
	operator bool() const
	{
		return m_src != nullptr;
	}

private:
	void* m_src = nullptr;
	std::type_index m_type = typeid(void);
};
}
