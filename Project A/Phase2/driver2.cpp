#include "BankAccount1.cpp"
#include <thread>
#include <vector>

//Method to simulate multiple deposits
void multipleDeposits(BankAccount &account, double amount, int run_time){
    for(int i = 0; i < run_time; i++){
        account.deposit(amount);
    }
}

int main(){
    //Initialize bank account with a starting balance of $0
    BankAccount pflegerAccount(0);

    //Use vector to hold 8 threads to run multipleDeposit task
    std::vector<std::thread> threads;
    for(int i = 0; i < 8; i++){
        threads.push_back(std::thread(multipleDeposits, std::ref(pflegerAccount), 1, 100000));
    }

    //join all of the threads
    for(auto&t : threads){
        t.join();
    }

    //Output the results
    std::cout << "Expected Balance: $800000\n";
    std::cout << "Actual Balance: $" << pflegerAccount.get_balance() << "\n";
    
    return 0;
}