#pragma once
#include <iterator>
#include <tuple>
#include <memory>

namespace twistoy {
	template<typename _T_y, typename Category>
	class _enumerate_iterator {};

	template<typename T>
	class _enumerate_iterator_base {
	public:
		int cur_;
		T it_;
		_enumerate_iterator_base(const T& it, const int& cur) : it_(it), cur_(cur) {}
	};

	// input_enumerate_iterator
	template<typename T>
	class _enumerate_iterator<T, std::input_iterator_tag> : public _enumerate_iterator_base<T> {
	public:
		using rebind = _enumerate_iterator<T, std::input_iterator_tag>;
		using value_type = std::tuple<int, typename std::iterator_traits<T>::value_type>;
		using difference_type = ptrdiff_t;
		using pointer = std::unique_ptr<value_type>;
		using reference = value_type&;
		using iterator_category = std::input_iterator_tag;

		_enumerate_iterator<T, std::input_iterator_tag>(const T& it, int cur) : _enumerate_iterator_base<T>(it, cur) {}

		value_type operator*() { return value_type(this->cur_, *(this->it_)); }
	};

	template<typename T>
	class _enumerate_iterator<T, std::forward_iterator_tag> : public _enumerate_iterator<T, std::input_iterator_tag> {
	public:
		using rebind = _enumerate_iterator<T, std::forward_iterator_tag>;
		using value_type = std::tuple<int, typename std::iterator_traits<T>::reference>;
		using pointer = std::unique_ptr<value_type>;
		using reference = value_type&;
		using iterator_category = std::forward_iterator_tag;

		_enumerate_iterator(const T& it, int cur) : _enumerate_iterator<T, std::input_iterator_tag>(it, cur) {}
	};

	template<typename T>
	class _enumerate_iterator<T, std::bidirectional_iterator_tag> : public _enumerate_iterator<T, std::forward_iterator_tag> {
	public:
		using rebind = _enumerate_iterator<T, std::bidirectional_iterator_tag>;
		using iterator_category = std::bidirectional_iterator_tag;

		_enumerate_iterator(const T& it, int cur) : _enumerate_iterator<T, std::forward_iterator_tag>(it, cur) {}
	};

	template<typename T>
	class _enumerate_iterator<T, std::random_access_iterator_tag> : public _enumerate_iterator<T, std::bidirectional_iterator_tag> {
	public:
		using rebind = _enumerate_iterator<T, std::random_access_iterator_tag>;
		using iterator_category = std::random_access_iterator_tag;

		_enumerate_iterator(const T& it, int cur) : _enumerate_iterator<T, std::bidirectional_iterator_tag>(it, cur) {}
	};

	template<typename T>
	using enumerate_iterator = _enumerate_iterator<T, typename std::iterator_traits<T>::iterator_category>;
	template<typename T>
	using input_enumerate_iterator = _enumerate_iterator<T, std::input_iterator_tag>;
	template<typename T>
	using forward_enumerate_iterator = _enumerate_iterator<T, std::forward_iterator_tag>;
	template<typename T>
	using bidirectional_enumerate_iterator = _enumerate_iterator<T, std::bidirectional_iterator_tag>;
	template<typename T>
	using random_access_enumerate_iterator = _enumerate_iterator<T, std::random_access_iterator_tag>;
	

	// operator==  [input]
	template<typename T>
	bool operator==(const input_enumerate_iterator<T>& a, const input_enumerate_iterator<T>& b) {
		return a.it_ == b.it_;
	}

	// operator!=  [input]
	template<typename T>
	bool operator!=(const input_enumerate_iterator<T>& a, const input_enumerate_iterator<T>& b) {
		return a.it_ != b.it_;
	}
	
	// operator++  [input]
	template<typename T>
	enumerate_iterator<T>& operator++(enumerate_iterator<T>& a) {
		++a.cur_;
		++a.it_;
		return a;
	}

	// operator++(void)  [input]
	template<typename T>
	enumerate_iterator<T>& operator++(enumerate_iterator<T>& a, int) {
		auto ret = a;
		++a;
		return ret;
	}

	// operator-- [bidirectional]
	template<typename T>
	enumerate_iterator<T>& operator--(enumerate_iterator<T>& a) {
		static_assert(std::is_base_of<bidirectional_enumerate_iterator<T>, enumerate_iterator<T>>::value);
		--a.cur_;
		--a.it_;
		return a;
	}

	// operator--(void) [bidirectional]
	template<typename T>
	enumerate_iterator<T>& operator--(enumerate_iterator<T>& a, int) {
		static_assert(std::is_base_of<bidirectional_enumerate_iterator<T>, enumerate_iterator<T>>::value);
		auto ret = a;
		--a;
		return ret;
	}

	// operator+(int) [random_access]
	template<typename T>
	enumerate_iterator<T> operator+ (const random_access_enumerate_iterator<T>& a, int n) {
		return enumerate_iterator<T>(a.it_ + n, a.cur_ + n);
	}
	template<typename T>
	enumerate_iterator<T> operator+ (int n, const random_access_enumerate_iterator<T>& a) {
		return enumerate_iterator<T>(a.it_ + n, a.cur_ + n);
	}

	// operator-(int) [random_access]
	template<typename T>
	enumerate_iterator<T> operator- (const random_access_enumerate_iterator<T>& a, int n) {
		return enumerate_iterator<T>(a.it_ - n, a.cur_ - n);
	}
	template<typename T>
	enumerate_iterator<T> operator- (int n, const random_access_enumerate_iterator<T>& a) {
		return enumerate_iterator<T>(a.it_ - n, a.cur_ - n);
	}

	// operator< [random_access]
	template<typename T>
	bool operator< (const random_access_enumerate_iterator<T>& a, const random_access_enumerate_iterator<T>& b) {
		return a.it_ < b.it_;
	}
	// operator<= [random_access]
	template<typename T>
	bool operator<= (const random_access_enumerate_iterator<T>& a, const random_access_enumerate_iterator<T>& b) {
		return a.it_ <= b.it_;
	}
	// operator> [random_access]
	template<typename T>
	bool operator> (const random_access_enumerate_iterator<T>& a, const random_access_enumerate_iterator<T>& b) {
		return a.it_ > b.it_;
	}
	// operator>= [random_access]
	template<typename T>
	bool operator>= (const random_access_enumerate_iterator<T>& a, const random_access_enumerate_iterator<T>& b) {
		return a.it_ > b.it_;
	}

	template<typename container>
	class enumerate_impl {
	private:
		container& val_;
		int start_;
	public:
		using iterator = enumerate_iterator<typename std::decay<container>::type::iterator>;
		enumerate_impl(container& val, int start = 0) : val_(val), start_(start) {}
		iterator begin() {
			return iterator(val_.begin(), start_);
		}
		iterator end() {
			return iterator(val_.end(), -1);
		}
	};

	template<typename T>
	enumerate_impl<T> enumerate(T& val) {
		return enumerate_impl<T>(val);
	}
}