#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>

static int wywolania = 0;
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
	process operator=(process& a)
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
	process* tab;	// wskaznik inicjalizujacy tablice procesow

	/*
		metoda wczytujšca dane z pliku
		in:
			name - nazwa pliku
	*/
	void readFile(std::string name)
	{
		int a, b, c;	// tymczasowe zmienne przechowujšce parametry procesu
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

	/*
		konstruktor
	*/
	tasks()
	{
		size = 0;
		tab = nullptr;
	}

	/*
		konstruktor kopiujacy
	*/
	tasks(tasks& oth)
	{
		size = oth.size;
		tab = new process[size];
		for (int i = 0; i < size; i++)
		{
			tab[i] = oth.tab[i];
		}

	}
	/*
		destruktor klasy
	*/
	~tasks()
	{
		delete[] tab;
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
void build_heap_R(process* arr, int size, int i)
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
		if (isR)
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
	swap(arr[0], arr[size - 1]);	// zamiana pierwszego procesu z ostatnim
	if (isR)
		build_heap_R(arr, size - 1, 0);	// naprawa kopca dla pozostalych elementow
	else
		build_heap_Q(arr, size - 1, 0);	// naprawa kopca dla pozostalych elementow
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
	int cmax = 0;	// czas konca produkcji
	int last_q = 0;	// indeks procesu z ostatnim q
	int path_begin = 0;	// indeks poczatku sciezki krytycznej
	std::vector<int> jopy;	// wektor przechowujacy liste przerw na maszynie
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
			if (a.q + t > cmax)
				last_q = a.id;
			cmax = std::max(t + a.q, cmax);	// wyznacz czas zakonczenia pracy
		}
		else	// jezeli kopiec zadan gotowych jest pusty, przejdz do nastepnej chwili czasu
		{
			if (std::find(jopy.begin(), jopy.end(), data.tab[0].id) == jopy.end())
			{
				jopy.push_back(data.tab[0].id);	// dodanie przerwy na maszynie
			}
			t++;
		}
	}
	//	uzyskiwanie indeksow procesow w wektorze kolejnosci na podstawie ich id
	int i = 0;
	while (order[i] != last_q)
	{
		i++;
	}
	for (int j = i; j > 0; j--)
	{
		if (std::find(jopy.begin(), jopy.end(), order[j]) != jopy.end())
		{
			path_begin = j;
			j = 0;
		}
	}
	order.push_back(cmax);
	order.push_back(i);
	order.push_back(path_begin);
	delete[] prepared;
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
	delete[] prepared;
	return cmax;
}

/*
	funkcja wyznaczajaca nowe r
	in:
		input - zestaw danych
		A - poczatek sciezki krytycznej
		B - koniec sciezki krytycznej
		order - kolejnosc ulozenia zadan
	out:
		sum + minr - nowe r procesu
*/
int newr(tasks input, int A, int B, std::vector<int> order)
{
	int minr = 999999;
	int sum = 0;
	for (int i = A + 1; i <= B; i++)	//	przejscie przez sciezke krytyczna
	{
		if (input.tab[order[i] - 1].r < minr)	//	znalezienie najmniejszego r
			minr = input.tab[order[i] - 1].r;	//	przypisanie

		sum += input.tab[order[i] - 1].p;	//	wyznaczenie sumy p

	}
	return sum + minr;
}

/*
	funkcja wyznaczajaca nowe q
	in:
		input - zestaw danych
		A - poczatek sciezki krytycznej
		B - koniec sciezki krytycznej
		order - kolejnosc ulozenia zadan
	out:
		sum + minq - nowe q procesu
*/
int newq(tasks input, int A, int B, std::vector<int> order)
{
	int minq = 999999;
	int sum = 0;
	for (int i = A + 1; i <= B; i++)	//	przejscie przez sciezke krytyczna
	{
		if (input.tab[order[i] - 1].q < minq)	//	znalezienie najmniejszego q
			minq = input.tab[order[i] - 1].q;	//	przypisanie

		sum += input.tab[order[i] - 1].p;	//	wyznaczenie sumy p

	}
	return sum + minq;
}

/*
	funkcja wyznaczajaca granice H
	in:
		input - zestaw danych
		A - poczatek sciezki krytycznej
		B - koniec sciezki krytycznej
		order - kolejnosc ulozenia zadan
	out:
		sum + maxq + maxr - granica H
*/
int calc_H(tasks input, int A, int B, std::vector<int> order)
{
	int minr = 99999;
	int minq = 99999;
	int sum = 0;
	for (int i = A + 1; i <= B; i++)	// przejscie przez sciezke krytyczna
	{
		if (input.tab[order[i] - 1].r < minr)	//	znalezienie najmniejszego r
			minr = input.tab[order[i] - 1].r;	//	przypisanie
		if (input.tab[order[i] - 1].q < minq)	//	znalezienie najmniejszego q
			minq = input.tab[order[i] - 1].q;	//	przypisanie

		sum += input.tab[order[i] - 1].p;	//	wyznaczenie sumy p

	}
	return sum + minq + minr;
}

std::vector<int> BestOrder;	//	globalny wektor kolejnosci
int best = 9999999;	//	globalny cmax

/*
	algorytm Carliera
	in:
		input - zestaw danych
*/
void Carlier(tasks input)
{
	wywolania++;
	int UB;	//	gorna granica
	tasks temp = input;
	std::vector<int> order = algorithm(temp);	//	wywolanie schrage
	int path_begin = order[order.size() - 1];	//	zdjecie z wektora indeksu poczatku sciezki krytycznej
	order.pop_back();
	int last_q = order[order.size() - 1];	//	zdjecie z wektora indeksu konca sciezki krytycznej
	order.pop_back();

	int critical_process = last_q;

	for (int i = last_q; i >= path_begin; i--)	//	znalezienie procesu krytycznego
	{
		if (input.tab[order[i] - 1].q < input.tab[order[last_q] - 1].q)
		{
			critical_process = i;
			break;
		}
	}
	UB = order[order.size() - 1];	//	przypisanie	wyniku algorytmu schrage jako ub
	order.pop_back();
	if (UB < best)	//	jezeli nowe schrage jest lepsze niz aktualny najlepszy cmax
	{
		best = UB;	//	zaktualizuj najlepszy cmax
		BestOrder = order;	//	zaktualizuj kolejnosc
	}
	if (critical_process == last_q)	//	jezeli nie ma zadania krytycznego
	{
		return;	//	koniec
	}
	tasks input_copy = input;
	int sch_p = algorithm2(input_copy);	//	wywolanie schrage z podzialem
	if (sch_p >= best)	//	jezeli schrage z podzialem jest wieksze badz rowne aktualnemu najlepszemu cmax
	{
		return;	//	koniec
	}

	tasks input_copy_r = input;

	input_copy_r.tab[order[critical_process] - 1].r = newr(input_copy_r, critical_process, last_q, order);	//	wyznaczenie nowego r
	int LBr;
	//	wyznaczenie dolnej granicy dla zmiany r
	LBr = std::max(calc_H(input_copy_r, critical_process, last_q, order), std::max(calc_H(input_copy_r, critical_process - 1, last_q, order), sch_p));

	tasks input_copy_q = input;
	input_copy_q.tab[order[critical_process] - 1].q = newq(input_copy_q, critical_process, last_q, order);	//	wyznaczenie nowego q
	int LBq;
	//	wyznaczenie dolnej granicy dla zmiany q
	LBq = std::max(calc_H(input_copy_q, critical_process, last_q, order), std::max(calc_H(input_copy_q, critical_process - 1, last_q, order), sch_p));

	if (LBr <= LBq)	//	jezeli dolna granica dla zmiany r mniejsza rowna dolna granica dla zmiany q najpierw wejdz do zmiany r
	{
		if (LBr < best)	//	jezeli dolna granica lepsza niz aktualny najlepszy cmax
			Carlier(input_copy_r);	//	wywolaj algorytm dla zmienionego r
		if (LBq < best)	//	jezeli dolna granica lepsza niz aktualny najlepszy cmax
			Carlier(input_copy_q);	//	wywolaj algorytm dla zmienionego q
	}
	else {	//	jezeli LBr > LBq wejdz najpiejrw do zmiany q
		if (LBq < best)
			Carlier(input_copy_q);
		if (LBr < best)
			Carlier(input_copy_r);
	}
}


int main()
{
	std::vector<std::string> files{ "data0.txt", "data1.txt", "data2.txt", "data3.txt", "data4.txt", "data5.txt", "data6.txt", "data7.txt", "data8.txt" };
	//std::vector<std::string> files{ "data1.txt" };
	for (int i = 0; i < files.size(); i++)
	{
		wywolania = 0;
		best = 999999;
		tasks siemanko;
		siemanko.readFile(files[i]);
		std::cout << files[i] << std::endl;
		Carlier(siemanko);
		std::cout << best << std::endl;
		std::cout << "najlepsza kolejnosc to: \n";
		for (auto a : BestOrder)
		{
			std::cout << a - 1 << " ";
		}
		std::cout << std::endl;
		std::cout << "Carlier wywolany razy: " << wywolania << std::endl;
	}
	return 0;
}
