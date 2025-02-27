//Bank Account Class
#include <iostream>
#include <mutex>
#include <chrono>
#include <thread>
class BankAccount{
    private:
        double balance; //Shared resource between threads
        std::mutex mtx;
    public:
        //BankAccount constructor
        BankAccount(double initital_balance){
            balance = initital_balance;
        }
        //BankAccount destructor
        ~BankAccount(){}

        //deposit method
        bool deposit(double amount){
            std::unique_lock<std::mutex> lock(mtx);
            if(amount <= 0){
                std::cout << "ERROR: deposit amount must be greater than 0\n";
                return false;
            }
            else{
                balance += amount;
            }
            return true;
        }
        //withdraw method
        bool withdraw(double amount){
            std::unique_lock<std::mutex> lock(mtx);
            if(amount > balance){
                std::cout << "ERROR: Insufficient Funds\n";
                return false;
            }
            else{
                balance -= amount;
            }
            return true;
        }
        //get_balance method
        double get_balance(){
            std::unique_lock<std::mutex> lock(mtx);
            return balance;
        }
        static void transfer (BankAccount& sendingAccount, BankAccount& receivingAccount, double amount){

            
            //Check if sending account and recieving account are the same
            if(&sendingAccount == &receivingAccount){
                std::cout << "ERROR: Can not transfer between the same account\n";
                return;
            }

            BankAccount *sender = &sendingAccount; //pointer to sender account
            BankAccount *receiver = &receivingAccount;//pointer to receiver account

            std::unique_lock<std::mutex> lock1(sender->mtx);

            std::this_thread::sleep_for(std::chrono::milliseconds(100000)); //simulate a delay to demonstarte deadlock

            std::unique_lock<std::mutex> lock2(receiver->mtx);

            if(sender->balance >= amount){
                sender->balance -= amount;
                receiver->balance += amount;
            }
            else{
                std::cout << "ERROR: Insufficient funds for transfer\n";
            }

        }
};