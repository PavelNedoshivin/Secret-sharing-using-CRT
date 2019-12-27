#ifndef ENCODER_H
#define ENCODER_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>
typedef struct secret
{
	int p;
	std::vector<int> d;
	std::vector<int> k;
} secret_s;
class encoder
{
private:
	std::string text;
	std::vector<int> primes;
	int M;
	int p;
	int n;
	int m;
	std::vector<int> d;
	std::vector<int> k;
public:
	encoder(int om, int on);
	friend std::istream& operator>>(std::istream& in, encoder& o);
	friend std::ostream& operator<<(std::ostream& out, encoder& o);
	std::istream& read(std::istream& in);
	std::ostream& write(std::ostream& write) const;
	void load(const std::string& filename, bool isBinary);
	void save(const std::string& filename, bool isBinary) const;
	std::string getText();
	void setText(std::string& str);
	secret_s encode(int o);
	int decode(secret_s sec);
	~encoder();
};
#endif