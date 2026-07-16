#include "LongNum.h" 


int main() {
	/*LongNum a("1234567890345678345678903456789"); 
	LongNum b (21345678904567);
	LongNum c; 
	LongNum d(a); 


	std::cout <<"a =" << a << std::endl;
	std::cout << "b =" <<b << std::endl;
	std::cout << "c =" << c << std::endl;

	std::cout <<"d =" << d << std::endl;
	return 0; 

	try {
		LongNum e ("-12739ffdfgg"); 
		std::cout << e << std::endl;
	}
	catch (const std:: invalid_argument& ex) {
		std::cout << ex.what() << std::endl; 
	}


	LongNum a("1234567890345678345678903456789");
	LongNum b("0");
	if (!a) std::cout << "true" << std::endl;
	else std::cout << "false" << std::endl;

	if (!b) std::cout << "true" << std::endl;
	else std::cout << "false" << std::endl;


	LongNum a = "12222222222636447428238283181219232013001440014"; 
	LongNum b = 12838848283; 
	LongNum c = a; 

	std:: cout << "a = " << a << std::endl;
	std::cout << "b = " << b << std::endl;
	std::cout << "c = " << c << std::endl;


	

	if (a == b) std::cout << "true" << std::endl;
	else std::cout << "false" << std::endl; 
	
	if (a > b) std::cout << "true" << std::endl;
	else std::cout << "false" << std::endl;
	
	if (a < b) std::cout << "true" << std::endl;
	else std::cout << "false" << std::endl;

	LongNum a = "-12349934924923991329994445934920"; 
	LongNum b = "1234993492492399132999444593492";

	a *= b; 
	std::cout << a << std::endl;



	if (a >= b) std::cout << "true" << std::endl;
	else std::cout << "false" << std::endl;


	if (a >= b) std::cout << "true" << std::endl;
	else std::cout << "false" << std::endl;
*/



	LongNum c = "-100000000000000000000000000000000000"; 
	LongNum d = "200000000000000000000000000000000000"; 
	LongNum z = d * d; 
	z.print(); 
	std::cout << std::endl;
	std:: cout << z << std::endl;

	//std::cout << LongNum::power(d, 6) << std::endl;
}