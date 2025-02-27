#include "BankAccount3.cpp"
#include <thread>

//task1 to transfer from pflegerAccount to johnsonAccount
void task1(BankAccount& acc1, BankAccount& acc2) {
    BankAccount::transfer(acc1, acc2, 100);
}
//task2 to transfer from johnsonAccount to pflegerAccount
void task2(BankAccount& acc1, BankAccount& acc2) {
    BankAccount::transfer(acc2, acc1, 50);
}

int main(){

    //Creation of two test bank accounts
    BankAccount pflegerAccount(500);
    BankAccount johnsonAccount(500);

    //Thread creation
    std::thread thread1(task1, std::ref(pflegerAccount), std::ref(johnsonAccount));
    std::thread thread2(task2, std::ref(pflegerAccount), std::ref(johnsonAccount));

    //Thread join
    thread1.join();
    thread2.join();

    //Outout the results
    std::cout << "Pfleger Account Balance: $" << pflegerAccount.get_balance() << "\n";
    std::cout << "Johnson Account Balance: $" << johnsonAccount.get_balance() << "\n";
}

