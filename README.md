# [공통] 기본환경
## OS version
Ubunto 20.04
## compiler version
gcc 9.3.0

## 공통 예외처리
1.원본 파일이 열리지 않을 경우 

## 프로그램 개요
Flash memory의 계층구조와 Mapping Algorithm을 실험해보기 위한 프로그램 이다.<br>
실제 Flash memory 대신 flashmemory라는 파일을 이용하였다. 메모리 구조는 blockmap.h의 수정으로 바꿀 수 있다.<br>
Mapping Algorithm 으로는 Block Mapping 방식을 이용하였다<br>

main.c - File system<br>
ftl.c  - Flash translation Layer<br>
fdevice.c - Flash Device Driver<br>
의 역활을 수행한다.<br>
메모리(flashmemory 파일)의 초기화는 '-1' 로 이루어 진다. -1 기준으로 data 유뮤 여부를 판단한다.<br>

## 실행방법
makefile 로 실행파일을 생성하면, main.c에 적혀있는 테스트 코드에 따라 실행된다.

### main.c
기본 전제 : ftl_open()함수를 호출하여 Mapping Table 을 복구 해야 한다.<br>
data를 전달/받기 위해서 sector size 만큼 선언후 원하는 data 를 넣어 ftl.c 에 있는 함수를 호출해야 한다. <br>
구현 내용 : flashmemory파일을 열어 모든 바이트를 -1 로 초기화한다. 만약 기존 파일을 열고싶다면, 주석 처리하면 된다. <br>
그후 원하는데로 테스트를 진행 하면 된다. 

### ttl.c
기본 전제 : file system 에서 전달된 data를 담은 buffer를 포함한 실제 page data를 위한 buffer를 할당해야 한다.<br>
구현 내용 : file system의 요청을 처리할 수 있는 모든 함수가 구현 되어 있다. logical page와 physical page를 Mapping 시켜준다.<br>
freeBlock를 관리하기 위한 알고리즘도 구현되어 있다.

### fdevicedriver.c
기본 전제 : 전달받은 data와 위치에 대한 직접적인 연산을 수행한다.<br>
구현 내용 : erase 연산에 대해선 Block size 만큼 메모리를 확보한 후 -1 로 초기화 하여 해당 block 을 전부 초기화 해준다.









