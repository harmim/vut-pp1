// Author: Dominik Harmim <iharmim@fit.vut.cz>

#include <mutex>


class Test
{
private:
	std::mutex mutexA, mutexB;


	void f1() {}
	void f2() {}
	void f3() {}
	void g() {}


public:
	void test_std_mutex()
	{
		mutexA.lock(); // {f1, f2}
		f1(); f2();
		mutexA.unlock();

		g();

		mutexA.lock(); // {f1, f2, f3}
		{
			f1();
			mutexB.lock(); // {f3}
			f3();
			mutexB.unlock();
			f2();
		}
		mutexA.unlock();
	}


	void test_std_lock()
	{
		std::lock(mutexA, mutexB); // {f1, f2}; {f1, f2, f3}
		f1(); f2();
		mutexA.unlock();
		f3();
		mutexB.unlock();
	}


	void test_lock_guard()
	{
		std::lock_guard<std::mutex> guardA(mutexA); // {f1, f2, f3}
		f1();
		{
			std::lock_guard<std::mutex> guardB(mutexB); // {f3}
			f3();
		}
		f2();
	}


	void test_unique_lock()
	{
		std::unique_lock<std::mutex> lockA(mutexA, std::defer_lock); // {f2, f3}
		g();
		lockA.lock();
		{
			std::unique_lock<std::mutex> lockB(mutexB); // {f3}
			f3();
			lockB.unlock();
		}
		f2();
	}


	void test_violations()
	{
		f1(); f2(); // (f1, f2)
	}
};


int main()
{
	Test test;

	test.test_std_mutex();
	test.test_std_lock();
	test.test_lock_guard();
	test.test_unique_lock();

	test.test_violations();

	return 0;
}
