/**
 * C++ version of Java's original at
 * https://github.com/trxsys/gluon
 *
 * @Author VeriFIT group at www.fit.vutbr.cz (smrcka@fit.vutbr.cz)
 * @Date 2016-03-08
 */

#include <thread> // thread
#include <atomic> // atomic

class Cell {
    std::atomic<int> n;

public:
    Cell() : n(0) {}

    //@Atomic
    int getValue() {
        return n;
    }

    // @Atomic
    void setValue(int x) {
        n = x;
    }
};

Cell x;

void run() {
    int tmp;
    tmp = x.getValue();
    tmp++;
    x.setValue(tmp);
}

int main()
{
    std::thread t1{run}, t2{run};

    t1.join();
    t2.join();

    return 0;
}
