// WebAssembly 모듈이 초기화되면 실행
Module().then((Module) => {

  // '추천받기' 버튼 클릭 시 동작할 함수 지정
  document.getElementById('recommendBtn').onclick = () => {

    // 사용자 입력값 가져오기
    const age = parseInt(document.getElementById('age').value);              // 나이
    const usage = parseInt(document.getElementById('usage').value);          // 일주일 교통 이용 횟수
    const isLowIncome = document.getElementById('lowIncome').value === 'yes' ? 1 : 0;  // 저소득 여부
    const transportType = document.getElementById('mode').value;             // 선택한 교통수단
    const customFare = transportType === 'custom'
      ? parseInt(document.getElementById('customFare').value || 0)           // 직접입력 요금
      : 0;

    // 추천 카드 이름을 받아오기 (C++ recommendCard 함수 호출)
    const ptr = Module.ccall(
      'recommendCard',
      'number',
      ['number', 'number', 'number', 'string', 'number'],
      [age, usage, isLowIncome, transportType, customFare]
    );
    const cardName = Module.UTF8ToString(ptr);  // 메모리 주소 → 문자열 변환

    // 월 교통비 계산 (C++ getMonthlyCost 함수 호출)
    const monthlyCost = Module.ccall(
      'getMonthlyCost',
      'number',
      ['number', 'number', 'number', 'string', 'number'],
      [age, usage, isLowIncome, transportType, customFare]
    );

    // K-Pass 절감액 계산 (C++ getKPassSaving 함수 호출)
    const kpassSave = Module.ccall(
      'getKPassSaving',
      'number',
      ['number', 'number', 'number', 'string', 'number'],
      [age, usage, isLowIncome, transportType, customFare]
    );

    // 기후동행카드 절감액 계산 (C++ getClimateSaving 함수 호출)
    const climateSave = Module.ccall(
      'getClimateSaving',
      'number',
      ['number', 'number', 'number', 'string', 'number'],
      [age, usage, isLowIncome, transportType, customFare]
    );

    // 추천 카드에 따라 외부 링크 생성
    let link = "";
    if (cardName.includes("K-Pass")) {
      link = `<a href="https://korea-pass.kr/" target="_blank">👉 K-Pass 신청하러 가기</a>`;
    } else if (cardName.includes("기후동행")) {
      link = `<a href="https://news.seoul.go.kr/traffic/archives/510651" target="_blank">🌱 기후동행카드 안내 보기</a>`;
    }

    // 기후동행카드 절감액이 음수면 "추천되지 않음" 문구 표시
    const climateText = climateSave > 0
      ? `🌿 기후동행카드를 쓴다면 <strong>${climateSave.toLocaleString()}원</strong>을 절약할 수 있어요!`
      : `🌿 기후동행카드는 이 경우 추천되지 않아요.`;

    // 결과 영역 표시
    const result = document.getElementById('result');
    result.style.display = 'block';
    result.innerHTML = `
      <div style="background:#fff3cd; border: 2px dashed #f0ad4e; padding: 20px; border-radius: 15px;">
        <h2>✨ 결과 나왔어요! ✨</h2>
        <p>💳 <strong>${cardName}</strong> 카드가 딱이에요!</p>
        <p>🚌 당신의 월 교통비는 <strong>${monthlyCost.toLocaleString()}원</strong> 정도예요.</p>
        <p>🅺 K-Pass를 쓴다면 <strong>${kpassSave.toLocaleString()}원</strong>을 아낄 수 있어요!</p>
        <p>${climateText}</p>
        <p style="margin-top:10px; font-size:0.9em;">* 계산은 대략적인 예시입니다! 😉</p>
        <br>${link}
      </div>
    `;
  };
});