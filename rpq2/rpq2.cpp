#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

class krotka
{
public:

	krotka(int x, int y, int z) :r(x), p(y), q(z) {};
	int r;
	int p;
	int q;

};


void razrazsort(std::vector<krotka>& arr, int n)
{
	int i, j;
	std::vector<krotka> res(arr.begin(), arr.end());
	for (i = 0; i < (n - 1)/2; i++)
	{
		krotka min = arr[i];
		krotka max = arr[i];
		for (j = 0+i; j < n - i- 1; j++)
		{
			if (min.r > arr[j].r)
			{
				min = arr[j];
			}
		}
		res[i] = min;
		for (j = 0+i; j < n - i - 1; j++)
		{
			if (max.q < arr[j].q)
			{
				max = arr[j];
			}
		}
		res[n - 1 - i] = max;
	}
	arr = res;
	
}


class dane
{
public:
	std::vector<krotka> numerki;
	int liczba;

	void czytajplik(std::string nazwa)
	{
		int a, b, c;
		std::ifstream plik;
		plik.open(nazwa);
		plik >> liczba;
		for (int i = 0; i < liczba; i++)
		{
			plik >> a >> b >> c;
			numerki.emplace_back(a, b, c);
		}
		plik.close();
	}

	void wyswietl()
	{
		for (krotka a : numerki)
		{
			std::cout << a.r << " " << a.p << " " << a.q << std::endl;
		}
	}


};

std::vector<int> ret123(dane wejscie)
{
	std::vector<int> p;
	for (unsigned int i = 0; i < wejscie.numerki.size(); i++)
		p.push_back(i);
	return p;
}

int czas(std::vector<krotka> dane, std::vector<int> kolejnosc)
{
	int m = 0;
	int c = 0;
	for (unsigned int i = 0; i < dane.size(); i++)
	{
		int j = kolejnosc[i];
		m = dane[j].p + std::max(m, dane[j].r);
		c = std::max(c, m + dane[j].q);
	}
	return c;
}


int main()
{
	std::vector<std::string> pliki{ "dane1.txt", "dane2.txt", "dane3.txt", "dane4.txt" };
	int czaskoncowy = 0;
	for (int i = 0; i < 4; i++)
	{

		dane k;
		k.czytajplik(pliki[i]);
		k.wyswietl();
		razrazsort(k.numerki, k.liczba);
		czaskoncowy += czas(k.numerki, ret123(k));
	}
	std::cout << "czas koncowy: " << czaskoncowy << std::endl;
	return 0;
}