//Bank Account Class
#include <iostream>
#include <mutex>
#include <chrono>
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