// Author: Dominik Harmim <iharmim@fit.vut.cz>

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;


class Locks
{
	private Lock lockA = new ReentrantLock(), lockB = new ReentrantLock();
	private Object monitor = null;


	private void f1() {}
	private void f2() {}
	private void f3() {}
	private void g() {}


	public void testLock()
	{
		lockA.lock(); // {f1, f2}
		f1(); f2();
		lockA.unlock();

		g();

		lockA.lock(); // {f1, f2, f3}
		{
			f1();
			lockB.lock(); // {f3}
			f3();
			lockB.unlock();
			f2();
		}
		lockA.unlock();
	}


	public synchronized void testSynchronizedMethod() // {f1, f2, f3}
	{
		f1();
		synchronized(this) // {f3}
		{
			f3();
		}
		f2();
	}


	public void testSynchronizedBlocks()
	{
		synchronized(this) // {f1, f2, f3}
		{
			f1();
			synchronized(monitor) // {f3}
			{
				f3();
			}
			f2();
		}
	}


	public void testViolations()
	{
		f1(); f2(); // (f1, f2)
	}
}


class Test
{
	public static void main(String[] args)
	{
		Locks locks = new Locks();

		locks.testLock();
		locks.testSynchronizedMethod();
		locks.testSynchronizedBlocks();

		locks.testViolations();
	}
}
