//Bank Account Class
#include <iostream>
#include <mutex>
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
};