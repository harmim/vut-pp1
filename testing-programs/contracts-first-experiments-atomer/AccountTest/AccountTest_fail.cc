/**
 * C++ version of Java's original at
 * https://github.com/trxsys/gluon
 *
 * @Author VeriFIT group at www.fit.vutbr.cz (smrcka@fit.vutbr.cz)
 * @Date 2016-03-08
 */

#include <thread> // thread
#include <string> // string
#include <mutex> // mutex
#include <atomic> // atomic
#include <cstdlib> // rand()

class Account {
private:
    Account(const Account&) {}

protected:
    std::atomic<int> balance;
    std::string name;

    std::mutex mtx;

public:
    Account(int balance = 0, std::string name = "") {
        this->balance = balance;
        this->name = name;
    }

    std::string getName() {
        std::lock_guard<std::mutex> lock(mtx);
        return name;
    }

    // @Atomic
    int getBalance() {
        return balance;
    }

    // @Atomic
    void setBalance(int newValue) {
        balance = newValue;
    }
};

class Update {
    Account &account;

public:
    Update(Account &account) : account(account) {}

    void update(int a) {
        int tmp = account.getBalance();
        tmp = tmp + a;
        account.setBalance(tmp);
    }

    void operator()() {
        int nupdates = std::rand() % 100;
        for (int i = 0; i < nupdates; i++) {
            // generate random amount
            int n = rand()%200 - 100;
            update(n);
        }
    }
};

int main()
{
    Account a{0, "account name"};

    std::thread t1{Update{a}};
    std::thread t2{Update{a}};

    t1.join();
    t2.join();

    return 0;
}
