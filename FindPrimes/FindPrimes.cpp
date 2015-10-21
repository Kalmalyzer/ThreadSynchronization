
// Example program which identifies and prints prime numbers

#include <iostream>
#include <vector>
using namespace std;

struct PrimeResult
{
	int value;
};

std::vector<PrimeResult> primes;
std::vector<PrimeResult> non_primes;

bool test_if_prime(int i)
{
	for (int j = 2; j <= i / 2; j++)
	if (!(i % j))
		return false;

	return true;
}

void evaluate_primes(int begin, int end)
{
	for (int i = begin; i < end; i++)
	{
		bool isPrime = test_if_prime(i);

		if (isPrime)
			primes.push_back({ i });
		else
			non_primes.push_back({ i });
	}
}

void print_results()
{
	for (auto& result : primes)
	{
		cout << "Prime found: " << result.value << endl;
	}
}

int main()
{
	evaluate_primes(1, 1000);
	print_results();

	return 0;
}