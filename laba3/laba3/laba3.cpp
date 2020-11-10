
#include <iostream>
#include <time.h>
#include <locale.h>
#define TABLE_SIZE 8
using namespace std;

double congruential(int &); // прототип функции генерации псевдослучайных чисел
bool nullTest(double* a, int size);
void interval_count(double* mass, int position, int* counter);

int main()
{
	setlocale(LC_ALL, "Rus");
	const int number_numbers = 100; // количество псевдослучайных чисел
	double* mass_rand = new double[TABLE_SIZE];
	for (int i = 0; i < TABLE_SIZE; i++)
		mass_rand[i] = 0;
	int* mass_count = new int[10];
	for (int i = 0; i < 10; i++)
		mass_count[i] = 0;
	time_t timec = time(0);
	int x0 = timec; // начальное значение

	cout << "\n";
	int k = 0;
	int sch = 1;
	for (int i = 0; i < number_numbers; i++)
	{
		k = rand() % TABLE_SIZE;
		while (k == TABLE_SIZE)
			k = rand() % TABLE_SIZE;
		if (mass_rand[k] == 0)
			mass_rand[k] = congruential(x0);
		else
		{
			cout << "["<< sch << "]  " << mass_rand[k] << "\n"; 
			sch++;
			interval_count(mass_rand, k, mass_count);
			mass_rand[k] = congruential(x0);
		}
	}
	while (!nullTest(mass_rand, TABLE_SIZE))
	{
		k = rand() % TABLE_SIZE;
		while (k == TABLE_SIZE)
			k = rand() % TABLE_SIZE;
		if (mass_rand[k] != 0)
		{
			cout << "[" << sch << "]  " << mass_rand[k] << "\n";
			sch++;
			interval_count(mass_rand, k, mass_count);		
			mass_rand[k] = 0;
		}
	}
	cout << "\n Распределение \n";
	float n = 0;
	for (int i = 0; i < 10; i++)
	{
		cout << "[" << n << " - " << n + 0.1 << "]" << " -> " << mass_count[i] << "\n";
		n = n + 0.1;
	}
	delete mass_rand;
	delete mass_count;
	cin.peek();
	return 0;
}

bool nullTest(double* a, int size) // функция проверки таблицы на не пустые ячейки
{
	int count = 0;
	for (int i = 0; i < size; i++)
	{
		if (a[i] != 0)
			count++;
	}
	if (count == 0)
		return true;
	else
		return false;
}

void interval_count(double* mass, int position, int* counter) // функция подсчета частоты в интервале
{
	if ((0 < mass[position]) && (mass[position] < 0.1))
		counter[0]++;
	else if ((0.1 < mass[position]) && (mass[position] < 0.2))
		counter[1]++;
	else if ((0.2 < mass[position]) && (mass[position] < 0.3))
		counter[2]++;
	else if ((0.3 < mass[position]) && (mass[position] < 0.4))
		counter[3]++;
	else if ((0.4 < mass[position]) && (mass[position] < 0.5))
		counter[4]++;
	else if ((0.5 < mass[position]) && (mass[position] < 0.6))
		counter[5]++;
	else if ((0.6 < mass[position]) && (mass[position] < 0.7))
		counter[6]++;
	else if ((0.7 < mass[position]) && (mass[position] < 0.8))
		counter[7]++;
	else if ((0.8 < mass[position]) && (mass[position] < 0.9))
		counter[8]++;
	else if ((0.9 < mass[position]) && (mass[position] < 1))
		counter[9]++;
}

double congruential(int &x) // функция генерации псевдослучайных чисел
{
	const unsigned int m = 2147483647, // m = 2^31 - 1 = 2.147.483.647
		a = 48271, // множитель
		c = 8; // инкрементирующее значение
	x = ((a * x) + c) % m; // формула линейного конгруэнтного метода генерации псевдослучайных чисел 
	return (x / double(m));
}
