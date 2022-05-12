// Author: Dominik Harmim <iharmim@fit.vut.cz>

#include <stdlib.h>
#include <pthread.h>


pthread_mutex_t lockA, lockB, lockC;


void f1(void) {}
void f2(void) {}
void f3(void) {}
void f4(void) {}
void g(void) {}
// (f1, f4); (f3, f1); warning (f1, f4); warning (f3, f1)
void ff(void) { f3(); f1(); f4(); }


void atomic_sequences(void)
{
	pthread_mutex_lock(&lockA); // {f1, f2, f3}
	f1(); f2(); f3();
	pthread_mutex_unlock(&lockA);

	g();

	pthread_mutex_lock(&lockA); // {f2, f4}
	f4(); f2();
	pthread_mutex_unlock(&lockA);

	g();

	pthread_mutex_lock(&lockA); // {f1, f3}
	f1(); f3();
	pthread_mutex_unlock(&lockA);

	g();

	pthread_mutex_lock(&lockA); // {f1, f3, f4, ff}
	ff(); f3();
	pthread_mutex_unlock(&lockA);
}


void test(void)
{
	f1();
	pthread_mutex_lock(&lockA); // {f2, f3}
	f2(); // (f1, f2)
	pthread_mutex_lock(&lockB); // {f3, f4}
	f3();
	pthread_mutex_unlock(&lockA);
	f4();
	pthread_mutex_unlock(&lockB);
	f3(); // (f4, f3)

	g();

	pthread_mutex_lock(&lockA); // {f1, f2}
	f2(); f1();
	pthread_mutex_lock(&lockB); // {f1, f2}
	pthread_mutex_unlock(&lockA);
	f2(); f1(); // (f1, f2)
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
	f2(); f3(); // (f1, f2); (f2, f3)
	pthread_mutex_unlock(&lockB);
}


void test_nested(void)
{
	ff(); // (ff, f3)
	pthread_mutex_lock(&lockA); // {f2, f3}
	f2(); // (f4, f2)
	pthread_mutex_lock(&lockB); // {f3, f4}
	f3();
	pthread_mutex_unlock(&lockA);
	f4();
	pthread_mutex_unlock(&lockB);
	ff(); // (f4, ff); (ff, f3)

	g();

	pthread_mutex_lock(&lockA); // {f1, f3, f4, ff}
	f3(); ff();
	pthread_mutex_lock(&lockB); // {f1, f3, f4, ff}
	pthread_mutex_unlock(&lockA);
	ff(); f3(); // (f4, ff)
	pthread_mutex_unlock(&lockB);
}


int main(void)
{
	if (pthread_mutex_init(&lockA, NULL)) return 1;
	if (pthread_mutex_init(&lockB, NULL)) return 1;

	atomic_sequences();

	test();
	test_only_lock();
	test_only_unlock();
	test_nested();

	pthread_mutex_destroy(&lockA);
	pthread_mutex_destroy(&lockB);

	return 0;
}
