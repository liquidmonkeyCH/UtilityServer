#pragma once
#ifndef __SUDOKU_HPP__
#define __SUDOKU_HPP__

#include <cstdint>
#include <assert.h>
#include <bitset>

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

namespace sudoku
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class candi
{
private:
	static constexpr uint16 VALUE_MASK = 0x03FE;
	static constexpr uint16 EXTEN_MASK = 0xFC00;
	inline bool _test(uint8 i) { return m_bits & (1 << i); }
public:
	candi(void) = default;
	~candi(void) = default;

	class iterator
	{
	public:
		iterator(void) = default;
		iterator(candi* c) : m_owner(c) {}
		iterator(const iterator& other) :m_owner(other.m_owner), m_pos(other.m_pos) {}
		iterator& operator=(const iterator& other) { m_owner = other.m_owner; m_pos = other.m_pos; return *this; }
		iterator& operator++(int) { ++m_pos; return *this; }
		iterator&& operator++() { iterator a(*this); ++m_pos; return std::move(a); }
		bool operator*() { return m_owner->test(m_pos); }
		bool operator==(iterator& other) { return m_owner == other.m_owner && m_pos == other.m_pos; }
	private:
		friend class candi;
		iterator(candi* o, uint8 i) :m_owner(o), m_pos(i) {}
	private:
		candi* m_owner = nullptr;
		uint8	m_pos = 1;
	};

	inline bool test(uint8 i) { assert(i > 0 && i < 10); return m_bits & (1 << i); }
	inline void set(uint8 i) { assert(i > 0 && i < 10); m_bits |= (1 << i); }
	inline void unset(uint8 i) { assert(i > 0 && i < 10); if (_test(i)) m_bits ^= (1 << i); }

	operator uint16(void) { return m_bits & VALUE_MASK; }
	candi& operator << (uint8 i) { set(i); return *this; }
	candi& operator >> (uint8 i) { unset(i); return *this; }
	void operator()(uint16 value) { m_bits = value & VALUE_MASK; }
	bool operator[](uint8 i) { return test(i); }

	iterator begin(void) { return { this,1 }; }
	iterator end(void) { return { this,10 }; }

	inline uint8 value(void) { return m_value; }
	candi& operator = (uint8 i) { assert(i > 0 && i < 10); m_value = i; return *this; }
	operator uint8(void) { return m_value; }
private:
	uint16	m_bits = 0;
	uint8	m_candidates[9] = {};
	uint8	m_value = 0;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class cell
{
private:
	candi m_data;
};

class region
{
	candi	m_cells[9];
	candi	m_candis[9];
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif