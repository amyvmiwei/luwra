#ifndef LUWRA_GENERIC_H_
#define LUWRA_GENERIC_H_

#include "common.hpp"
#include "functions.hpp"
#include "methods.hpp"
#include "fields.hpp"

LUWRA_NS_BEGIN

namespace internal {
	template <typename T>
	struct GenericWrapper {
		static_assert(
			sizeof(T) == -1,
			"Parameter to GenericWrapper is not a valid type"
		);
	};

	// Functions
	template <typename R, typename... A>
	struct GenericWrapper<R (A...)>:
		FunctionWrapper<R (A...)> {};

	template <typename R, typename... A>
	struct GenericWrapper<R (*)(A...)>:
		FunctionWrapper<R (*)(A...)> {};

	// Methods
	template <typename T, typename R, typename... A>
	struct GenericWrapper<R (T::*)(A...) const volatile>:
		MethodWrapper<R (T::*)(A...) const volatile> {};

	template <typename T, typename R, typename... A>
	struct GenericWrapper<R (T::*)(A...) const>:
		MethodWrapper<R (T::*)(A...) const> {};

	template <typename T, typename R, typename... A>
	struct GenericWrapper<R (T::*)(A...) volatile>:
		MethodWrapper<R (T::*)(A...) volatile> {};

	template <typename T, typename R, typename... A>
	struct GenericWrapper<R (T::*)(A...)>:
		MethodWrapper<R (T::*)(A...)> {};

	// Fields
	template <typename T, typename R>
	struct GenericWrapper<R T::*>: FieldWrapper<R T::*> {};

	template <typename T, typename R>
	struct GenericWrapper<const R T::*>: FieldWrapper<const R T::*> {};
}

LUWRA_NS_END

#ifdef _MSC_VER
#define __STRING( x ) #x
#endif // VC++ doesn't have __STRING

/**
 * Generate a `lua_CFunction` wrapper for a field, method or function.
 * \returns Wrapped entity as `lua_CFunction`
 */
#define LUWRA_WRAP(entity) \
	(&luwra::internal::GenericWrapper<decltype(&entity)>::template invoke<&entity>)

#ifdef _MSC_VER // Because VS C++
	#define LUWRA_MEMBER(type, name) \
		{#name, LUWRA_WRAP(##type::##name)}
#else
	#define LUWRA_MEMBER(type, name) \
		{__STRING(name), LUWRA_WRAP(type::name)}
#endif

#endif
