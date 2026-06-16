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

