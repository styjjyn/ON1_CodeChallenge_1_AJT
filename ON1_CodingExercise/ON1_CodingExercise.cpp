// ON1_CodingExercise.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>


//Thread-safe queue used to store the numbers to be checked.
template<typename T>
class TSQueue
{
public:
	void push(const T& item) {
		std::unique_lock<std::mutex> lock(m_mutex);
		m_queue.push(item);
		m_cv.notify_one();
	}

	bool try_pop(T& item) {
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_queue.empty()) {
			return false;
		}

		item = m_queue.front();
		m_queue.pop();
		return true;
	}

private:
	std::mutex m_mutex;
	std::queue<T> m_queue;
	std::condition_variable m_cv;

};


// Thread-safe Vector for storing the prime numbers
class TSVector {
public:
	void push_back(int item) {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_vector.push_back(item);
	}

	const std::vector<int>& getVector() const {
		return m_vector;
	}

private:
	std::mutex m_mutex;
	std::vector<int> m_vector;
};



//Check to see if the number is prime
bool isPrime(int numberToCheck) {
	if (numberToCheck <= 1) {
		return false;
	}

	for (int i = 2; i <= sqrt(numberToCheck); ++i) {
		if (numberToCheck % i == 0) {
			return false;
		}
	}

	return true;
}



//The function used to iterate through the queue and check for prime numbers
void worker(TSQueue<int>& queue, TSVector& primeNumbers) {
	int numberToCheck = 0;

	while (queue.try_pop(numberToCheck)) {
		if (isPrime(numberToCheck))
		{
			primeNumbers.push_back(numberToCheck);
		}
	}

}


int main()
{
    std::cout << "Hello ON1!" << std::endl;
	std::cout << std::endl;

	std::vector<int> numbersToCheck = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
	TSQueue<int> queueToCheck;
	for (int num : numbersToCheck)
	{
		queueToCheck.push(num);
	}

	TSVector primeNumbers;
	std::thread thread1(worker, std::ref(queueToCheck), std::ref(primeNumbers));
	std::thread thread2(worker, std::ref(queueToCheck), std::ref(primeNumbers));

	thread1.join();
	thread2.join();

	const std::vector<int>& vectorOfPrimes = primeNumbers.getVector();
	std::cout << "The Prime Numbers are:" << std::endl;
	for (int num : vectorOfPrimes) {
		std::cout << " - " << num << std::endl;
	}

	return 0;
}




// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
