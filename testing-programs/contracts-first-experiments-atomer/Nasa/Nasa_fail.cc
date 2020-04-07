/**
 * C++ version of Java's original at
 * https://github.com/trxsys/gluon
 * revisited wrt. Artho. High-Level Data Races. 2003:
 * http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.13.7296
 *
 * @Author VeriFIT group at www.fit.vutbr.cz (smrcka@fit.vutbr.cz)
 * @Date 2016-03-09
 */

#include <thread> // thread
#include <mutex> // mutex
#include <atomic> // atomic
#include <cstdlib> // rand

// simple syntactic sugar
#define synchronized std::lock_guard<std::mutex> lock

class Cell  {
public:
    int value;
    bool achieved;

    Cell(int value = 0, bool achieved = false) :
        value(value), achieved(achieved) {}
};

const int NPROPERTIES = 100;

class LockTable {
public:
    std::mutex mtx;

    Cell cells[NPROPERTIES];

    Cell &operator[](int property) {
        return cells[property];
    }
};

class TaskManager {
protected:
    LockTable &table;

public:
    TaskManager(LockTable &table) : table(table) {}

    // @Atomic
    void setValue(int v, int N) {
        synchronized{table.mtx};
        table[N].value = v;
    }

    // @Atomic
    void setAchieved(int N) {
        synchronized{table.mtx};
        table[N].achieved = true;
    }

    // @Atomic
    void unsetAchieved(int N) {
        synchronized{table.mtx};
        table[N].achieved = false;
    }

    void lock_table() {
        table.mtx.lock();
    }

    void unlock_table() {
        table.mtx.unlock();
    }

};

class Task {
protected:
    TaskManager &tm;

public:
    Task(TaskManager &tm) : tm(tm) {}

    void operator()() {
        /* select property */
        int N = 42;
        /* set requested value */
        int v = std::rand();

        //tm.lock_table();

        /* register property value */
        tm.setValue(v, N);
        /* achieve property */
        tm.setAchieved(N);

        //tm.unlock_table();

        /* wait for unspecified amount of time ... */
        tm.unsetAchieved(N);
    }
};

class SystemState {
public:
    int values[NPROPERTIES];
};

class Daemon {
protected:
    LockTable &table;
    SystemState &system_state;

public:
    Daemon(LockTable &table, SystemState &system_state) :
        table(table), system_state(system_state) {}

    void operator()() {
        // check consistency of the whole table
        for (int prop = 0; prop < NPROPERTIES; prop++) {
            tryIssueWarning(prop);
        }

        /*
        // simplified version for the experiment
        tryIssueWarning(42);
        */
    }

private:
    // @Atomic
    void tryIssueWarning(int N) {
        synchronized{table.mtx};
        if (table[N].achieved && system_state.values[N] != table[N].value)
            issueWarning();
    }

    void issueWarning() {
        throw -42;
    }
};

int main()
{
    LockTable table;
    SystemState system_state;
    TaskManager tm{table};
    Task task{tm};

    std::thread t1{task};
    /*std::thread t2{task};
    std::thread t3{task};*/

    std::thread daemon{Daemon{table, system_state}};

    t1.join();
    /*t2.join();
    t3.join();*/
    daemon.join();

    return 0;
}
