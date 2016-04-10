#pragma once
#include <type_traits>

namespace twistoy {

	template<int first, int... last>
	struct MaxInteger :
		std::integral_constant<int, (first > MaxInteger<last...>::value ? first : MaxInteger<last...>::value) > {};
	template<int first>
	struct MaxInteger<first> :
		std::integral_constant<int, first> {};

	constexpr bool _if_bigger(int a, int b) { return a > b; }
	template<typename T>
	struct _rebind {
		using type = T;
	};
	template<typename T>
	struct _only_value {
		static constexpr typename T::value_type value = T::value;
	};

	template<typename first, typename... last>
	struct MaxType :
		_only_value<MaxInteger<sizeof(first), sizeof(last)...>>,
		std::conditional<_if_bigger(sizeof(first), MaxInteger<sizeof(last)...>::value), _rebind<first>, MaxType<last...>>::type {};

	template<typename first>
	struct MaxType<first> :
		_rebind<first>,
		_only_value<MaxInteger<sizeof(first)>> {};


}