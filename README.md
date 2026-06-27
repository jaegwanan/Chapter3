# Unreal Engine C++ Assignment

## 과제 3 - Enemy Tank 구현

### 구현 내용

Enemy Tank를 3가지 타입으로 분리하여 C++로 구현하였습니다.

#### 1. Enemy_Moving

* 지정된 범위 내에서 단순 이동
* 이동 기능만 구현

#### 2. Enemy_Turret

* 제자리에서 포탑(Turret) 회전
* 회전 기능만 구현

#### 3. Enemy_Both

* 지정된 범위 내 이동
* 플레이어 방향을 추적하여 포탑 회전
* 이동 및 회전 기능 동시 구현

### 클래스 구성

```text
Enemy_Moving
 └─ 범위 내 이동 구현

Enemy_Turret
 └─ 포탑 회전 구현

Enemy_Both
 ├─ 범위 내 이동 구현
 └─ 플레이어 추적 포탑 회전 구현
```

---

## 과제 4 - 플레이어 및 게임 모드 구현

### 게임 모드 제어

* `MyGameModeBase`

  * 게임 규칙 관리
  * 플레이어 Pawn 설정

* `MyPlayerController`

  * 플레이어 입력 처리
  * Pawn 제어

---

### 플레이어 클래스 구현

#### 1. Player_Soldier

* 이동 (Move)
* 점프 (Jump)

#### 2. Player_Tank

* 이동 (Move)
* 포신 회전 및 조절

#### 3. Player_Fighter

* 이동 (Move)
* 상승 (Ascend)
* 하강 (Descend)
* 활강 (Glide)

---

### 블루프린트 구성

모든 C++ 클래스는 블루프린트로 상속받아 최종적으로 사용하였습니다.

```text
C++ Class
    ↓
Blueprint Class
    ↓
Level 배치 및 사용
```

### 사용 기술

* Unreal Engine 5.7
* C++
* Blueprint
* GameMode
* PlayerController
* Pawn

### 참고사항
* 3인칭 Person베이스로 만들어서 기존의 언리얼에서 제공되는 파일들이 포함되어있습니다.
  위에 명시한 파일 위주로 보시면 구현한 부분에 대해서는 모두 볼 수 있습니다.

  ## 5번 과제 구현 사항

## Wave 1
- 인간형 캐릭터 조작
- 지뢰를 피해 이동
- 코인 10개 수집
- 제한 시간 내 목표 지점 도달

## Wave 2
- 탱크 조작
- 적 탱크 5대 격파
- 제한 시간 내 목표 지점 도달

## Wave 3
- 전투기 조작
- 큐브 10개 격발
- 코인 10개 수집
- 제한 시간 내 목표 지점 도달

---

# 구현 기술

- 간단한 사격 시스템
- Data Table을 이용한 랜덤 스폰
- 데미지 시스템
- 타임 리미트 UI 구성

---

# 참고 사항

- `Content` 폴더의 과제 관련 에셋은 **MyProject** 폴더에 정리되어 있습니다.
- `Source` 폴더에서는 **`My`가 붙은 클래스**가 직접 구현한 코드입니다.
- 그 외의 코드는 언리얼 엔진 기본 Third Person 템플릿에서 제공되는 코드이므로 확인하지 않으셔도 됩니다.
