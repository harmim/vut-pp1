// Author: Dominik Harmim <iharmim@fit.vut.cz>

#include <stdlib.h>
#include <pthread.h>


pthread_mutex_t lockA, lockB, lockC;
struct locks
{
	pthread_mutex_t lockA, lockB;
} locks;


void f1(void) {}
void f2(void) {}
void f3(void) {}
void f4(void) {}
void f5(void) {}
void ff(void) { f1(); f2(); }


void test1(void)
{
	f4();
	pthread_mutex_lock(&lockA); // {f1, f2, f3}
	{
		f1();
		pthread_mutex_lock(&lockB); // {f2}
		f2();
		pthread_mutex_unlock(&lockB);
		f3();
	}
	pthread_mutex_unlock(&lockA);
	f5();

	f4();
	pthread_mutex_lock(&lockA); // {f1, f2}
	f1();
	pthread_mutex_lock(&lockB); // {f2, f3}
	f2();
	pthread_mutex_unlock(&lockA);
	f3();
	pthread_mutex_unlock(&lockB);
	f5();
}


void test1_struct(void)
{
	f4();
	pthread_mutex_lock(&locks.lockA); // {f1, f2, f3}
	{
		f1();
		pthread_mutex_lock(&locks.lockB); // {f2}
		f2();
		pthread_mutex_unlock(&locks.lockB);
		f3();
	}
	pthread_mutex_unlock(&locks.lockA);
	f5();

	f4();
	pthread_mutex_lock(&locks.lockA); // {f1, f2}
	f1();
	pthread_mutex_lock(&locks.lockB); // {f2, f3}
	f2();
	pthread_mutex_unlock(&locks.lockA);
	f3();
	pthread_mutex_unlock(&locks.lockB);
	f5();
}


void test2(void)
{
	pthread_mutex_lock(&lockA); // {f1, f2, f3, f4, f5}
	{
		f1();
		pthread_mutex_lock(&lockB); // {f2}
		f2();
		pthread_mutex_unlock(&lockB);
		f3();
		pthread_mutex_lock(&lockB); // {f4}
		f4();
		pthread_mutex_unlock(&lockB);
		f5();
	}
	pthread_mutex_unlock(&lockA);

	pthread_mutex_lock(&lockA); // {f1}
	f1();
	pthread_mutex_lock(&lockB); // {f3}
	pthread_mutex_unlock(&lockA);
	f3();
	pthread_mutex_unlock(&lockB);

	pthread_mutex_lock(&lockA); // {f1, f2, f3}
	f1();
	pthread_mutex_lock(&lockB); // {f2, f3, f4}
	f2(); f3();
	pthread_mutex_unlock(&lockA);
	f4();
	pthread_mutex_unlock(&lockB);
}


void test_only_lock(void)
{
	pthread_mutex_lock(&lockA); // {f1, f2, f3}
	f1();
	pthread_mutex_lock(&lockB); // {f2, f3}
	f2(); f3();
	pthread_mutex_unlock(&lockC);
}


void test_only_unlock(void)
{
	f1();
	pthread_mutex_unlock(&lockA);
	f2(); f3();
	pthread_mutex_unlock(&lockB);
}


void test_nested(void)
{
	pthread_mutex_lock(&lockA); // {f1, f2, f3, f4, ff}
	f3(); ff();
	pthread_mutex_lock(&lockB); // {f1, f2, f4, f5, ff}
	f4();
	pthread_mutex_unlock(&lockA);
	f5(); ff();
	pthread_mutex_unlock(&lockB);
}


int main(void)
{
	if (pthread_mutex_init(&lockA, NULL)) return 1;
	if (pthread_mutex_init(&lockB, NULL)) return 1;
	if (pthread_mutex_init(&lockC, NULL)) return 1;
	if (pthread_mutex_init(&locks.lockA, NULL)) return 1;
	if (pthread_mutex_init(&locks.lockB, NULL)) return 1;

	test1();
	test1_struct();
	test2();
	test_only_lock();
	test_only_unlock();
	test_nested();

	pthread_mutex_destroy(&lockA);
	pthread_mutex_destroy(&lockB);
	pthread_mutex_destroy(&lockC);
	pthread_mutex_destroy(&locks.lockA);
	pthread_mutex_destroy(&locks.lockB);

	return 0;
}
