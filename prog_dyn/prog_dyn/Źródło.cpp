#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>


/* 
	pojedynczy proces
*/
class process
{
public:

	process(int x, int y, int z) :p(x), w(y), d(z) {}
	int p;	// czas wykonywania procesu
	int w;	// waga 
	int d;	// oczekiwany czas zako�czenia procesu

/*
	wy�wietlanie procesu 
*/
	void show()
	{
		std::cout << p << " " << w << " " << d << std::endl;
	}
};

/*
	zestaw wszystkich proces�w
*/
class data
{
public:
	std::vector<process> tasks;	// wektor przechowuj�cy wszystkie procesy
	int num;	// ilo�� proces�w

	/*
		wczytywanie danych z pliku
		in:
			name - nazwa pliku
	*/
	void readFile(std::string name)
	{
		int a, b, c;	// tymczasowe zmienne przechowuj�ce parametry procesu
		std::ifstream file;
		file.open(name);
		file >> num;
		for (int i = 0; i < num; i++)
		{
			file >> a >> b >> c;
			tasks.emplace_back(a, b, c);	// dodawanie procesu do wektora
		}
		file.close();
	}
	
	/*
		wy�wietlanie proces�w
	*/
	void show()
	{
		for (process a : tasks)
		{
			a.show();
		}
	}
};

/*
	wyznaczanie kosztu ostatniego procesu
	in:
		time - czas zako�czenia proces�w
		task - ostatni proces 
	out:
		koszt wykonania procesu
*/
int cost(int time, process task)
{
	if (task.d > time)	// je�eli proces ko�czy si� przed oczekiwanym czasem
		return 0;
	return (time - task.d) * task.w; // je�eli czas zosta� przekroczony
}

/*
	wyznaczanie czasu zako�czenia si� proces�w
	in:
		tab - wektor wszystkich proces�w
		number - liczba, kt�ra definiuje, kt�re procesy s� brane pod uwag�
	out:
		time - czas wykonania proces�w
*/
int gettime(std::vector<process> tab, int number)
{
	int time = 0;
	int i = 0;
	while (pow(2, i) <= number)		// wyznaczanie kt�re procesy bra� pod uwag�
	{								// na podstawie liczby number (z zapisu binarnego)
		i++;						// znalezienie najwi�kszej pot�gi 2, kt�ra mie�ci si� w liczbie
	}
	i--;
	while ((number > 0) && (i >= 0))	// odejmowanie od liczby kolejnych pot�g 2, umo�liwiaj�ce wyznaczenie, kt�re procesy bra�
	{
		if ((number - pow(2, i)) >= 0)
		{
			time = time + tab[i].p;
			number = number - pow(2, i);
		}
		i--;
	}
	return time;
}

/*
	wyznaczanie jakie konfiguracje proces�w sk�adaj� si� na grup�
	in: 
		num - liczba decymalna 
	out:
		vec - wektor liczb, mo�liwych kombinacji
*/
std::vector<int> bin(int num)
{
	std::vector<int> vec;
	int n = 1;
	int i = 0;
	int temp;
	while (pow(2, n) <= num)	// znalezienie najwi�kszej pot�gi 2 mieszcz�cej si� w num
	{
		n++;
	}
	n--;
	temp = num;
	while (n >= 0)	// rozbijanie liczby na pojedyncze sk�adniki
	{
		if ((temp - pow(2, n)) >= 0)
		{
			temp = temp - pow(2, n);
			vec.push_back(num - pow(2, n));
		}
		n--;
	}
	return vec;
}

/*
	algorytm wyznaczania kosztu
	in:
		take - liczba decymalna opisuj�ca jakie procesy s� brane pod uwag�
		tab - wektor zawieraj�cy koszty wykonania kolejnych grup proces�w
		data - wektor zawieraj�cy wszystkie procesy
	out:
		temp - najmniejszy koszt dla wybranych proces�w
*/
int alg(int take, std::vector<int> &tab, std::vector<process> data)
{
	int temp = 999999;
	std::vector<int> tasks;
	if (take == 0)	// je�eli zejdzie do procesu 0 - koniec rekurencji
		return 0;
	if (tab[take] != -1)	// je�eli wektor koszt�w jest ju� wype�niony
		return tab[take];
	tasks = bin(take); // wyznaczenie proces�w
	for (int a : tasks) // dla ka�dego procesu oblicz koszt z nim na ko�cu 
	{
		temp = std::min(temp, alg(a, tab, data) + cost(gettime(data,take), data[log2(take-a)]));
	}
	tab[take] = temp; // wpisanie kosztu to tablicy koszt�w
	return temp;
}

/*
	wyznaczanie najmniejszego kosztu
	in:
		tab - wektor wszystkich proces�w
	out:
		arr - wektor wyznaczonych koszt�w dla wszystkich grup proces�w
*/
std::vector<int> calculate(std::vector<process> tab)
{
	int time = 0;
	std::vector<int> arr;
	for (int i = 0; i < pow(2,tab.size()); i++) // wype�nienie wektora koszt�w -1
	{
		arr.push_back(-1);
	}
	arr[0] = 0;

	alg(pow(2,tab.size())-1, arr, tab);	// wywo�anie algorytmu dla pe�nego zestawu danych
	std::cout << arr[arr.size() - 1] << std::endl;
	return arr;
}


int main()
{
	std::vector<std::string> files{ "dane0.txt", "dane1.txt", "dane2.txt", "dane3.txt", "dane4.txt", "dane5.txt", "dane6.txt", "dane7.txt", "dane8.txt", "dane9.txt", "dane10.txt"};
	for (int i = 0; i < files.size(); i++)
	{
		data d;
		d.readFile(files[i]);
		std::cout << "Suma WiTi dla " << files[i] << " : ";
		calculate(d.tasks);
	}
	return 0;
}