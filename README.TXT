Kick It UP! Version 0.4b

This program is Freeware, and Cannot be used for Commericial purpose.
이 프로그램은 프리웨어입니다, 그리고 상업적인 목적에 이용할 수 없습니다.

Agreement ---------------------------------------------------------
이 프로그램의 사용으로인한 어떤 사용상의 피해나 손상에대한 보상을 하지 않습니다.
-------------------------------------------------------------------

Warning -----------------------------------------------------------
This program is NOT Emulator!
이 프로그램은 에뮬레이터가 아닙니다!
-------------------------------------------------------------------

Copyright ---------------------------------------------------------
Copyright 1999-2000 (c) mahalo, the nogada worker, Amigo~*
Copyright      2000 (c) Hyung-seok Choi, the Graphics Designer.
-------------------------------------------------------------------

use keys ----------------------------------------------------------
For 1-player - Q E S Z C
FOr 2-player - 7 5 9 1 3
-------------------------------------------------------------------

Music Data --------------------------------------------------------

Kick It UP!
|
|--- Image
|--- Song
|     |
|     |----- Banya - Hypnosis
|     |----- CLON - Funky Tonight
|     |----- SES - Love
|     |----- 노바소닉 - 또다른 진심

디렉토리는 이런 형식으로 운영됩니다.
Song디렉토리안에 임의의 디렉토리를 만들어주고 데이터를 넣으면 되는것이죠.. ^^;

각 노래 데이터 디렉토리안의 구성은

    Song.wav or Song.mp3 -----> 음악 데이터. (wav)
    Back.bmp -----> 배경그림
    Disc.bmp   -----> 선곡시 표시될 디스크 그림입니다. 없으면 임의의 그림을 그려주지요.. 
    Intro.wav or Intro.mp3 -----> 선곡시 나올 인트로 음악입니다.
    Title.bmp  -----> 노래 로딩시에 나오는 이미지 입니다,
    Hard_1.ksf -----> 하드 데이터 파일입니다. (1인플레이)
    Hard_2.ksf -----> 하드 데이터 파일입니다. (2인플레이)
    Easy_1.ksf -----> 이지 데이터 파일입니다. (1인플레이)
    Easy_2.ksf -----> 이지 데이터 파일입니다. (2인플레이)
    Double.ksf -----> 더블 데이터 파일입니다.


Things changed since last version ---------------------------------

 version 0.4b - Minor Release (2000/07/25)
- [-]. Xaudio MP3 Play support removed.
- [-]. Background Movie play support removed. (for improve safety)

- [+]. DirectShow(in Direct Media) used for mp3 & wav playing. (In some systems, may slowly)
- [+]. Intro.mp3 for Intro song support.
- [+]. Fullscreen <-> Window mode Support.
-------------------------------------------------------------------


History -----------------------------------------------------------

 version 0.4a - 상처뿐인 영광 -_-;
- [+]. Improved Configuration. (Added Fusion Setting, Auto key for 2p, Judgement Picture On/Off,...)
- [+]. Added Sudden Random mode.. (79513579)
- [+]. Added Random Speed mode (Preliminary.. -_-) (찾아보시길. :p)

- [x]. Fixed bug in selectmode (Always Synchro mode :) )
- [x]. Fixed bug in Couple Mode. 
- [*]. 사운드 버그 다수일것으로 예상. (사운드 수정 작업중)

 version 0.4

- [+]. Background Movie file play support on game play.
- [+]. Disk moving in select song.

- [-]. Disabled some feature -_-;

- [x]. Fixed bug on 4d mode in Single mode. (now work)
- [x]. Fixed bug on select song. (not draw disk image)
- [x]. Fixed bug on playing game. (not finished when song end -_-;;)
- [x]. Fixed bug on select song. (고를 수 없는 곡이 있고, 고르면 다운되는 현상)
- [x]. 디렉토리중 하나가 비어있으면 로딩이 중단되는 버그 패치.
- [x]. 노래 시작시 찌꺼기 판정 출현 버그 패치.
- [x]. 전시회중 잡다 버그 패치.. -_-;;;


 version 0.3a - DDR Version or Fusion version.
- In DDR version: Arranged Down arrow to center button.
- In Fusion version: Support Pumpitup Fusion's Key setting.

- 2p모드 캔슬시 더블이 걸리던 문제 해결

 version 0.3 - 4월 마지막 버전.
- 메뉴판 삭제
- 환경설정용 키 배정.
- 오른쪽에서 시작해서 플레이가능.
- 더블을 커맨드 방식으로 변경.
- DDR장판 지원 (사용법은 알아서 찾아보시길..)
- 기타 잡다.

* 당분간 버그 패치를 제외한 업데이트는 없을예정입니다.

 version 0.2
//+ MP3 플레이 추가.
+ 게이지 추가.
+ 점수 추가.
+ 폭사 추가.
+ 환경설정 추가 (실험적)
+ 4차원 모드.
+ 배속 모드 (2~8배속)
+ 랜덤 모드
+ 유니온 모드
+ 미러 모드

x 판정 그림 바뀜.
x 잡다 버그 잡음

* 0.1 버전의 readme.txt 파일을 날려먹은 관계로 예전것에 덧붙입니다.. -_-;;

 version 0.004 (The Public Release.)
+ 게이지 테두리 추가. (테두리만)
+ 노래 끝날때 쓰레기값이 출력되던 버그 개선.
+ 화살표 색깔 순환버그 고침.

* 오토플레이는 더블시 오른쪽, 2인플레이시 오른쪽 방향에 들어있습니다.
  키 설정이 되지 않는관계로 넣은것입니다. 다음버전엔 개선하겠습니다.

known bugs: 노래 셀렉트시 문제가 발생합니다. 5곡이하일경우.
            화살표가 터지는 그림이 제대로 표현되지 않습니다.
            판정시 버그있습니다. (화살표가 사라질때 말입니다.)
            가끔씩 다운됩니다.. -_-;;
            부두계열 그래픽 카드에서 3스테이지 이상 진행시 프레임이 떨어지는 현상이 있습니다.

 version 0.003 (The Graphics' Revolution)
* 프로그램 전면 수정 (버림받은 MSD -_-;; )
* 몇몇 분들에게만 배포하는 버전입니다. 다른데로 뿌리지 말아주세요..

+ 더블모드 다시 추가
+ 2인용 모드 다시 추가
+ 배속모드 개선
+ 노래 데이터 교체
+ 그래픽 대폭 개선
+ 오토 플레이 추가

 version 0.002
+ 더블 모드 추가했습니다
+ 인트로 음악 출력 가능합니다.
  (노래 디렉토리에 Intro.wav로 넣어주심 됩니다.)
+ 히든 모드도 있습니다 (함 찾아보시길)
+ MSD형식이 바뀌었습니다. (예전데이터와 호환이 안됩니다.)
  (예전데이터를 쓰면 다운됩니다.)


=======================================================    끝으로

버그 리포트나 소감. 및 의견. 기타등등은

http://hello.to/kickitup에다가 해 주세요.

