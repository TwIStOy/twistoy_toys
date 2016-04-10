#pragma once
#include <tuple>
#include <type_traits>
#include "utils.h"

namespace twistoy {

	namespace detail {
		template<typename T, typename U>
		typename std::enable_if<std::is_convertible<T, U>::value || std::is_convertible<U, T>::value, bool>::type compare(T t, U u) {
			return t == u;
		}
		bool compare(...) {
			return false;
		}

		template<int I, typename T, typename... Args>
		struct find_index {
			static int call(const std::tuple<Args...> &t, T&& val) {
				return (compare(std::get<I>(t), val) ? I : find_index<I - 1, T, Args...>::call(t, std::forward<T>(val)));
			}
		};
		template<typename T, typename... Args>
		struct find_index<0, T, Args...> {
			static int call(const std::tuple<Args...> &t, T&& val) {
				return (compare(std::get<0>(t), val) ? 0 : -1);
			}
		};

		template<int I, typename T, typename First, typename... Rest>
		struct find_type_impl :
			std::conditional<std::is_same<T, First>::value, std::integral_constant<int, I>, find_type_impl<I + 1, T, Rest...>>::type {};
		template<int I, typename T, typename First>
		struct find_type_impl<I, T, First> :
			std::conditional<std::is_same<T, First>::value, std::integral_constant<int, I>, std::integral_constant<int, -1>>::type {};

		template<int... I, typename Tuple>
		auto _reverse_impl(const Tuple& t, seq<I...>) {
			return std::make_tuple(std::get<I>(t)...);
		}

		template<int I1, int... I2, typename Tuple> 
		void _traverse_tuple_impl(const Tuple& t, seq<I1, I2...>) {
			cout << std::get<I1>(t) << endl;
			_traverse_tuple_impl(t, seq<I2...>());
		}
		template<typename Tuple>
		void _traverse_tuple_impl(const Tuple& t, seq<>) {}
	}

	// find if val in the tuple
	// if exists, return the index of val; else return -1
	template<typename T, typename... Args>
	int find_index(const std::tuple<Args...>& t, T&& val) {
		return detail::find_index<sizeof...(Args)-1, T, Args...>::call(t, std::forward<T>(val));
	}

	// find if type T in the tuple
	// if exists, value will be the index; else value will be -1
	template<typename T, typename... Args>
	struct find_type : detail::find_type_impl<0, T, Args...> {};

	// find out and return the value of type T in the tuple
	template<typename T, typename... Args>
	T find_type_value(const std::tuple<Args...>& t) {
		if (find_type<T, Args...>::value != -1) {
			return std::get<find_type<T, Args...>::value>(t);
		}
		else {
			throw invalid_argument("there's no type t in the tuple");
		}
	}

	// return a tuple which is reverse from input
	template<typename... Args>
	auto get_reverse_tuple(const std::tuple<Args...>& t) {
		return detail::_reverse_impl(t, static_range<sizeof...(Args), -1>::type());
	}

	// traverse a tuple, and output each element
	template<typename... Args>
	void traverse_tuple(const std::tuple<Args...>& t) {
		detail::_traverse_tuple_impl(t, static_range<sizeof...(Args)>::type());
	}
}