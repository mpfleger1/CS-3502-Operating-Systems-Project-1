//Bank Account Class
#include <iostream>
#include <mutex>
#include <chrono>
#include <thread>
#include <vector>
class BankAccount{
    private:
        double balance; //Shared resource between threads
        int id; //Unqiue id for each account
        std::mutex mtx; //mutex lock for security
    public:
        //BankAccount constructor
        BankAccount(double initital_balance, int user_id){
            balance = initital_balance;
            id = user_id;
        }
        //BankAccount destructor
        ~BankAccount(){}

        //deposit method
        bool deposit(double amount){
            //Attempt to lock mutex (if fails mutex already locked) avoid deadlock
            if(mtx.try_lock()){
                if(amount <= 0){
                    std::cout << "ERROR: deposit amount must be greater than 0\n";
                    return false;
                }
                else{
                    balance += amount;
                    mtx.unlock();//Explicitly unlock
                }
            }
            return true;    
        }
        //withdraw method
        bool withdraw(double amount){
            //Attempt to lock mutex (if fails mutex already locked) avoid deadlock
            if(mtx.try_lock()){
                if(amount > balance){
                    std::cout << "ERROR: Insufficient Funds\n";
                    return false;
                }
                else{
                    balance -= amount;
                }
                mtx.unlock();//Explicitly unlock
            }
            return true;
        }
        
        //get_balance method
        double get_balance(){
            //Attempt to lock mutex (if fails mutex already locked) avoid deadlock
            if(mtx.try_lock()){
                return balance;
                mtx.unlock();//Explicitly unlock
            }
            return 0;
        }
        int get_id(){
            return id;
        }
        static void transfer(BankAccount& sendingAccount, BankAccount& receivingAccount, double amount) {
            // Check if sending and receiving accounts are the same
            if (&sendingAccount == &receivingAccount) {
                std::cout << "ERROR: Cannot transfer between the same account\n";
                return;
            }
        
            // Determine the lock order without affecting sender/receiver roles
            BankAccount* first = &sendingAccount;
            BankAccount* second = &receivingAccount;
        
            if (sendingAccount.get_id() > receivingAccount.get_id()) {
                std::swap(first, second);
            }
        
            std::unique_lock<std::mutex> lock1(first->mtx, std::defer_lock);
            std::unique_lock<std::mutex> lock2(second->mtx, std::defer_lock);
        
            // Lock both mutexes simultaneously to avoid deadlocks
            std::lock(lock1, lock2);
        
            // Perform the transfer correctly using the original sender/receiver references
            if (sendingAccount.balance >= amount) {
                sendingAccount.balance -= amount;
                receivingAccount.balance += amount;
                std::cout << "Transfer complete\n";
            } else {
                std::cout << "ERROR: Insufficient funds for transfer\n";
            }
        }
        
};

int main(){
    BankAccount account1(1356.88, 1234);
    BankAccount account2(1245.87, 4321);
    BankAccount account3(2234.12, 5678);
    BankAccount account4(987.66, 8765);
    BankAccount account5(500.91, 9876);

    std::vector<std::thread> threads;
    threads.push_back(std::thread(&BankAccount::transfer, std::ref(account1), std::ref(account2), 100));
    threads.push_back(std::thread(&BankAccount::transfer, std::ref(account2), std::ref(account3), 200));
    threads.push_back(std::thread(&BankAccount::transfer, std::ref(account3), std::ref(account4), 300));
    threads.push_back(std::thread(&BankAccount::transfer, std::ref(account4), std::ref(account5), 400));
    threads.push_back(std::thread(&BankAccount::transfer, std::ref(account5), std::ref(account1), 500));
    
    threads.push_back(std::thread(&BankAccount::deposit, std::ref(account1), 100));
    threads.push_back(std::thread(&BankAccount::deposit, std::ref(account2), 200));
    threads.push_back(std::thread(&BankAccount::deposit, std::ref(account3), 300));
    threads.push_back(std::thread(&BankAccount::deposit, std::ref(account4), 400));
    threads.push_back(std::thread(&BankAccount::deposit, std::ref(account5), 500));

    threads.push_back(std::thread(&BankAccount::withdraw, std::ref(account1), 100));
    threads.push_back(std::thread(&BankAccount::withdraw, std::ref(account2), 200));
    threads.push_back(std::thread(&BankAccount::withdraw, std::ref(account3), 300));
    threads.push_back(std::thread(&BankAccount::withdraw, std::ref(account4), 400));
    threads.push_back(std::thread(&BankAccount::withdraw, std::ref(account5), 500));

    for(auto& thread : threads){
        thread.join();
    }

    std::cout << "Account 1 balance: $" << account1.get_balance() << std::endl;
    std::cout << "Account 2 balance: $" << account2.get_balance() << std::endl;
    std::cout << "Account 3 balance: $" << account3.get_balance() << std::endl;
    std::cout << "Account 4 balance: $" << account4.get_balance() << std::endl;
    std::cout << "Account 5 balance: $" << account5.get_balance() << std::endl;

    return 0;
}