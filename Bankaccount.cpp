//
// Created by nikla on 22/05/2022.
//
//TODO interst Rate, etc...
#include <iomanip>
#include <iostream>
#include "Bankaccount.h"
int Bankaccount::_number = 9000;
Bankaccount::Bankaccount(float balance, int owner, int id) {
    _number = _number++;
    _owner = owner;
    _pinCode = Bankaccount::randomPIN();
    _id = id;
    _balance = balance;
    _statementRecords.size();
    _activities.size();
}

string Bankaccount::randomPIN() {
    srand((unsigned int) __TIME__);
    string pin;
    for (int i = 0; i < 4; ++i) {
        pin += pin + to_string((rand()%10));
    }
    cout<<pin<<endl;
    return pin;
}
bool Bankaccount::operator==(const Bankaccount &b) const {
    return (_id == b._id);
}
void Bankaccount::interestBalance(DateTime date) {
    //TODO
}

string Bankaccount::toString(){
    ostringstream os;
    os<<"Accountnumber: "<< _number << " User: " << _owner << " ID: "<<_id<<" Last activity: "<< _lastUpdate;
    return os.str();
};
int Bankaccount::getOwner() const {
    return _owner;
}

int Bankaccount::getID() {
    return _id;
}

void Bankaccount::addActivity(Activity activity) {
    _activities.push_back(&activity);
}
vector<Activity*>Bankaccount::getActivities(){
    return _activities;
}
//Bankaccount::~Bankaccount() = default;

void Giro::payIn(float amount, DateTime d) {
    _balance += amount;
    ostringstream os;
    os << setw(17) << d.toString("en_GB.UTF8") << "|" << setw(26) << "|" << setw(26) << "|" << "+" << setw(24) << amount
       << "|" << setw(25);
    //if (_balance < 0) { os << "-"; }
    os << setw(20) << _balance << "|";
    _activities.push_back(new Activity("PayIn: ",amount,d));
    _statementRecords.push_back(os.str());
}


float Bankaccount::balance() {
    return _balance;
}

Giro::Giro(int owner, float startAmount,float dispolimit, float debitinterest) : Bankaccount() {

    _owner = owner;
    _balance = startAmount;
    _dispoLimit = dispolimit;
    _debitInterest = debitinterest;
    _activities.push_back(new Activity("Account created"));
}

//Giro::~Giro() = default;

void Giro::withdrawl(float amount, DateTime d) {
    try {
        if ((_balance - amount) < _dispoLimit) {
            throw runtime_error("Your dispolimit has been exeeded!");
        }
        _balance -= amount;
        ostringstream os;
        os << setw(17) << d.toString("en_GB.UTF8") << "|" << setw(26) << "|" << setw(26) << "|" << "-" << setw(24)
           << amount << "|" << setw(25);
        os << setw(20) << _balance << "|";
        _statementRecords.push_back(os.str());
        _activities.push_back(new Activity("Withdrawl: ",amount,d));
    } catch (runtime_error &e) {
        std::cerr << e.what() << endl;
    }
}

void Giro::transfer(float amount, string accountnumber, string discription, DateTime d) {
    try {
        if (_balance - amount <= 0) {
            throw runtime_error("Your balance cant be negative!");
        }
        _balance -= amount;

        ostringstream os;
        os << setw(17) << d.toString("en_GB.UTF8") << "|" << setw(25) << discription << "|" << setw(25) << accountnumber
           << "|" << "-" << setw(24) << amount << "|" << setw(25);
        os << setw(20) << _balance << "|";
        _activities.push_back(new Activity(("Transfer, "+discription+" : "),amount,d));
        _statementRecords.push_back(os.str());
    } catch (runtime_error &e) {
        std::cerr << e.what() << endl;
    }
}

string Giro::statement() {
    ostringstream statement;
    statement << setw(17) << "Date" << "|" << setw(25) << "Discription" << "|" << setw(25) << "Receiver" << "|"
              << setw(25) << "Withdrawl/Deposit" << "|";
    statement << setw(20) << "Balance" << "|" << "\n";
    for (auto &record: _statementRecords) {
        statement << record << "\n";
    }
    ostringstream balance_setprecision4;
    balance_setprecision4 << _balance << setprecision(4);
    string currentBalance = "Current Balance: " + balance_setprecision4.str();
    statement << setw(116) << "==========" << "|" << "\n" << setw(116) << currentBalance << "|" << "\n";
    return statement.str();
}

Savingsaccount::Savingsaccount(int owner, float startAmount, float interestRate) : Bankaccount() {
    _interestRate = interestRate;
    _owner = owner;
    _balance = startAmount;
    _activities.push_back(new Activity("Account created"));
}

void Savingsaccount::withdrawl(float amount, DateTime d) {
    try {
        if (_balance - amount <= 0) {
            throw runtime_error("Your balance cant be negative!");
        }
            _balance -= amount;
        ostringstream os;
        os << setw(17) << d.toString("en_GB.UTF8") << "|" << "-" << setw(24) << amount << "|" << setw(25);
        os << setw(20) << _balance;
        _activities.push_back(new Activity("Withdrawl: ",amount,d));
        _statementRecords.push_back(os.str());
    } catch (runtime_error &e) {
        std::cerr << e.what() << endl;
    }
}

string Savingsaccount::statement() {
    ostringstream statement;
    statement << setw(17) << "Date" << "|" << setw(25) << "Withdrawl/Deposit" << "|";
    statement << setw(20) << "Balance" << "\n";
    for (auto &record: _statementRecords) {
        statement << record << "\n";
    }
    ostringstream balance_setprecision4;
    balance_setprecision4 << _balance << setprecision(4);
    string currentBalance = "Current Balance: " + balance_setprecision4.str();
    statement << setw(64) << "==========" << "\n" << setw(64) << currentBalance << "\n";
    return statement.str();
}

void Savingsaccount::payIn(float amount, DateTime d) {
    _balance += amount;
    ostringstream os;
    os << setw(17) << d.toString("en_GB.UTF8") << "|" << "+" << setw(24) << amount << "|" << setw(25);
    os << setw(20) << _balance;
    _activities.push_back(new Activity("PayIn: ",amount,d));
    _statementRecords.push_back(os.str());
}

