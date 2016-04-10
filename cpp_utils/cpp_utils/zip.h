#pragma once
#include <iterator>
#include <tuple>

namespace twistoy {
	template<typename first, typename... last>
	class _zip_iterator 
		: std::iterator<std::forward_iterator_tag, std::tuple<typename std::iterator_traits<first>::reference, 
		typename std::iterator_traits<last>::reference...>> {
	public:
		using sub_iterator = _zip_iterator<last...>;
		using rebind = _zip_iterator<first, last...>;
	private:
		first it_;
		sub_iterator sub_it_;
	public:
		_zip_iterator(cosnt first& it, const sub_iterator& sub_it) : it(it_), sub_it_(sub_it) {}

		rebind& operator++() {
			++it_;
			++sub_it_;
			return *this;
		}
		
		value_type operator *() {
			return std::tuple_cat < std::tuple<typename std::iterator_traits<first>::reference(*it_), *sub_it_);
		}

		bool operator != (const rebind& others) const {
			return (it_ != others.it_) && (sub_it_ != others.sub_it_);
		}
	};

	template<typename first>
	class _zip_iterator<first> 
		: std::iterator<std::forward_iterator_tag, std::tuple<typename std::iterator_traits<first>::reference>> {
	public:
		using rebind = zip_iterator<first>;
	private:
		first it_;
	public:
		_zip_iterator(const first& it) : it_(it) {}

		value_type operator *() {
			return value_type(*it_);
		}
		rebind& operator ++() {
			++it_;
			return *this;
		}
		bool operator != (const rebind& others) const {
			return it_ != others.it_;
		}
	};
	template<typename first, typename... last>
	class zip_impl : zip_impl<last...> {
	public:
		using iterator = _zip_iterator<typename first::iterator, typename last::iterator...>;
	private:
		first& value_;
	public:
		zip_impl(first& value, last&... args) : value_(value), zip_impl<last...>(args...) {}
		iterator begin() {
			return iterator(value_.begin(), zip_impl<last...>::begin());
		}
		iterator end() {
			return iterator(value_.end(), zip_impl<last...>::end());
		}
	};

	template<typename first>
	class zip_impl<first> {
	public:
		using iterator = zip_iterator<typename first::iterator>;
	private:
		first& value_;
	public:
		zip_impl(first& value) : value_(value) {}
		iterator begin() {
			return iterator(value_.begin());
		}
		iterator end() {
			return iterator(value_.end());
		}
	};

	template<typename... args_t>
	zip_impl<typename std::decay<args_t>::type...> zip(args_t&... args) {
		zip_impl<typename std::decay<args_t>::type...> tmp(args...);
		return tmp;
	}
}
