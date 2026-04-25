#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector
{
public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::random_access_iterator_tag;

	private:
		pointer ptr;
		const vector<T>* parent;

	public:
		iterator(pointer p = nullptr, const vector<T>* par = nullptr) : ptr(p), parent(par) {}
		iterator(const iterator &other) : ptr(other.ptr), parent(other.parent) {}

		iterator operator+(const int &n) const
		{
			return iterator(ptr + n, parent);
		}
		iterator operator-(const int &n) const
		{
			return iterator(ptr - n, parent);
		}
		int operator-(const iterator &rhs) const
		{
			if (parent != rhs.parent) throw invalid_iterator();
			return ptr - rhs.ptr;
		}
		iterator& operator+=(const int &n)
		{
			ptr += n;
			return *this;
		}
		iterator& operator-=(const int &n)
		{
			ptr -= n;
			return *this;
		}
		iterator operator++(int) {
			iterator tmp = *this;
			ptr++;
			return tmp;
		}
		iterator& operator++() {
			ptr++;
			return *this;
		}
		iterator operator--(int) {
			iterator tmp = *this;
			ptr--;
			return tmp;
		}
		iterator& operator--() {
			ptr--;
			return *this;
		}
		T& operator*() const{
			return *ptr;
		}
		T* operator->() const {
			return ptr;
		}
		bool operator==(const iterator &rhs) const {
			return ptr == rhs.ptr;
		}
		bool operator==(const const_iterator &rhs) const {
			return ptr == rhs.ptr;
		}
		bool operator!=(const iterator &rhs) const {
			return ptr != rhs.ptr;
		}
		bool operator!=(const const_iterator &rhs) const {
			return ptr != rhs.ptr;
		}
		bool operator<(const iterator &rhs) const {
			if (parent != rhs.parent) throw invalid_iterator();
			return ptr < rhs.ptr;
		}
		bool operator>(const iterator &rhs) const {
			if (parent != rhs.parent) throw invalid_iterator();
			return ptr > rhs.ptr;
		}
		bool operator<=(const iterator &rhs) const {
			if (parent != rhs.parent) throw invalid_iterator();
			return ptr <= rhs.ptr;
		}
		bool operator>=(const iterator &rhs) const {
			if (parent != rhs.parent) throw invalid_iterator();
			return ptr >= rhs.ptr;
		}
	};

	class const_iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = const T*;
		using reference = const T&;
		using iterator_category = std::random_access_iterator_tag;

	private:
		pointer ptr;
		const vector<T>* parent;

	public:
		const_iterator(pointer p = nullptr, const vector<T>* par = nullptr) : ptr(p), parent(par) {}
		const_iterator(const const_iterator &other) : ptr(other.ptr), parent(other.parent) {}
		const_iterator(const iterator &other) : ptr(other.ptr), parent(other.parent) {}

		const_iterator operator+(const int &n) const
		{
			return const_iterator(ptr + n, parent);
		}
		const_iterator operator-(const int &n) const
		{
			return const_iterator(ptr - n, parent);
		}
		int operator-(const const_iterator &rhs) const
		{
			if (parent != rhs.parent) throw invalid_iterator();
			return ptr - rhs.ptr;
		}
		const_iterator& operator+=(const int &n)
		{
			ptr += n;
			return *this;
		}
		const_iterator& operator-=(const int &n)
		{
			ptr -= n;
			return *this;
		}
		const_iterator operator++(int) {
			const_iterator tmp = *this;
			ptr++;
			return tmp;
		}
		const_iterator& operator++() {
			ptr++;
			return *this;
		}
		const_iterator operator--(int) {
			const_iterator tmp = *this;
			ptr--;
			return tmp;
		}
		const_iterator& operator--() {
			ptr--;
			return *this;
		}
		const T& operator*() const{
			return *ptr;
		}
		const T* operator->() const {
			return ptr;
		}
		bool operator==(const iterator &rhs) const {
			return ptr == rhs.ptr;
		}
		bool operator==(const const_iterator &rhs) const {
			return ptr == rhs.ptr;
		}
		bool operator!=(const iterator &rhs) const {
			return ptr != rhs.ptr;
		}
		bool operator!=(const const_iterator &rhs) const {
			return ptr != rhs.ptr;
		}
		bool operator<(const const_iterator &rhs) const {
			if (parent != rhs.parent) throw invalid_iterator();
			return ptr < rhs.ptr;
		}
		bool operator>(const const_iterator &rhs) const {
			if (parent != rhs.parent) throw invalid_iterator();
			return ptr > rhs.ptr;
		}
		bool operator<=(const const_iterator &rhs) const {
			if (parent != rhs.parent) throw invalid_iterator();
			return ptr <= rhs.ptr;
		}
		bool operator>=(const const_iterator &rhs) const {
			if (parent != rhs.parent) throw invalid_iterator();
			return ptr >= rhs.ptr;
		}
	};

private:
	T* data;
	size_t current_size;
	size_t current_capacity;

	void copy_from(const vector &other) {
		data = (T*)(::operator new(other.current_capacity * sizeof(T)));
		current_size = other.current_size;
		current_capacity = other.current_capacity;
		for (size_t i = 0; i < current_size; ++i) {
			new (data + i) T(other.data[i]);
		}
	}

	void clear_data() {
		for (size_t i = 0; i < current_size; ++i) {
			data[i].~T();
		}
		if (data) ::operator delete(data);
		data = nullptr;
		current_size = 0;
		current_capacity = 0;
	}

	void check_and_expand() {
		if (current_size < current_capacity) return;
		size_t new_capacity = (current_capacity == 0) ? 1 : current_capacity * 2;
		T* new_data = (T*)(::operator new(new_capacity * sizeof(T)));
		for (size_t i = 0; i < current_size; ++i) {
			new (new_data + i) T(data[i]);
			data[i].~T();
		}
		if (data) ::operator delete(data);
		data = new_data;
		current_capacity = new_capacity;
	}

public:
	vector() : data(nullptr), current_size(0), current_capacity(0) {}
	vector(const vector &other) {
		copy_from(other);
	}
	~vector() {
		clear_data();
	}
	vector &operator=(const vector &other) {
		if (this == &other) return *this;
		clear_data();
		copy_from(other);
		return *this;
	}
	T & at(const size_t &pos) {
		if (pos < 0 || pos >= current_size) throw index_out_of_bound();
		return data[pos];
	}
	const T & at(const size_t &pos) const {
		if (pos < 0 || pos >= current_size) throw index_out_of_bound();
		return data[pos];
	}
	T & operator[](const size_t &pos) {
		if (pos < 0 || pos >= current_size) throw index_out_of_bound();
		return data[pos];
	}
	const T & operator[](const size_t &pos) const {
		if (pos < 0 || pos >= current_size) throw index_out_of_bound();
		return data[pos];
	}
	const T & front() const {
		if (current_size == 0) throw container_is_empty();
		return data[0];
	}
	const T & back() const {
		if (current_size == 0) throw container_is_empty();
		return data[current_size - 1];
	}
	iterator begin() {
		return iterator(data, this);
	}
	const_iterator begin() const {
		return const_iterator(data, this);
	}
	const_iterator cbegin() const {
		return const_iterator(data, this);
	}
	iterator end() {
		return iterator(data + current_size, this);
	}
	const_iterator end() const {
		return const_iterator(data + current_size, this);
	}
	const_iterator cend() const {
		return const_iterator(data + current_size, this);
	}
	bool empty() const {
		return current_size == 0;
	}
	size_t size() const {
		return current_size;
	}
	void clear() {
		for (size_t i = 0; i < current_size; ++i) {
			data[i].~T();
		}
		current_size = 0;
	}
	iterator insert(iterator pos, const T &value) {
		size_t ind = pos - begin();
		return insert(ind, value);
	}
	iterator insert(const size_t &ind, const T &value) {
		if (ind < 0 || ind > current_size) throw index_out_of_bound();
		check_and_expand();
		if (ind == current_size) {
			new (data + current_size) T(value);
		} else {
			new (data + current_size) T(data[current_size - 1]);
			for (size_t i = current_size - 1; i > ind; --i) {
				data[i] = data[i - 1];
			}
			data[ind] = value;
		}
		current_size++;
		return iterator(data + ind, this);
	}
	iterator erase(iterator pos) {
		size_t ind = pos - begin();
		return erase(ind);
	}
	iterator erase(const size_t &ind) {
		if (ind < 0 || ind >= current_size) throw index_out_of_bound();
		for (size_t i = ind; i < current_size - 1; ++i) {
			data[i] = data[i + 1];
		}
		data[current_size - 1].~T();
		current_size--;
		return iterator(data + ind, this);
	}
	void push_back(const T &value) {
		check_and_expand();
		new (data + current_size) T(value);
		current_size++;
	}
	void pop_back() {
		if (current_size == 0) throw container_is_empty();
		data[current_size - 1].~T();
		current_size--;
	}
};


}

#endif
