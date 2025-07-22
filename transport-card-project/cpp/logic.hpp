#ifndef LOGIC_HPP
#define LOGIC_HPP

#include "user.hpp"
#include <string>

class RecommendationEngine {
public:
    enum CardType { K_PASS, CLIMATE_CARD, GENERAL_CARD };

    static int unitFare(const User& user);
    static int climateCardCost(const User& user);
    static int calculateKPassRefund(const User& user);
    static CardType recommendCard(const User& user);
    static std::string cardName(CardType card);
};

extern "C" {
    const char* recommendCard(int age, int ridesPerWeek, bool isLowIncome, const char* transportType, int customFare);
    int getMonthlyCost(int age, int ridesPerWeek, bool isLowIncome, const char* transportType, int customFare);
    int getKPassSaving(int age, int ridesPerWeek, bool isLowIncome, const char* transportType, int customFare);
}

#endif