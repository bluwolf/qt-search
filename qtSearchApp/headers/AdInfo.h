#ifndef ADINFO_H
#define ADINFO_H

#include <string>

using namespace std;

class Ad
{
public:
	Ad(double b, string c): bidAmount(b), companyName(c){};
	~Ad(){};

	double bidAmount;
	string companyName;

	double currentBill =0;
};

#endif