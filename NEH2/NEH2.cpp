// NEH2.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <fstream>
#include <cstdio>
#include <ctime>

int Cmax(int** orderedData, int machines, int tasks)
{
    int** tempdata = new int* [tasks];
    for (int i = 0; i < tasks; i++)
    {
        tempdata[i] = new int[machines];
    }

    for (int i = 0; i < tasks; i++)
    {
        for (int j = 0; j < machines; j++)
        {
            if (i == 0 && j == 0)
                tempdata[i][j] = 0 + orderedData[i][j];
            else if (i == 0)
                tempdata[i][j] = tempdata[0][j - 1] + orderedData[i][j];
            else if(j == 0)
                tempdata[i][j] = tempdata[i-1][0] + orderedData[i][j];
            else
                tempdata[i][j] = std::max(tempdata[i-1][j], tempdata[i][j-1]) + orderedData[i][j];
        }
    }

    int a = tempdata[tasks - 1][machines - 1];

    for ( int i = 0; i < tasks ; i++)
    {
        delete [] tempdata[i];
    }
    delete [] tempdata;

    return a;
}

int* NEH(int** data, int machines, int tasks)
{
    int* order = new int[tasks];
    for (int i = 0; i < tasks; i++)
    {
        order[i] = i+1;
    }
    int* sums = new int[tasks];
    for (int i = 0; i < tasks; i++)
    {
        sums[i] = 0;
        for (int j = 0; j < machines;j++)
        {
            sums[i] += data[i][j];
        }
    }


    for (int i = 0; i < tasks - 1; i++)
    {
        for (int j = 0; j < tasks - i - 1; j++)
        {
            if (sums[j] < sums[j + 1])
            {
                std::swap(sums[j], sums[j + 1]);
                std::swap(data[j], data[j + 1]);
                std::swap(order[j], order[j + 1]);
            }

        }
    }

    std::cout << std::endl;
    for (int i = 0;i < tasks;i++)
    {
        int bestpos = -1;
        int bestCmax = 0xFFFF; 
        for (int j =i ;j >=1;j--)
        {
            std::swap(data[j], data[j-1]);
            std::swap(order[j], order[j-1]);
        }
      
        for (int j = 0;j <= i;j++)
        {
            int a = Cmax(data, machines, i+1);
            //std::cout << " wyliczony pośredni cmax dla procesu nr " << order[j] << " na pozycji " << j << " wynosi " << a << std::endl;
            if (a < bestCmax)
            {
                bestpos = j;
                bestCmax = a;
            }
            if (j < i)
            {
                std::swap(data[j], data[j + 1]);
                std::swap(order[j], order[j + 1]);
            }
        }
        for (int j = i; j > bestpos; j--)
        {
            std::swap(data[j], data[j - 1]);
            std::swap(order[j], order[j - 1]);
        }
    }
    std::cout << Cmax(data, machines, tasks)<<std::endl;
    return order;
}

int main()
{
    std::ifstream plik;
    std::string nazwa = "dane0.txt";
    plik.open(nazwa);
  clock_t start = clock();
  std::string temp;
    for (int i = 0; i < 121; i++)
    {
        do
        {
            plik >> temp;
        } while (temp[0] != 'd');
        int tasks, machines;

        plik >> tasks;
        plik >> machines;
        int** readData = new int* [tasks];
        for (int i = 0; i < tasks; i++)
        {
            readData[i] = new int[machines];
        }
        for (int i = 0; i < tasks; i++)
        {
            for (int j = 0; j < machines; j++)
            {
                plik >> readData[i][j];
            }
        }
        int* res = NEH(readData, machines, tasks);
        for (int i = 0;i < tasks;i++)
        {
            std::cout << res[i] << " ";
        }
        printf("Czas wykonywania: %lu ms\n", clock() - start);

    }
      
        plik.close();
        printf("Czas wykonywania całości: %lu ms\n", clock() - start);
}
