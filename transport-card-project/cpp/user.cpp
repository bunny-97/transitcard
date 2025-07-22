#include "user.hpp"

// User 클래스 생성자: 모든 멤버 변수 초기화
User::User(int age, int ridesPerWeek, bool isLowIncome, const std::string& transportType, int customFare)
    : age(age), ridesPerWeek(ridesPerWeek), isLowIncome(isLowIncome),
      transportType(transportType), customFare(customFare) {}

// 나이 반환
int User::getAge() const {
    return age;
}

// 주간 이용 횟수를 기준으로 월 이용 횟수 계산 (4.3배수 적용)
int User::getMonthlyRides() const {
    return static_cast<int>(ridesPerWeek * 4.3);
}

// 저소득층 여부 반환
bool User::getLowIncomeStatus() const {
    return isLowIncome;
}

// 사용자가 선택한 교통수단 종류 반환 ("subway", "bus", "town", "custom")
std::string User::getTransportType() const {
    return transportType;
}

// 사용자 입력 기반의 커스텀 요금 반환
int User::getCustomFare() const {
    return customFare;
}

// 실제 1회 이용 요금 계산
int User::getFarePerRide() const {
    // 직접 입력인 경우 해당 요금 사용
    if (transportType == "custom") return customFare;

    // 청소년 요금
    if (age < 19) {
        if (transportType == "subway") return 900;
        if (transportType == "bus") return 800;
        if (transportType == "town") return 600;
    }
    // 일반 요금
    else {
        if (transportType == "subway") return 1500;
        if (transportType == "bus") return 1400;
        if (transportType == "town") return 1200;
    }
    // 정의되지 않은 경우 기본값 0
    return 0;
}

// 디버깅용 사용자 정보 출력 함수
void User::printInfo() const {
    std::cout << "나이: " << age << ", 주당 이용 횟수: " << ridesPerWeek
              << ", 저소득층 여부: " << (isLowIncome ? "예" : "아니오")
              << ", 교통수단: " << transportType
              << ", 직접입력 요금: " << customFare << std::endl;
}