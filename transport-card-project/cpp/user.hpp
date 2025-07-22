#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <iostream>

class User {
private:
    int age;
    int ridesPerWeek;
    bool isLowIncome;
    std::string transportType;
    int customFare;

public:
    User(int age, int ridesPerWeek, bool isLowIncome, const std::string& transportType, int customFare);

    int getAge() const;
    int getMonthlyRides() const;
    bool getLowIncomeStatus() const;
    std::string getTransportType() const;
    int getCustomFare() const;
    int getFarePerRide() const;
    void printInfo() const;
};

#endif