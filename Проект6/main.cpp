#include "encoder.h"
using namespace std;
int main()
{
	int n = 0;
	cout << "Max users: ";
	cin >> n;
	int m = 0;
	cout << "Min users: ";
	cin >> m;
	int M = 0;
	cout << "Secret: ";
	cin >> M;
	encoder* test = new encoder(m, n);
	secret_s key = test->encode(M);
	cout << "p = " << key.p << endl;
	cout << "d = [";
	for (int i = 0; i < n - 1; i++)
	{
		cout << key.d[i] << " ";
	}
	cout << key.d[n - 1] << "]" << endl;
	cout << "k = [";
	for (int i = 0; i < n - 1; i++)
	{
		cout << key.k[i] << " ";
	}
	cout << key.k[n - 1] << "]" << endl;
	int res = test->decode(key);
	if (M == res)
	{
		cout << "Correct" << endl;
	}
	else
	{
		cout << "Incorrect" << endl;
	}
	delete test;
	return 0;
}