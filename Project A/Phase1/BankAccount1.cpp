//Bank Account Class
#include <iostream>
class BankAccount{
    private:
        double balance; //Shared resource between threads
    public:
        //BankAccount constructor
        BankAccount(double intitial_balance){
            balance = intitial_balance;
        }
        //BankAccount destructor
        ~BankAccount(){}

        //deposit method
        bool deposit(double amount){
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
            return balance;
        }
};