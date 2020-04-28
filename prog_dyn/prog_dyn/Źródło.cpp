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
	int d;	// oczekiwany czas zakoñczenia procesu

/*
	wyœwietlanie procesu 
*/
	void show()
	{
		std::cout << p << " " << w << " " << d << std::endl;
	}
};

/*
	zestaw wszystkich procesów
*/
class data
{
public:
	std::vector<process> tasks;	// wektor przechowuj¹cy wszystkie procesy
	int num;	// iloœæ procesów

	/*
		wczytywanie danych z pliku
		in:
			name - nazwa pliku
	*/
	void readFile(std::string name)
	{
		int a, b, c;	// tymczasowe zmienne przechowuj¹ce parametry procesu
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
		wyœwietlanie procesów
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
		time - czas zakoñczenia procesów
		task - ostatni proces 
	out:
		koszt wykonania procesu
*/
int cost(int time, process task)
{
	if (task.d > time)	// je¿eli proces koñczy siê przed oczekiwanym czasem
		return 0;
	return (time - task.d) * task.w; // je¿eli czas zosta³ przekroczony
}

/*
	wyznaczanie czasu zakoñczenia siê procesów
	in:
		tab - wektor wszystkich procesów
		number - liczba, która definiuje, które procesy s¹ brane pod uwagê
	out:
		time - czas wykonania procesów
*/
int gettime(std::vector<process> tab, int number)
{
	int time = 0;
	int i = 0;
	while (pow(2, i) <= number)		// wyznaczanie które procesy braæ pod uwagê
	{								// na podstawie liczby number (z zapisu binarnego)
		i++;						// znalezienie najwiêkszej potêgi 2, która mieœci siê w liczbie
	}
	i--;
	while ((number > 0) && (i >= 0))	// odejmowanie od liczby kolejnych potêg 2, umo¿liwiaj¹ce wyznaczenie, które procesy braæ
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
	wyznaczanie jakie konfiguracje procesów sk³adaj¹ siê na grupê
	in: 
		num - liczba decymalna 
	out:
		vec - wektor liczb, mo¿liwych kombinacji
*/
std::vector<int> bin(int num)
{
	std::vector<int> vec;
	int n = 1;
	int i = 0;
	int temp;
	while (pow(2, n) <= num)	// znalezienie najwiêkszej potêgi 2 mieszcz¹cej siê w num
	{
		n++;
	}
	n--;
	temp = num;
	while (n >= 0)	// rozbijanie liczby na pojedyncze sk³adniki
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
		take - liczba decymalna opisuj¹ca jakie procesy s¹ brane pod uwagê
		tab - wektor zawieraj¹cy koszty wykonania kolejnych grup procesów
		data - wektor zawieraj¹cy wszystkie procesy
	out:
		temp - najmniejszy koszt dla wybranych procesów
*/
int alg(int take, std::vector<int> &tab, std::vector<process> data)
{
	int temp = 999999;
	std::vector<int> tasks;
	if (take == 0)	// je¿eli zejdzie do procesu 0 - koniec rekurencji
		return 0;
	if (tab[take] != -1)	// je¿eli wektor kosztów jest ju¿ wype³niony
		return tab[take];
	tasks = bin(take); // wyznaczenie procesów
	for (int a : tasks) // dla ka¿dego procesu oblicz koszt z nim na koñcu 
	{
		temp = std::min(temp, alg(a, tab, data) + cost(gettime(data,take), data[log2(take-a)]));
	}
	tab[take] = temp; // wpisanie kosztu to tablicy kosztów
	return temp;
}

/*
	wyznaczanie najmniejszego kosztu
	in:
		tab - wektor wszystkich procesów
	out:
		arr - wektor wyznaczonych kosztów dla wszystkich grup procesów
*/
std::vector<int> calculate(std::vector<process> tab)
{
	int time = 0;
	std::vector<int> arr;
	for (int i = 0; i < pow(2,tab.size()); i++) // wype³nienie wektora kosztów -1
	{
		arr.push_back(-1);
	}
	arr[0] = 0;

	alg(pow(2,tab.size())-1, arr, tab);	// wywo³anie algorytmu dla pe³nego zestawu danych
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