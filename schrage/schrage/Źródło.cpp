#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>


/*
	klasa przechowujaca pojedynczy proces
*/
class process
{
public:
	int r;	// czas przygotowania
	int p;	// czas na maszynie
	int q;	// czas odpoczynku
	int id;	// numer procesu

	/*
		konstruktor pojedynczego procesu
	*/

	process() {
		r = 0;
		p = 0;
		q = 0;
		id = 0;
	}
	/*
		dodawanie parametru procesu
		in:
			x - czas przygotowania
			y - czas trwania
			z - czas odpoczynku
			i - nr id procesu
	*/
	void add(int x, int y, int z, int i)
	{
		r = x;
		p = y;
		q = z;
		id = i + 1;
	}
	
	/*
	wyswietlanie procesu
	*/
	void show()
	{
		std::cout << r << " " << p << " " << q << std::endl;
	}

	/*
		przeciazenie operatora przypisania dla procesu
		in:
			a - proces, ktory zostaje przypisany
	*/
	process operator=(process &a)
	{
		r = a.r;
		p = a.p;
		q = a.q;
		id = a.id;
		return *this;
	}
};

/*
	klasa przechowujaca zestaw danych
*/
class tasks
{
public:
	int size;	// ilosc danych
	process *tab;	// wskaznik inicjalizujacy tablice procesow

	/*
		metoda wczytuj¹ca dane z pliku
		in:
			name - nazwa pliku
	*/
	void readFile(std::string name)
	{
		int a, b, c;	// tymczasowe zmienne przechowuj¹ce parametry procesu
		std::ifstream file;
		file.open(name);
		file >> size;
		tab = new process[size];	// tworzenie tablicy
		for (int i = 0; i < size; i++)
		{
			file >> a >> b >> c;
			tab[i].add(a, b, c, i);	
		}
		file.close();
	}

	/*
		metoda zwracajaca pierwszy element znajdujacy sie w tablicy
		out:
			tab[0] - pierwszy element tablicy
	*/
	process first_element()
	{
		return tab[0];
	}

	/*
		wyswietlanie procesow
	*/
	void show()
	{
		for (int i = 0; i < size; i++)
		{
			tab[i].show();
		}
	}
};

/*
	funkcja zamieniajaca miejscami dwa procesy
	in:
		x - proces pierwszy
		y - proces drugi
*/
void swap(process& x, process& y)
{
	process temp;
	temp = x;
	x = y;
	y = temp;
}

/*
	funkcja naprawiajaca kopiec sortujacy po parametrze r
	in:
		arr - wskaznik na tablice procesow
		size - rozmiar tablicy
		i - indeks, od ktorego zaczyna sie naprawianie kopca w dol
*/
void build_heap_R(process *arr, int size, int i)
{
	int parent = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;
	if (l < size && arr[l].r < arr[parent].r) // jezeli lewe dziecko jest mniejsze od rodzica
		parent = l;
	if (r < size && arr[r].r < arr[parent].r) // jezeli prawe dziecko jest mniejsze od rodzica
		parent = r;
	if (parent != i)	// jezeli nastapila zmiana rodzica
	{
		swap(arr[i], arr[parent]);	// zamienia rodzica z dzieckiem
		build_heap_R(arr, size, parent);	// naprawia kopiec w dol
	}
}

/*
	funkcja naprawiajaca kopiec sortujacy po parametrze q
	in:
		arr - wskaznik na tablice procesow
		size - rozmiar tablicy
		i - indeks, od ktorego zaczyna sie naprawianie kopca w dol
*/
void build_heap_Q(process* arr, int size, int i)
{
	int parent = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;
	if (l < size && arr[l].q > arr[parent].q)	// jezeli lewe dziecko jest wieksze od rodzica
		parent = l;
	if (r < size && arr[r].q > arr[parent].q)	// jezeli prawe dziecko jest wieksze od rodzica
		parent = r;
	if (parent != i)	// jezeli nastapila zmiana rodzica
	{
		swap(arr[i], arr[parent]);	// zamienia rodzica z dzieckiem
		build_heap_Q(arr, size, parent);	// naprawia kopiec w dol
	}
}

/*
	funkcja budujaca kopiec
	in:
		arr - wskaznik na tablice procesow
		size - rozmiar tablicy
		isR - zmienna okreslajaca po jakim parametrze ma byc budowany kopiec:
			  true - po parametrze r, false - po parametrze q
*/
void sortheap(process* arr, int size, bool isR)
{
	for (int i = size / 2 - 1; i >= 0; i--)
	{
		if(isR)
			build_heap_R(arr, size, i);
		else
			build_heap_Q(arr, size, i);
	}
}

/*
	funkcja dodajaca element do kopca
	in:
		arr - wskaznik na tablice procesow
		size - rozmiar tablicy
		new_task - proces do dodania
*/
void add_to_heap(process* arr, int size, process new_task)
{
	arr[size] = new_task;
	sortheap(arr, size + 1, false);
}

/*
	funkcja sciagajaca proces z kopca
	in:
		arr - wskaznik na tablice procesow
		size - rozmiar tablicy
		isR - isR - zmienna okreslajaca po jakim parametrze jest budowany kopiec:
			  true - po parametrze r, false - po parametrze q
	out:
		temp - proces sciagniety z kopca
*/
process take_from_heap(process* arr, int size, bool isR)
{
	process temp;
	temp = arr[0];
	swap(arr[0], arr[size-1]);	// zamiana pierwszego procesu z ostatnim
	if (isR)
		build_heap_R(arr, size-1, 0);	// naprawa kopca dla pozostalych elementow
	else
		build_heap_Q(arr, size-1, 0);	// naprawa kopca dla pozostalych elementow
	return temp;
}

/*
	algorytm Schrage bez podzialu
	in:
		data - dane, na ktorych ma zostac wykonany algorytm
	out:
		order - wektor zawierajacy kolejnosc wykonania zadan oraz czas
*/
std::vector<int> algorithm(tasks data)
{
	int size_not_ready = data.size;	// rozmiar kopca zadan niegotowych
	int size_ready = 0;		// rozmiar kopca zadan gotowych
	int t = 0;	// aktualny czas 
	int cmax;	// czas konca produkcji
	std::vector<int> order;	// wektor przechowujacy kolejnosc zadan
	process* prepared = new process[data.size];	// tablica przechowujaca kopiec zadan gotowych
	sortheap(data.tab, data.size, true);
	while (size_not_ready != 0 || size_ready != 0)	// dopoki oba kopce sie sa puste
	{
		while (data.first_element().r <= t && size_not_ready > 0)	// dopoki kopiec zadan niegotowych nie jest pusty oraz minal czas przygotowania pierwszego zadania na kopcu 
		{
			add_to_heap(prepared, size_ready++, take_from_heap(data.tab, size_not_ready--, true)); // dodaj do kopca gotowych pierwsze zadanie z gotowego
		}
		if (size_ready > 0) // jezeli kopiec zadan gotowych nie jest pusty
		{
			auto a = take_from_heap(prepared, size_ready, false);	// zabierz zadanie z kopca
			size_ready--;	// zmniejsz rozmiar kopca
			order.push_back(a.id);
			t += a.p;	// dodaj do aktualnego czasu czas wykonania zadania	
			cmax = std::max(t + a.q, cmax);	// wyznacz czas zakonczenia pracy
		}
		else	// jezeli kopiec zadan gotowych jest pusty, przejdz do nastepnej chwili czasu
		{
			t++;
		}
	}
	order.push_back(cmax);
	return order;
}

/*
	algorytm Schrage z podzialem
	in:
		data - dane, na ktorych ma zostac wykonany algorytm
	out:
		cmax - czas zakonczenia pracy
*/
int algorithm2(tasks data)
{
	int size_not_ready = data.size;
	int size_ready = 0;
	int t = 0;
	int cmax;
	process* prepared = new process[data.size];
	sortheap(data.tab, data.size, true);
	while (size_not_ready != 0 || size_ready != 0)
	{
		while (data.first_element().r <= t && size_not_ready > 0)
		{
			add_to_heap(prepared, size_ready++, take_from_heap(data.tab, size_not_ready--, true));
		}
		if (size_ready > 0)	// jezeli kopiec zadan gotowych nie jest pusty
		{
			prepared[0].p--;	// zmniejsz czas pozostaly do wykonania procesu
			t++;	// dodaj jednostke czsau
			if (prepared[0].p == 0)	//	jezeli proces aktualnie wykonywany sie zakonczyl
			{
				cmax = std::max(cmax, t + prepared[0].q);	// zaktualizuj czas zakonczenia pracy
				take_from_heap(prepared, size_ready--, false);	// zdejmij proces z kopca
			}
		}
		else
		{
			t++;
		}
	}
	return cmax;
}

int main()
{
	std::vector<std::string> files{ "data0.txt", "data1.txt", "data2.txt", "data3.txt", "data4.txt", "data5.txt", "data6.txt", "data7.txt", "data8.txt"};
	for (int i = 0; i < files.size(); i++)
	{
		tasks siemanko;
		std::vector<int> bla;
		siemanko.readFile(files[i]);
		std::cout << "------------------------" << std::endl;
		std::cout << files[i] << std::endl;
		std::cout << "Schrage bez podzialu: " << std::endl << "kolejnosc: " << std::endl;
		bla = algorithm(siemanko);
		for (int x : bla)
		{
			if (x == *(bla.end() - 1))
			{
				std::cout << std::endl << "czas: ";
			}
			std::cout << x << " ";
		}
		std::cout << std::endl << "Schrage z podzialem:" << std::endl << "czas: ";
		std::cout << algorithm2(siemanko) << std::endl << std::endl;
	}
	return 0;
}