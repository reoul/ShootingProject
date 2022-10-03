# ShootingProject

## 프로젝트 소개

- 학교에서 기본 제공 2D 슈팅 소스(1945같은 게임)를 기준으로 발전시킨 프로젝트
- DirectX9 과 Win32API 를 사용한 슈팅 게임
- 프로젝트 인원 : 1명

## 프로젝트 기간

- 2019.09 ~ 2019.10 처음 프로젝트 기간
- 2021.12 ~ 2021.12 코드 리팩토링 및 확장
- 2022.09 ~ 2022.09 전체적인 구조 변경 및 코드 리팩토링

## 플레이 영상

- https://www.youtube.com/watch?v=WfoJj2R4Fv0

## 다양한 시도 및 구현

- 고정된 화면에서 플레이어 위치 기준으로 주변을 보여주게 그려주는 로직을 변경
- 타일맵에디터가 있는줄 모르고 직접 타일맵에디터 비슷한 기능 구현
  - 구현했고 처음에는 사용했는데 나중에 `Tiled` 라는 타일맵에디터를 사용했음
- 이미지를 그릴때 각도를 돌려서 그려주는 함수를 개발 당시에 못찾았기에 이미지 회전 공식을 인터넷에서 참조해 구현
- 플레이어 위치와 사물 위치를 비교하면서 사물 근처에 가면 소리가 점점 커지게 구현
- 화살을 매번 생성하지 않고 오브젝트 풀링을 통해 성능 최적화

## 문제와 해결법

- 이미지를 적게 읽어올때는 별로 안걸렸는데 점차 개발하면서 이미지가 늘어나다 보니까 **로딩이 느려짐**
  - 원인 : 이미지 파일을 읽어오는 함수에서 픽셀 하나씩만 읽어오느라 로딩이 오래 걸렸음
  - 해결법 : 이미지 파일을 읽어오는 함수에서 픽셀 **하나씩 읽어오는 로직**을 **가로 한줄씩 로딩**하는 걸로 변경

<br>

- 타일을 그려줄 때 타일 데이터를 가진 파일을 읽어와서 타일 하나하나 다 그려주려고 했는데 런타임시 5프레임도 안 나옴
  - 원인 : `1920*1080` 기준 `32*32` 타일은 1프레임에 대략 `1980`번 그려주다 보니 프레임이 엄청 떨어졌었음
  - 해결법 : `Tiled` 라는 타일맵에디터를 사용해 **전체 맵을 뽑아서 한번에 그려주니** 프레임이 좋아졌음

<br>

- FadeIn 과 FadeOut 기능을 구현해서 호출했는데 엄청 느렸음
  - 원인 : 메인 쓰레드에서만 `1920*1080` 화면 픽셀을 알파블렌딩 계산을 해서 느렸음
  - 해결법 : 멀티쓰레딩을 사용해서 **연산을 분산**시킴

## 코드 부분

- 게임 플레이 로직
https://github.com/reoul/ShootingProject/blob/1049c92e745b16a88ae119f450da601b10a135cc/MapEditor/GameMain.cpp#L145-L407

<>

- 화살이 타겟에게 향하는 방향과 속도를 계산 (유도화살)
https://github.com/reoul/ShootingProject/blob/07b8c606ff9949cfa1d4cae390ab698300b8dc56/MapEditor/Arrow.cpp#L176-L201

<br>

- BMP 파일을 읽어오는 기능
https://github.com/reoul/ShootingProject/blob/cee2ee6071edf9c8628d31a08b84aac14f5d506a/MapEditor/Bmp.cpp#L14-L129

<br>

- 픽셀 버퍼에서 표면으로 복사
https://github.com/reoul/ShootingProject/blob/cee2ee6071edf9c8628d31a08b84aac14f5d506a/MapEditor/Bmp.cpp#L131-L163

<br>

- 보스 상태에 따른 이미지 변경
https://github.com/reoul/ShootingProject/blob/d144aae27dc55bb3e419b3c228d3ff0d1df88ca0/MapEditor/Boss.cpp#L215-L279

<br>

- 보스의 방향을 결정
https://github.com/reoul/ShootingProject/blob/8b6d0bf01c018b37e47c9804a2c3a7df734f8537/MapEditor/Boss.cpp#L173-L207

<br>

- 보스 다음 패턴 설정
https://github.com/reoul/ShootingProject/blob/d144aae27dc55bb3e419b3c228d3ff0d1df88ca0/MapEditor/Boss.cpp#L337-L368

<br>

- 화면 알파 블렌딩과 알파 블렌딩 쓰레드 워커 함수
https://github.com/reoul/ShootingProject/blob/6c755d036a1f41df3d5a38e6934a91f33e05ea8a/MapEditor/Camera.cpp#L216-L300

<br>

- 게임오브젝트 화면에 그려주는 기능
https://github.com/reoul/ShootingProject/blob/5fe1630e7abb83e92f8f2fcb2b245f20eaf68477/MapEditor/GameObject.cpp#L55-L129

<br>

- 플레이어 벽 충돌 확인
https://github.com/reoul/ShootingProject/blob/634d9d7fe5597141fde0bc1f2f51adf9500cf76d/MapEditor/Player.cpp#L198-L476

<br>

- 플레이어 키 입력 감지
https://github.com/reoul/ShootingProject/blob/634d9d7fe5597141fde0bc1f2f51adf9500cf76d/MapEditor/Player.cpp#L496-L535

<br>

- 플레이어 이동과 상태 업데이트
https://github.com/reoul/ShootingProject/blob/634d9d7fe5597141fde0bc1f2f51adf9500cf76d/MapEditor/Player.cpp#L629-L806

<br>

- 이미지 그려주는 함수
https://github.com/reoul/ShootingProject/blob/762c5b5a5a5074ac6a77d1b341eef65c5c25e058/MapEditor/Sprite.cpp#L112-L188

<br>

- 이미지 회전 함수
https://github.com/reoul/ShootingProject/blob/762c5b5a5a5074ac6a77d1b341eef65c5c25e058/MapEditor/Sprite.cpp#L492-L529

<br>

- 시간 관련 함수
https://github.com/reoul/ShootingProject/blob/9729c2be6539cc804ec1aa641b2f34ff2368f126/MapEditor/Timer.cpp#L19-L45

<br>

- 벡터를 각도로 변환
https://github.com/reoul/ShootingProject/blob/48835339e06e4a3dab7884ccd82ce53807bd21b4/MapEditor/Vector2.cpp#L45-L70
