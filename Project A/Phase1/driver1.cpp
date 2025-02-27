#include "BankAccount1.cpp"
#include <thread>

//Task one for depositing into bank account
void task1(BankAccount &account, double amount){
    std::cout << "You are withdrawing $" << amount << "\n";
    account.withdraw(amount);
}

//Task two for witdrawing from bank account
void task2(BankAccount &account, double amount){
    std::cout << "You are depositing $" << amount << "\n";
    account.deposit(amount);
}

int main(){
    //intitialize bank account with starting balance of $100
    BankAccount pflegerAccount(100);

    //Output balance before thread operations
    std::cout << "Balance before thread operations: $" << pflegerAccount.get_balance() << "\n";

    std::thread thread1(task1, std::ref(pflegerAccount), 30); //Creation of thread1
    std::thread thread2(task2, std::ref(pflegerAccount), 22); //Creation of thread1

    //join both threads to ensure they finish before program terminates
    thread1.join();
    thread2.join();

    //Output balance after thread operations
    std::cout << "Balance after thread operations: $" << pflegerAccount.get_balance() << "\n";

    return 0;//return 0;
}