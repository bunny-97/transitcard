#include "logic.hpp"
#include <algorithm>
#include <cstring>

// 1회 요금 반환
int RecommendationEngine::unitFare(const User& user) {
    return user.getFarePerRide();
}

// 기후동행카드 비용
int RecommendationEngine::climateCardCost(const User& user) {
    return (user.getAge() >= 19 && user.getAge() <= 39) ? 55000 : 62000;
}

// K-Pass 환급액 계산
int RecommendationEngine::calculateKPassRefund(const User& user) {
    if (user.getAge() < 19) return 0;

    int usage = user.getMonthlyRides();
    int validUsage = std::min(usage, 60); // 최대 60회
    validUsage = std::min(validUsage, 2 * 30); // 하루 2회 기준으로도 제한

    int rate = 0;
    if (user.getLowIncomeStatus()) rate = 53;
    else if (user.getAge() <= 34) rate = 30;
    else rate = 20;

    return validUsage * unitFare(user) * rate / 100;
}

// 추천 카드 결정
RecommendationEngine::CardType RecommendationEngine::recommendCard(const User& user) {
    int usage = user.getMonthlyRides();
    int fare = unitFare(user);
    int totalCost = usage * fare;

    int kpassRefund = calculateKPassRefund(user);
    int kpassNetCost = totalCost - kpassRefund;

    int climateCost = climateCardCost(user);

    if (usage < 15 || user.getAge() < 19) {
        return (climateCost < totalCost) ? CLIMATE_CARD : GENERAL_CARD;
    }

    if (kpassNetCost <= climateCost && kpassNetCost <= totalCost) return K_PASS;
    if (climateCost <= kpassNetCost && climateCost <= totalCost) return CLIMATE_CARD;
    return GENERAL_CARD;
}

// 카드 이름 반환
std::string RecommendationEngine::cardName(CardType card) {
    switch (card) {
        case K_PASS: return "K-Pass 카드";
        case CLIMATE_CARD: return "기후동행카드";
        case GENERAL_CARD: return "일반 교통카드";
        default: return "알 수 없음";
    }
}

// 외부 연동 함수
extern "C" {

// 추천 카드
const char* recommendCard(int age, int ridesPerWeek, bool isLowIncome, const char* transportType, int customFare) {
    static std::string result;
    User user(age, ridesPerWeek, isLowIncome, transportType, customFare);
    RecommendationEngine::CardType card = RecommendationEngine::recommendCard(user);
    result = RecommendationEngine::cardName(card);
    return result.c_str();
}

// 월 교통비
int getMonthlyCost(int age, int ridesPerWeek, bool isLowIncome, const char* transportType, int customFare) {
    User user(age, ridesPerWeek, isLowIncome, transportType, customFare);
    return user.getMonthlyRides() * user.getFarePerRide();
}

// K-Pass 절약액
int getKPassSaving(int age, int ridesPerWeek, bool isLowIncome, const char* transportType, int customFare) {
    User user(age, ridesPerWeek, isLowIncome, transportType, customFare);
    return RecommendationEngine::calculateKPassRefund(user);
}

// 기후동행카드 절약액
int getClimateSaving(int age, int ridesPerWeek, bool isLowIncome, const char* transportType, int customFare) {
    User user(age, ridesPerWeek, isLowIncome, transportType, customFare);
    int totalCost = user.getMonthlyRides() * user.getFarePerRide();
    int climate = RecommendationEngine::climateCardCost(user);
    return totalCost - climate;
}

}