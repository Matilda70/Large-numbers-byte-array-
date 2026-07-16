#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream> 
#include <sstream>
#include <cstddef>
#include <cctype>
#include <cmath>
#include <cstdint>


typedef std::vector <uint8_t> array_bytes;



class LongNum {
	bool is_negative;
	array_bytes num_in_bytes; 
public:
	LongNum();
	LongNum(const std::string& str_num);
	LongNum(const char* str);
	LongNum(const long long& num);	
	LongNum(const LongNum& ln); 
protected:
	bool check_valid_input(const std::string& str_num);
	array_bytes get_num_bytes(const std::string& s);
	static void dec_mul_small(std::string& s, int k); 
	static void dec_add_small(std::string& s, int k);
	array_bytes for_minus(const array_bytes& a, const array_bytes& b) const ;
	array_bytes for_plus(const array_bytes& a,const array_bytes& b) const;
	
	static uint8_t get_bit(const array_bytes & a, size_t index);
	static void set_bit(array_bytes & a, size_t index, int value);
	static void add_bit(array_bytes & a, int value);
	static size_t count_important_bits(const array_bytes & a);
	static void shift_left(array_bytes &a);
    static std::string print_decimal(const LongNum& n); 
	
public:
	friend std::ostream& operator << (std::ostream& os, const LongNum& ln);
	friend std::istream& operator >>(std::istream& is, LongNum& ln); 
	void print(); 
	
	bool operator !() const; 
	LongNum& operator = (const LongNum& ln);
	LongNum& operator = (const std::string & str_num);
	LongNum& operator = (const char* str);
	bool operator != (const LongNum& ln) const; 
	bool operator == (const LongNum& ln) const;
	bool operator > (const LongNum& ln) const ;
	bool operator < (const LongNum& ln)const;
	bool operator >= (const LongNum& ln)const ;
	bool operator <= (const LongNum& ln)const;
	
	LongNum& operator +=(const LongNum& ln);
	LongNum& operator -=(const LongNum& ln); 
	LongNum& operator *=(const LongNum& ln); 
	LongNum& operator /=(const LongNum& ln); 
	LongNum operator ++(int);	
	LongNum& operator ++(); 
	LongNum operator -- (int);
	LongNum& operator -- (); 
    LongNum operator + (const LongNum& ln) const;
	LongNum operator - (const LongNum& ln) const;
	LongNum operator * (const LongNum& ln) const;
	LongNum operator / (const LongNum& ln) const ; 

	static LongNum power(const LongNum& ln, int k);
	static LongNum max(const LongNum& a, const LongNum& b) ;
    static LongNum min (const LongNum& a, const LongNum& b);
	static LongNum abs(const LongNum &ln);	

	operator long long() const; 
	
};

LongNum operator + (const LongNum& a, const char* str);
LongNum operator + (const char* str, const LongNum& a);
LongNum operator +(const LongNum& a, long long num); 
LongNum operator +(long long num, const LongNum& a); 
	
LongNum operator -(const LongNum& a, const char* str); 
LongNum operator -(const char* str, const LongNum& a);
LongNum operator -(const LongNum& a, long long num);
LongNum operator -(long long num, const LongNum& a);

LongNum operator * (const LongNum& a, long long b);
LongNum operator * (long long b, const LongNum& a); 

LongNum operator / (const LongNum& a, long long b); 
LongNum operator / (long long b, const LongNum& a); 