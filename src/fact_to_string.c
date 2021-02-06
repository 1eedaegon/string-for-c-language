
int factorial(int n);
int str_len(char *arr);
int data_str_len(char *arr);
int atoi_string(char *str);
void itoa_string(int num, char *target, int target_size);
void copy_string(char *dest, char *src);
void reverse_string(char *target);
int string_compare(char *s1, char *s2);
int get_digits(int num);
void input_args(int argc, char *argv[]);

int main(int argc, char *argv[]) {
	// 이 부분은 작동을 하지 않는다.
	// int variable = (int)*argv[1];
	// 문자열은 1개의 char가 아니라 char배열이기 때문이다.
	// 그리고 argv[1]은 그 char배열의 주소를 저장하고 있는 포인터 변수이다.
	
	int result = 0;
	int variable = 0;
	// 문자열 리터럴은 코드 영역상의 읽기전용이다(수정불가).
	char * string = "문자열을 복사할거에요";
	char copied_string[data_str_len(string)];
	// copy string source to destination
	copy_string(copied_string, string);
	printf("copied string: %s\n",copied_string);
	printf("해당 문자열 자료형의 크기: %ld\n",sizeof(copied_string));
	printf("해당 문자열의 길이: %d\n", str_len(copied_string));
	/*
		리터럴은 접근이 코드레벨과 동일한 스코프에 위치
	       하지만 상수는 아니다.
	       한글 리터럴의 경우 리눅스(UTF-8)에서 한글자당 3바이트씩 저장한다.
	       윈도우(EUC-KR, CP949)는 2바이트씩 저장한다.
	       그러므로 "문자열을 복사할거에요"의 길이는 한글(3바이트) 10글자 띄어쓰기(1바이트) 1글자 이므로 총 31이다.
	       처리할 수 있는 문자열 타입은 MCU(Multi character set)과 WCS(Wide character set)이 있다.
	       멀티는 각 바이트에 대해 가변, 와이드는 전체 바이트에 대해 고정이다.
	       
	*/
	reverse_string(copied_string);
	printf("reversed: %s\n", copied_string);
	if(argv[1]){
		printf("Argv[1] is exist\n");
	} else {	
		printf("Argv[1] is not exist!\n");
	}

	variable = atoi_string(argv[1]);
	//result = factorial(variable);
	printf("%d를 넣은 팩토리얼의 결과값은: %d\n",variable, result);
	int input_number = variable;
	char tmp[get_digits(input_number)+1];	
	itoa_string(input_number, tmp, sizeof(tmp));
	printf("itoa result: %s\n",tmp);
	return 0;
}

int factorial(int n) {
	if (n<=0 || n==1) {
		return 1;
	}
	return n*factorial(n-1);
}

int atoi_string(char *str) {
	// str은 아스키코드내의 입력으로 제한한다.
	int integer_result = 0;
	while(*str) {
		// 숫자 0은 아스키코드 48이므로 48을빼고 정수로 변환
		// 숫자가 들어올 때 마다 왼쪽으로 10의 자리만큼 Shift
		if('0' <= *str && *str <= '9'){
			integer_result *= 10;
			integer_result += (int)(*str-48);
		}
		str++;
	}
	return integer_result;
}

void copy_string(char *dest, char *src){
	// 배열변수도 함수의 인자로 전달하면 첫번째 주소를 넘겨주면서 크기정보가 사라진다.
	// 어차피 sizeof는 함수내에서 먹히지 않는다.
	while(*src){
		*dest = *src;
		dest++;
		src++;
	}
	*dest = *src;
}

int str_len(char *arr){
	// For linux system( UTF-8 )
	int idx = 0;
	int len = 0;
	while(arr[idx]){
		len++;
		// 아스키 코드의 범위를 벗어날 때 리눅스의 MBS는 3바이트이상이므로 3바이트를 건너뛴다.
		if(arr[idx] & 0x80){
			idx+=2;		
		}
		idx++;
	}
	return len;
}
/*
 * 필요없음 sizeof()-1이랑 동치
 * 필요없을 것이라 생각했지만 sizeof는 문자열 리터럴 갯수를 가져올 수 없다.
 * sizeof가 배열 변수를 가져오면 계산을 해주지만
 * 리터럴은 char[]의 첫번째주소를 가리키고 있을 뿐이기 때문에 포인터 크기만 가져온다.
*/
int data_str_len(char *arr){
	int len = 0;
	while(arr[len]){
		len++;
	}
	len++;
	return len;
}

void reverse_string(char *src){
	// 원본 배열의 크기를 구해 동일크기 배열 생성
	int len = data_str_len(src);
	if(len <= 2) return;
	
	int src_idx = 0;
	int target_idx = len-2;
	char target[len];
	
	// 마지막 문자 삽입
	*(target+len-1) = *(src+len-1);
	// 문자열 치환	
	while(src_idx<=len-1){
		if(*(src+src_idx) & 0x80){
			*(target+target_idx-2) = *(src+src_idx);
			*(target+target_idx-1) = *(src+src_idx+1);
			*(target+target_idx) = *(src+src_idx+2);
			target_idx-=3;
			src_idx+=3;
		}
		else{
			*(target+target_idx) = *(src+src_idx);
			target_idx--;
			src_idx++;
		}
	}
	
	/*
	 * 배열 변수는 상수 포인터이므로 날먹이 안됨.
	src = target;
	*/
	copy_string(src,target);
}



void itoa_string(int num, char *target, int target_size) {
	//target의 마지막에 delimit 추가
	target[--target_size]='\0';

	while(target_size > 0) {
		target[target_size-1] = (num % 10) + '0';
		num /= 10;
		target_size--;
		if(num == 0) break;
	}
	return;
}

int string_compare(char *s1, char *s2) {
	while(*s1) {
		if(*s1 != *s2) {
			return *s1 - *s2;
		}
		s1++;
		s2++;
	}
	return *s1 - *s2;
}

int get_digits(int num) {
	int digits = 0;
	if(num == 0) {
		return 1;
	}
	while(num != 0) {
		num/=10;
		digits++;
	}
	return digits;
}
