#include "encoder.h"
using namespace std;
encoder::encoder(int om, int on)
{
	m = om;
	n = on;
	d.resize(n);
	k.resize(n);
	srand(time(0));
	primes.push_back(2);
	for (int i = 3; primes.size() < 8191; i++)
	{
		bool flag = true;
		for (int j = 0; j < primes.size(); j++)
		{
			if (primes[j] > sqrt(i))
			{
				break;
			}
			if ((i % primes[j]) == 0)
			{
				flag = false;
				break;
			}
		}
		if (flag)
		{
			primes.push_back(i);
		}
	}
}
istream& operator>>(istream& in, encoder& o)
{
	o.read(in);
	return in;
}
ostream& operator<<(ostream& out, const encoder& o)
{
	o.write(out);
	return out;
}
istream& encoder::read(istream& in)
{
	text.clear();
	char* buf = new char[524288];
	for (int i = 0; i < 524288; i++)
	{
		buf[i] = 0;
	}
	in.read(buf, 524288);
	for (int i = 0; i < 524288; i++)
	{
		int error = 0;
		for (int j = 0; j < 8; j++)
		{
			if (buf[i + j] == -52)
			{
				error++;
			}
		}
		if (error == 8)
		{
			break;
		}
		text.push_back((unsigned char)buf[i]);
	}
	delete[] buf;
	return in;
}
ostream& encoder::write(ostream& out) const
{
	out << text;
	return out;
}
void encoder::load(const string& filename, bool isBinary)
{
	ifstream* fin;
	if (!isBinary)
	{
		fin = new ifstream(filename, ios_base::in);
	}
	else
	{
		fin = new ifstream(filename, ios_base::in | ios_base::binary);
	}
	fin->exceptions(ios_base::badbit | ios_base::failbit);
	fin->seekg(0, ios::end);
	size_t size = fin->tellg();
	fin->seekg(0);
	text.clear();
	char* buf = new char[size + 100];
	for (int i = 0; i < size; i++)
	{
		buf[i] = 0;
	}
	try
	{
		fin->read(buf, size);
	}
	catch (const ios_base::failure& e)
	{

	}
	for (int i = 0; i < size; i++)
	{
		text.push_back((unsigned char)buf[i]);
	}
	delete[] buf;
	fin->close();
	delete fin;
}
void encoder::save(const string& filename, bool isBinary) const
{
	ofstream* fout;
	if (!isBinary)
	{
		fout = new ofstream(filename, ios_base::out);
	}
	else
	{
		fout = new ofstream(filename, ios_base::out | ios_base::binary);
	}
	fout->exceptions(ios_base::badbit | ios_base::failbit);
	char* buf = new char[text.size() + 100];
	for (int i = 0; i < text.size(); i++)
	{
		buf[i] = text[i];
		fout->put(buf[i]);
	}
	fout->close();
	delete fout;
}
string encoder::getText()
{
	return text;
}
void encoder::setText(string& str)
{
	text = str;
}
secret_s encoder::encode(int o)
{
	M = o;
	int min = 0;
	while (primes[min] <= M)
	{
		min++;
	}
	while (true)
	{
		bool flag = false;
		int num = min + rand() % (primes.size() - 1 - n);
		if (num < primes.size() - 1 - n)
		{
			for (int shift = 1; shift < primes.size() - num - n; shift++)
			{
				for (int i = 0; i < n; i++)
				{
					d[i] = primes[num + shift + i];
				}
				p = primes[num];
				int left = 1;
				int right = p;
				for (int i = 0; i < m; i++)
				{
					left *= d[i];
				}
				for (int i = 2; i <= m; i++)
				{
					right *= d[n - 1 - m + i];
				}
				if (left > right)
				{
					flag = true;
					break;
				}
			}
		}
		else
		{
			for (int i = 0; i < n; i++)
			{
				d[i] = primes[num + 1 + i];
			}
			p = primes[num];
			int left = 1;
			int right = p;
			for (int i = 0; i < m; i++)
			{
				left *= d[i];
			}
			for (int i = 2; i <= m; i++)
			{
				right *= d[n - 1 - m + i];
			}
			if (left > right)
			{
				flag = true;
				break;
			}
		}
		if (flag)
		{
			break;
		}
	}
	int r = rand();
	int newM = M + r * p;
	for (int i = 0; i < n; i++)
	{
		k[i] = newM % d[i];
	}
	secret_s result;
	result.p = p;
	result.d = d;
	result.k = k;
	return result;
}
int encoder::decode(secret_s sec)
{
	M = 0;
	int newM = 0;
	int mod = 1;
	for (int i = 0; i < m; i++)
	{
		mod *= sec.d[i];
	}
	vector<vector<int>> ans;
	ans.resize(m);
	for (int i = 0; i < m; i++)
	{
		for (int j = k[i]; j < mod; j += d[i])
		{
			ans[i].push_back(j);
		}
	}
	for (int i = 0; i < ans[0].size(); i++)
	{
		newM = ans[0][i];
		bool flag = true;
		for (int j = 1; j < m; j++)
		{
			if (!binary_search(ans[j].begin(), ans[j].end(), newM))
			{
				flag = false;
				break;
			}
		}
		if (flag)
		{
			break;
		}
	}
	M = newM % sec.p;
	return M;
}
encoder::~encoder()
{

}