/**
 * C++ version of Java's original at
 * https://github.com/trxsys/gluon
 *
 * @Author VeriFIT group at www.fit.vutbr.cz (smrcka@fit.vutbr.cz)
 * @Date 2016-03-08
 */

#include <thread> // thread
#include <mutex> // mutex
#include <atomic> // atomic

#define synchronized std::lock_guard<std::mutex> lock

class Coord {
    static std::mutex mtx;
    int x, y;
public:
    Coord() {
        x=y=0;
    }

    // @Atomic
    void swap() {
        synchronized{mtx};
        int oldX = x;
        x = y; // swap X
        y = oldX; // swap Y
    }

    // @Atomic
    void resetX(){
        synchronized{mtx};
        x = 0;
    }

    // @Atomic
    void resetY(){
        synchronized{mtx};
        y = 0;
    }
};

std::mutex Coord::mtx;

std::mutex resetMtx;

class CoordMain {

    Coord &coord;

public:
    CoordMain(Coord &coord) : coord(coord) {}

    void operator()() {
        coord.swap();
        reset();
    }

    void reset() {
        synchronized{resetMtx};
        coord.resetX();
        // inconsistent state (0, y)
        coord.resetY();
    }
};

int main()
{
    Coord c;

    std::thread t1{CoordMain{c}};
    std::thread t2{CoordMain{c}};
    std::thread t3{CoordMain{c}};
    std::thread t4{CoordMain{c}};

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
