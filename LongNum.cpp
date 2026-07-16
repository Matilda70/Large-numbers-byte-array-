// Описывает функции для LongNum 
// порядок байтов little-endian 

#include "LongNum.h"


LongNum::LongNum() {
    is_negative = false;
    num_in_bytes = { 0 };
}
LongNum::LongNum(const std::string &str_num) {
    if (!check_valid_input(str_num)) {
        throw std::invalid_argument("Invalid argument in LongNum constructor");
    }
    is_negative = (str_num[0] == '-') ? true : false;
    num_in_bytes = get_num_bytes(str_num);
    
    
}
LongNum::LongNum(const char* str) {
    is_negative = (str[0] == '-') ? true : false;
    std :: string str_num(str); 
    if (!check_valid_input(str_num)) {
            throw std::invalid_argument("Invalid argument in LongNum constructor");
    }
	num_in_bytes = get_num_bytes(str_num);
}


LongNum::LongNum(const long long& num) {
    std::string str_num = std::to_string(num); 
    if (!check_valid_input(str_num)) {
        throw std::invalid_argument("Invalid argument in LongNum constructor");
    }
	is_negative = (str_num[0] == '-') ? true : false; 
	num_in_bytes = get_num_bytes(str_num);
}
LongNum::LongNum(const LongNum& ln) {
    this->num_in_bytes = ln.num_in_bytes;
    this->is_negative = ln.is_negative; 
}

bool LongNum::check_valid_input(const std::string& str_num) {
    int cnt = std::count_if(str_num.begin(), str_num.end(), [](char c) { return  isalpha(c);});
    int is_float = std::count_if(str_num.begin(), str_num.end(), [](char c) { return  c == '.';  });
    int minus = std::count_if(str_num.begin(), str_num.end(), [](char c) { return  c == '-';  });
    return (cnt == 0 && is_float == 0 && (minus <= 1));
}


array_bytes LongNum::get_num_bytes(const std::string &str_num) {
     array_bytes res;
     std::string s = str_num;
     if (s == "0") { 
         res.push_back(0); 
         return res;
     }
 
     int rem;
     while (!s.empty() && s[0] == ' ') s.erase(s.begin());
     if (!s.empty() && s[0] == '-') s = s.substr(1);
     
        if (s == "0") {
			res.push_back(0); 
            return res; 
        }
        while (s != "0") {
            rem = 0;
            std::string q;
            bool is_quotient_started = false;
            for (char c : s) {
                int digit = c - '0';
                int cur = rem * 10 + digit;
                int q_digit = cur / 256;
                rem = cur % 256;
                if (q_digit != 0 || is_quotient_started) {
                    q.push_back(q_digit + '0');
                    is_quotient_started = true;
                }
            }
            if (!is_quotient_started) q = "0";
            res.push_back(static_cast<uint8_t>(rem));
            s = q;   
        }
        return res;   
    }



 void LongNum::print() {
        for (uint8_t i : num_in_bytes) {
            std::cout << (int)i << " ";
        }
    }   

// dec = dec * k  (k <= 10^9)
// Прибавляет к десятичному числу в строке s маленькое целое k (0 <= k < 256).
// s содержит только символы '0'..'9'.
 void LongNum::dec_add_small(std::string& s, int k) {
     if (k == 0) {
         return;
     }
     int i = static_cast<int>(s.size()) - 1;
     int carry = k;
     while (i >= 0 && carry > 0) {
         int digit = s[i] - '0';
         int add = carry % 10;
         carry /= 10;

         int sum = digit + add;
         if (sum >= 10) {
             sum -= 10;
             ++carry;  
         }

         s[i] = static_cast<char>('0' + sum);
         --i;
     }
     while (carry > 0) {
         int new_digit = carry % 10;
         carry /= 10;
         s.insert(s.begin(), static_cast<char>('0' + new_digit));
     }
 }
    

// Умножает десятичное число в строке s на маленькое целое k (k >= 0).
// s содержит только символы '0'..'9'.
 void LongNum::dec_mul_small(std::string& s, int k) {
     if (k == 0 || s == "0") {
         s = "0";
         return;
     }
     int carry = 0;
     for (int i = static_cast<int>(s.size()) - 1; i >= 0; --i) {
         int digit = s[i] - '0';
         long long product = 1LL * digit * k + carry;

         int new_digit = static_cast<int>(product % 10);
         carry = static_cast<int>(product / 10);

         s[i] = static_cast<char>('0' + new_digit);
     }
     while (carry > 0) {
         int new_digit = carry % 10;
         carry /= 10;
         s.insert(s.begin(), static_cast<char>('0' + new_digit));
     }
 }


 std::string  LongNum::print_decimal(const LongNum& n) {
     std::string s = "0";
     std::string res = "";
     if (n.is_negative) res += "-";
     for (auto it = n.num_in_bytes.rbegin(); it != n.num_in_bytes.rend(); ++it) {
         dec_mul_small(s, 256);
         dec_add_small(s, (int)*it);
     }
     size_t p = s.find_first_not_of('0');

     ((p == std::string::npos) ? "0" : s.substr(p));
     res += s;
     return res;
 }


    bool LongNum:: operator !()const {
        return num_in_bytes.size() == 1 && num_in_bytes[0] == 0;
    }

    bool LongNum::operator == (const LongNum& ln)const {
        if (is_negative != ln.is_negative) return false;
        if (num_in_bytes.size() != ln.num_in_bytes.size()) return false;
        int cnt = 0;
        for (int i = 0; i < num_in_bytes.size(); i++) {
            if (num_in_bytes[i] == ln.num_in_bytes[i]) ++cnt;
        }
        return cnt == num_in_bytes.size();
    }

    bool LongNum::operator != (const LongNum& ln)const {
          return !(*this == ln);
    }


    bool  LongNum::operator < (const LongNum& ln) const {
        int this_size = this->num_in_bytes.size();
        int ln_size = ln.num_in_bytes.size();
        if (this_size == ln_size && this->is_negative == ln.is_negative) {
            if (this->is_negative == false && ln.is_negative == false) {
                for (int i = this_size - 1; i >= 0; --i) {
                    if (this->num_in_bytes[i] == ln.num_in_bytes[i]) continue;
                    else return this->num_in_bytes[i] < ln.num_in_bytes[i];
                }
            }
            if (this->is_negative == true && ln.is_negative == true) {
                for (int i = this_size - 1; i >= 0; --i) {
                    if (this->num_in_bytes[i] == ln.num_in_bytes[i]) continue;
                    else return this -> num_in_bytes[i] > ln.num_in_bytes[i];
                }
            }
        }
        else if (this->is_negative != ln.is_negative) {
            return is_negative;
        }
        else if (this_size != ln_size) {
            if (this->is_negative == false && ln.is_negative == false) {
                return this_size < ln_size;
            }
            if (this->is_negative == true && ln.is_negative == true) {
                return this_size > ln_size;
            }
        }
        return false; 
    }


    bool LongNum::operator > (const LongNum& ln) const {      
        return ln < *this; 
    }
    bool  LongNum::operator >= (const LongNum& ln) const{
		return !(*this < ln);
    }
    bool  LongNum::operator <= (const LongNum& ln) const{
        return !(ln < *this); 
        
    }


    LongNum& LongNum::operator = (const LongNum& ln) {
        if (this == &ln) return *this;
        this->num_in_bytes = ln.num_in_bytes; 
        this -> is_negative = ln.is_negative;
		return *this;
    }

    LongNum& LongNum::operator = (const std::string& str_num) {
        num_in_bytes = get_num_bytes(str_num);
		int cnt = std::count_if(str_num.begin(), str_num.end(), [](char c) { return  c == '-';  });
        is_negative = (cnt > 0) ? true : false;
        return *this; 
    }

    LongNum& LongNum::operator = (const char* str) {
        std::string str_num (str);
        int cnt = std::count_if(str_num.begin(), str_num.end(), [](char c) { return  c == '-';  });
        is_negative = (cnt > 0) ? true : false;
		num_in_bytes = get_num_bytes(str_num);
        return *this; 
    }
    LongNum LongNum::max(const LongNum & a, const LongNum& b) {
        if (a >= b) return a;
        else return b; 
       
    }

    LongNum  LongNum::min(const LongNum & a , const LongNum& b) {
        if (a <= b) return a;
        else return  b; 
  
    }

    // предполагатся что a >= b. Обязательно думать перед вызоовом , что больше
    array_bytes LongNum::for_minus(const array_bytes& a, const array_bytes& b)const {
        int max_size = std::max(a.size(), b.size());
        int min_size = std::min(a.size(), b.size()); 
        array_bytes res;
        res.resize(max_size, 0);   
        int carry = 0; 
        for (int i = 0; i < min_size; i++) {
            int dif = (int)a[i] - (int)b[i] - carry; 
            if (dif < 0) {
                carry = 1; 
                res[i] = static_cast<uint8_t>(256 + dif); 

            }
            else {
                carry = 0; 
                res[i] = static_cast<uint8_t>(dif); 
            }
        }
        for (int i = min_size; i < max_size; i++) {
            int dif = (int)a[i] - carry; 
            if (dif < 0) {
                carry = 1; 
				res[i] = static_cast<uint8_t>(256 + dif);
            }
            else {
                carry = 0; 
                res[i] = static_cast<uint8_t>(dif); 
            }
        }
        return res; 
    }

    array_bytes LongNum::for_plus(const array_bytes& a,const array_bytes& b)const {
         int max_size = std::max(a.size(), b.size()); 
         int min_size = std::min(a.size(), b.size());  
         array_bytes res;
         res.resize(max_size + 1, 0); 
         int carry = 0; 
         for (int i = 0; i < min_size; i++) {
             int sum = (int)a[i] + (int)b[i] + carry; 
             if (sum / 256 > 0) {
                 carry = sum / 256; 
				 res[i] = static_cast<uint8_t>(sum % 256); 
             }
             else {
                 carry = 0; 
				 res[i] = static_cast<uint8_t>(sum);
             }
            
         }
         if (a.size() == max_size) {
             for (int i = min_size; i < max_size; i++) {
                 int sum = (int)a[i] + carry;
                 if (sum / 256 > 0) {
                     carry = sum / 256;
                     res[i] = static_cast<uint8_t>(sum % 256);
                 }
                 else {
                     carry = 0;
					 res[i] = static_cast<uint8_t>(sum);
                 }

             }
         }
         else if (b.size() == max_size) {
             for (int i = min_size; i < max_size; i++) {
                 int sum = (int)b[i] + carry;
                 if (sum / 256 > 0) {
                     carry = sum / 256;
                     res[i] = static_cast<uint8_t>(sum % 256);
                 }
                 else {
                     carry = 0;
                     res[i] = static_cast<uint8_t>(sum);
                 }

             }
         }
         if (carry > 0) {
			 res.push_back(static_cast<uint8_t>(carry));
         }
         else {
             res.pop_back(); 
         }
         return res; 
          
   }

    LongNum LongNum :: abs(const LongNum& ln) {
        if (ln.is_negative == false) return ln;
        LongNum res = ln;
		res.is_negative = false;
		return res;
    }
   
    

    LongNum  LongNum::operator + (const LongNum& ln) const {
        LongNum res("0");
        int max_size = std::max(num_in_bytes.size(), ln.num_in_bytes.size());
        int min_size = std::min(num_in_bytes.size(), ln.num_in_bytes.size());
        if (is_negative == ln.is_negative) { // одинаковые знаки 
            res.is_negative = ln.is_negative;
            res.num_in_bytes = for_plus(num_in_bytes, ln.num_in_bytes);
            return res;
        }
    else if (is_negative == false && ln.is_negative == true) {
            int carry = 0; 
            LongNum abs_ln = abs(ln);
			res.num_in_bytes.resize(max_size, 0);
            
            if (*this < abs_ln) {  // 5 + (-7) = -(7 - 5) = -2   
                res.is_negative = true;
                res.num_in_bytes =for_minus(abs_ln.num_in_bytes, num_in_bytes);      
                return res;
            }
            else if (*this > abs_ln) { // 5 + (-1) = (5 - 1) = 4
                res.is_negative = false;
                res.num_in_bytes.resize(max_size, 0);
                res.num_in_bytes = for_minus(num_in_bytes, abs_ln.num_in_bytes);    
                return res;
            }
            else return res;
        }
        else if (is_negative == true && ln.is_negative == false) { // (-1000) + (1500) = -(7 - 5) = -2
            int carry = 0; 
            res.num_in_bytes.resize(max_size, 0); 
            LongNum abs_this = abs(*this); 

            if (abs_this < ln) {
                res.is_negative = false; 
                res.num_in_bytes = for_minus(ln.num_in_bytes, abs_this.num_in_bytes);
                return res; 
            }
            else if (abs_this > ln) // (-1500) + (1000) = -(1500 - 1000) = -500
            {
                res.is_negative = true;
                res.num_in_bytes.resize(max_size, 0);
                res.num_in_bytes = for_minus(abs_this.num_in_bytes, ln.num_in_bytes);
                return res; 
            }
            else  return res;    
        }
         
    }

    LongNum& LongNum::operator +=(const LongNum& ln) {
        *this = *this + ln; 
        return* this; 
    }


    LongNum  LongNum::operator - (const LongNum& ln)const {
        LongNum res("0");
        int max_size = std::max(num_in_bytes.size(), ln.num_in_bytes.size()); 
        int min_size = std::min(num_in_bytes.size(), ln.num_in_bytes.size()); 
        res.num_in_bytes.resize(max_size, 0);
        if (is_negative == false && ln.is_negative == false) // a - b 
        {
            if (*this >= ln) {    // a >= b , просто вычитаем большее из меньшего
                res.is_negative = false;
                res.num_in_bytes = for_minus(num_in_bytes, ln.num_in_bytes);
                return res;
            }
            else {  // a < b => -a - b =  -(b - a)
                res.is_negative = true;
                res.num_in_bytes = for_minus(ln.num_in_bytes, num_in_bytes);
                return res;
            }
        }
        else if (is_negative == true && ln.is_negative == false) { // -a - b =  -(a + b)
            res.is_negative = true; 
            res.num_in_bytes = for_plus(num_in_bytes, ln.num_in_bytes); 
            return res; 
        }
        else if (is_negative == false && ln.is_negative == true) { // a - (-b)            
            res.is_negative = false; 
            res.num_in_bytes = for_plus(num_in_bytes, ln.num_in_bytes); 
            return res; 
        }
        else  { //(-a)-(-b) = -a + b = b - a
            LongNum abs_this = abs(*this);
            LongNum abs_ln = abs(ln); 
            if (abs_this > abs_ln) {
                res.is_negative = true;
                res.num_in_bytes = for_minus(abs_this.num_in_bytes, abs_ln.num_in_bytes);
                return res; 

            }
            else  {
                res.is_negative = false; 
                res.num_in_bytes = for_minus(abs_ln.num_in_bytes, abs_this.num_in_bytes); 
                return res; 
            }
        } 
        return res; 
    }
    LongNum& LongNum::operator -=(const LongNum& ln) {
        *this = *this - ln;
        return *this; 
    }
    
        
    LongNum LongNum::operator* (const LongNum& ln) const {
        LongNum res("0");
        if (is_negative == ln.is_negative) res.is_negative = false;
        else res.is_negative = true;
        int min_size = std::min(num_in_bytes.size(), ln.num_in_bytes.size());
        int max_size = std::max(num_in_bytes.size(), ln.num_in_bytes.size());
        res.num_in_bytes.resize(max_size + min_size, 0);
        array_bytes min;
        array_bytes max;
        if (abs(*this) >= abs(ln)) {
            max = num_in_bytes; 
			min = ln.num_in_bytes; 
        }
        else {
            max = ln.num_in_bytes;
            min = num_in_bytes;
        }
        for (int i = 0; i < min_size; i++) {
            int carry = 0;
            for (int j = 0; j < max_size; j++) {
                int product = (int)min[i] * (int)max[j] + (int)res.num_in_bytes[i + j] + carry;
                res.num_in_bytes[i + j] = product % 256;
                carry = product / 256;
            }
            res.num_in_bytes[i + max_size] = carry;
        }
        return res;
    }

    // виталий владимирович объяснял мне вот так
    
    // возращаем ссылку на старый объект , но меняем его
    LongNum LongNum :: operator --(int) {
		LongNum res = *this; 
        if (res == LongNum("0")) {
            res = LongNum("1");
            res.is_negative = true;
        }
        res.num_in_bytes[0] = --res.num_in_bytes[0];
        return res;  
    }
    // ссылка , потому как мы меняем сам объект и он живет дальше
    LongNum& LongNum:: operator --() {
        if (*this == LongNum("0")) {
            *this = LongNum("1"); 
            this->is_negative = true; 
        }
        this->num_in_bytes[0] = --this->num_in_bytes[0]; 
        return *this; 
    }

    LongNum LongNum :: operator ++(int) {
        LongNum res = *this;
        res.num_in_bytes[0] = ++res.num_in_bytes[0]; 
        return res; 
    }
    LongNum& LongNum :: operator ++() {
        this->num_in_bytes[0] = ++this->num_in_bytes[0];
        return *this; 
    }
    
   
    
    
    uint8_t LongNum::get_bit(const array_bytes& a, size_t index) {
        int byte_index = index / 8; 
        if (byte_index >= a.size()) return 0;
        uint8_t byte = a[byte_index] & 0xFF; 
        int bit_index = index % 8; 
		return (byte >> bit_index) & 1;
    }
    void LongNum:: set_bit(array_bytes& a, size_t index, int value) {
        int byte_index = index / 8; 
        if (byte_index >= a.size()) 
            a.resize(byte_index + 1, 0);
        uint8_t byte = a[byte_index] & 0xFF; 
        int bit_index = index % 8;
        if (value) byte |= 1 << bit_index;
        else byte &= ~(1 << bit_index);
        a[byte_index] = byte; 
    }

    void LongNum::add_bit(array_bytes& a, int value) {
        if (value == 0) return;
        if (a.size() == 0) a.push_back(0);
        uint16_t carry = value;
        for (size_t i = 0; i < a.size() && carry; ++i) {
            uint16_t sum = (uint16_t)a[i] + carry;
            a[i] = uint16_t(sum & 0xFF);
            carry = sum >> 8;

        }
        if (carry) a.push_back((uint8_t)carry);
    }

    void LongNum::shift_left(array_bytes& a) {
        uint16_t carry = 0; 
        for (size_t i = 0; i < a.size(); ++i) {
            uint16_t sum = a[i] << 1 | carry; 
            a[i] = sum & 0xFF; 
			carry = sum / 256;
        }
        if (carry) a.push_back(carry);
    }

    size_t LongNum::count_important_bits(const array_bytes& a) {
        if (a.size() == 0) return 0; 
        size_t cnt = 0; 
        size_t i = a.size() - 1;
        while (a[i] == 0 && i > 0) --i;
        uint8_t byte = a[i] & 0xFF; 
        while (byte != 0) {
            byte >>= 1;
            ++cnt;
        }
        return cnt +  i * 8;
    }
    LongNum LongNum ::operator /(const LongNum& ln) const {
        LongNum res("0"); 
        LongNum a = LongNum::abs(*this);
        LongNum b = LongNum::abs(ln);

        res.is_negative = (is_negative != ln.is_negative) ? true : false; 
        if (ln == LongNum("0")) throw std::invalid_argument("Division by zero");
        else if (a == b) {
            res = LongNum("1");
			res.is_negative = (is_negative != ln.is_negative) ? true : false;
            return res;
        }
        else if (a < b) return res; 
        else {
            LongNum a = LongNum::abs(*this);
            LongNum b = LongNum::abs(ln);
            size_t n_bits = LongNum::count_important_bits(a.num_in_bytes);
            res.num_in_bytes.assign((n_bits + 7) / 8, 0);
            LongNum r("0"); 
            for (int i = (int)n_bits - 1; i >= 0; --i) {
				shift_left(r.num_in_bytes);
                uint8_t bit = LongNum::get_bit(a.num_in_bytes, i);
                add_bit(r.num_in_bytes, bit);
                if (r >= b) {
                    r = r - b; 
                    set_bit(res.num_in_bytes, size_t(i), 1);
                }
            }
            while (r.num_in_bytes.size() > 1 && r.num_in_bytes.back() == 0) 
				r.num_in_bytes.pop_back();
            while (res.num_in_bytes.size() == 1 && res.num_in_bytes[0] == 0)
                res.is_negative = false; 
            return res;
        }
    }

   

    LongNum& LongNum :: operator *=(const LongNum& ln) {
        *this = *this * ln; 
        return *this;
    }
    LongNum& LongNum :: operator /= (const LongNum& ln) {
        *this = *this / ln; 
        return *this;
    }
    LongNum LongNum :: power(const LongNum& ln, int k) {
         LongNum res("1");
         if (k == 0) return res;
         else if (k < 0 ) throw std::invalid_argument("Negative exponent");
         else {
             res = ln; 
             for (int i = 0; i < k - 1; ++i)
                 res *= ln;
         }
         return res;
    }


    std::ostream& operator << (std::ostream& os, const LongNum& ln) {
        std::string s = LongNum::print_decimal(ln);
        os << s ; 
        return os; 

    }
    std::istream& operator >>(std::istream& is, LongNum& ln) {
        std::string str; 
        is >> str; 
        ln = LongNum(str);
       
        return is;
    }
    
     LongNum:: operator long long() const{
         long long res = 0; 
         int size = std::min(num_in_bytes.size(), sizeof(long long));
         for (int i = 0; i < size; i++) {
             long long temp =(long long)num_in_bytes[i] << (i * 8);
             res += temp; 
         }
         return (is_negative) ? -res : res ;
     }


    LongNum operator +(const LongNum& a, const char *str) {   
        return  a.LongNum::operator + (LongNum(str)); 
    }
    LongNum operator +(const char* str,const LongNum &a) {
        return LongNum(str).LongNum::operator + (a);
    }
    LongNum operator +(const LongNum& a, long long num) {
        return a.LongNum::operator + (LongNum(num)); 
    }
    LongNum operator +(long long num, const LongNum& a) {
        return LongNum(num).LongNum::operator + (a); 
    }
    LongNum operator -(const LongNum& a, const char* str) {
        return a.LongNum::operator - (LongNum(str)); 
    }
    LongNum operator -(const char* str, const LongNum& a) {
        return LongNum(str).LongNum::operator - (a); 
    }
    LongNum operator -(const LongNum& a, long long num) {
        return a.LongNum::operator - (LongNum(num)); 
    }
    LongNum operator -(long long num, const LongNum& a) {
        return LongNum(num).LongNum::operator + (a); 
    }
   
    LongNum operator * (const LongNum& a, long long b) {
        return a.LongNum::operator * (LongNum(b)); 
    }
    LongNum operator * (long long b, const LongNum& a) {
        return (LongNum(b)).LongNum::operator * (a); 
    }

    LongNum operator / (const LongNum& a, long long b) {
        return a.LongNum::operator / (LongNum(b)); 
    }

    LongNum operator / (long long b, const LongNum& a) {
        return (LongNum(b)).LongNum::operator / (a);
    }