# [공통] 기본환경
## OS version
Ubunto 20.04
## compiler version
gcc 9.3.0

## 가능한 입력 파일
text 기반 으로 입력된 파일 ex).txt / .dat .. 

## 공통 예외처리
1.주어진 명령어들의 숫자가 맞지 않을경우
2.원본 파일이 열리지 않을 경우 

### copy.c
실행방법 : ./실행파일명 <원본파일명> <복사본파일명><br>
구현 내용 : <원본파일명> <복사본파일명> 을 입력으로 줄때 새로운 복사본파일에 원본 파일의 내용을 복사함.

### read.c
실행방법 : ./실행파일명 <파일명> <offset> <읽기 바이트 수><br>
구현 내용 : 파일에서 오프셋 포함하여 오른편에 존데하는 <읽기 바이트 수> 만큼 읽고 출력/ EOF 만나면 EOF전까지만 출력
종료 예외 처리 : 파일 크기를 넘어선 offset이 주어진 경우 

### merge.c
실행방법 : ./실행파일명 <파일1> <파일2> <파일3> <br>
구현 내용 : 파일2 와 파일3 를 병합한 파일 1을 생성한다.

### overwrite.c
실행방법 : ./실행파일명 <파일명> <offset> <data> <br>
구현 내용 : 주어진 파일의 offset 부터 data를 덮어 쓴다. EOF를 만나도 data를 계속 덮어 쓴다.
종료 예외 처리 : 파일 크기를 넘어선 offset이 주어진 경우 
  
### insert.c
실행방법 : ./실행파일명 <파일명> <offset> <data> <br>
구현 내용 : 주어진 파일의 offset-1과 offset 사이에 data를 쓴다.
종료 예외 처리 : 원본 파일의 크기가 1024*512 를 넘는 경우 (프로그램 내부의 임시 버퍼에 기존 파일을 담아두고, data를 insert하기 때문)

### delete.c
실행방법 : ./실행파일명 <파일명> <offset> <삭제 바이트 수> <br>
구현 내용 : 주어진 파일의 offset을 포함하여 오른쪽에 있는 data를 삭제 바이트 수 만큼 삭제한다.
종료 예외 처리 : 원본 파일의 크기가 1024*512 를 넘는 경우 (프로그램 내부의 임시 버퍼에 기존 파일을 담아두고, data를 delete 때문)



