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


	template<typename T, typename... List>
	struct Contains {};
	template<typename T, typename First, typename... Rest>
	struct Contains<T, First, Rest...> :
		std::conditional<std::is_same<T, First>::value, std::true_type, Contains<T, Rest...>>::type {};
	template<typename T>
	struct Contains<T> : std::false_type {};

	template<int... Arg>
	struct seq{
		using type = seq;
	};
	namespace inc_range {
		template<typename T, typename U> struct concat;
		template<int... L1, int... L2>
		struct concat<seq<L1...>, seq<L2...>> : seq<L1..., (sizeof...(L1)+L2)...> {};

		template<int N>
		struct _inc_static_range_impl : 
			concat<typename _inc_static_range_impl<N/2>::type, typename _inc_static_range_impl<N-N/2>::type>::type {};
		template<> struct _inc_static_range_impl<0> : seq<> {};
		template<> struct _inc_static_range_impl<1> : seq<0> {};
	}

	namespace dec_range {
		template<typename T, typename U> struct concat;
		template<int... L1, int... L2>
		struct concat<seq<L1...>, seq<L2...>> : seq<(sizeof...(L2)+L1)..., L2...> {};

		template<int N>
		struct _dec_static_range_impl :
			concat<typename _dec_static_range_impl<N / 2>::type, typename _dec_static_range_impl<N - N / 2>::type>::type {};
		template<> struct _dec_static_range_impl<0> : seq<> {};
		template<> struct _dec_static_range_impl<1> : seq<0> {};
	}


	// generate a sequence from 0 to N-1 inc or dec
	// if step = 1, then inc
	// if step = -1, then dec
	template<int N, int step = 1>
	struct static_range {};
	template<int N>
	struct static_range<N, 1> : inc_range::_inc_static_range_impl<N> {};
	template<int N>
	struct static_range<N, -1> : dec_range::_dec_static_range_impl<N> {};
}