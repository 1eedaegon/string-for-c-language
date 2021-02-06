#include <stdio.h>
#include <time.h>
/*
 * p는 패턴, s는 문자열, t는 target_array 줄임말
 * 나이브(브루트포스)와 kmp, 보이어무어 패턴찾기 성능비교
*/
int naaive(char *p, char *s);
int failure_fn(int idx, char *s);
int simple_kmp(char *p, char *s);
int kmp(char *p, char *s);
int bad_boyer_moore(char *p, char *s);
int data_str_len(char *t);
void search_engine_optimize(char *a, char *p);

int main(int argc, char *argv[]){
	// time.h에 있는 시간측정 구조체 clock_t
	clock_t start1, start2, start3, end1, end2, end3;
	// 시간결과 저장
	float res1, res2;
	char *sample = argv[1];
	char *pattern = argv[2];
	printf("====== Spatial testing ======\n");
	printf("원본 문자열: %s\n 입력 패턴: %s\n", sample, pattern);
	start1 = clock();
	printf("나이브에서 찾은 %s의 갯수: %d\n", pattern, naaive(pattern, sample));
	end1 = clock();
	start2 = clock();
	printf("KMP에서 찾은 %s의 갯수: %d\n", pattern, kmp(pattern, sample));
	end2 = clock();
	start3 = clock();
	printf("Boyer-moore에서 찾은 %s의 갯수: %d\n", pattern, bad_boyer_moore(pattern, sample));
	end3 = clock();

	printf("\n====== Time complexity testing ======\n");
	printf("나이브한 패턴검색: %.5f초\n",(float)(end1-start1)/CLOCKS_PER_SEC);
	printf("간단한 KMP 패턴검색: %.5f초\n",(float)(end2-start2)/CLOCKS_PER_SEC);
	printf("Boyer-moore 패턴검색: %.5f초\n",(float)(end3-start3)/CLOCKS_PER_SEC);
	printf("\n=== 실패함수 검증 === \n 패턴:%s, 자릿수:%d, 실패함수:%d\n", pattern, 5, failure_fn(5, pattern));
	return 0;
}

int data_str_len(char *t){
	int len = 0;
	while(t[len]){
		len++;
	}
	len++;
	return len;
}

int naaive(char *p, char *s){
	int cnt = 0;
	int s_cnt = 0;
	int p_cnt = 0;
	// flag is type of BOOL
	int flag = 0;
	while(*s){
		flag = 0;
		s_cnt = 0;
		p_cnt = 0;
		while(p[p_cnt]){
			if(s[s_cnt] != p[p_cnt]){
				flag = 1;
				break;
			};
			p_cnt++;
			s_cnt++;
		}
		if(!flag) { cnt++; }
		s++;
	}
	return cnt;
}
/*
 * KMP의 Failure function 이해.
 * 두 개의 배열을 생각해보자.
 * 접두사와 접미사 탐색을 하려면 접두사와 접미사를 한칸씩 이동시키면서 확인하면 O(n^2)의 시간복잡도를 얻는다.(==나이브)
 * 하지만 2번째 배열을 우측으로 한칸 이동하면서 1번째 배열을 우측으로 한칸 이동시키면 접두<=>접미사의 확인하는 과정을
 * O(N+M)으로 줄일 수 있다. 하지만 이부분을 이해하는 과정이 필요.
*/

// 해당 입력 문자열의 인덱스까지의 문자열에 대해 접두사와 접미사가 일치하는 최대 길이를 구한다.
// 입력값: 문자열, 인덱스
int failure_fn(int idx, char *s){
	// This function has O(n^2): while(1) * for(1) of time complexity, not useful
	// e is Maximum of length that eqauls prefix and sufix
	// p_idx, s_idx are position of prefix/sufix
	int e = 0;
	int p_idx = 0;
	int i = 0;
	int flag=0;
	while(p_idx < idx){
		flag = 0;
		for(i=0; i<=p_idx; i++){
			if(s[i] != s[idx-p_idx+i]){
				flag = 1;
				break;
			}
		}
		if( !flag ){ e = p_idx+1; }
		p_idx++;
	}
	return e;
}

// 실패함수의 의미.
// 문자열 S에 대해 임의의 위치 문자를 S(i)라고 한다면
// 실패함수 F(i)는 S의 0번쨰부터 i번쨰까지의 접두사와 접미사가 일치하는 최대 길이이다.(되돌아가는 위치가 아니다!!!!!!!!!!)
// pi is a list of maxmum lengths that equals suffix and prefix

int simple_kmp(char *p, char *s){
	int cnt = 0;
	int p_idx = 0;
	int p_len = data_str_len(p);
	while(*s){
		if(p[p_idx] != *s && p_idx > 0){
			p_idx = failure_fn(p_idx-1, p);
			continue;
		}
		if(p[p_idx] == *s){ p_idx++; }
		s++;
		// 맞은 갯수가 패턴의 길이와 일치하면 패턴일치수 +1, 패턴 처음부터 다시
		// 패턴을 찾기 때문에 null값은 따지지 않는다: p_len-1
		if(p_idx == p_len-1){ cnt++; p_idx=0; }
	}
	return cnt;
}

int kmp(char *p, char *s){
	int cnt = 0;
	int t_idx = 1;
	int p_idx = 0;
	int p_len = data_str_len(p);
	int pi[p_len];
	pi[0] = 0;

	// Make Failure array of p(pattern) using KMP
	// target == pattern
	while(t_idx < p_len){
		if(p[t_idx] != p[p_idx]){
		       if(p_idx > 0){
				p_idx = pi[p_idx-1];
				continue;
		       }
		       p_idx = 0;
		}
		if(p[t_idx] == p[p_idx]){ p_idx++; }
		pi[t_idx] = p_idx;
		t_idx++;
	}
	printf("PI함수 검증: ");
	for(int i=0; i<p_len-1; i++){
		printf("|%c:%d| ",p[i],pi[i]);
	}
	printf("\n");

	// Find number of string pattern using kmp
	t_idx = 0;
	p_idx = 0;
	while(s[t_idx]){
		if(s[t_idx] != p[p_idx] && p_idx > 0){
			p_idx = pi[p_idx-1];
			continue;
		}
		if(s[t_idx] == p[p_idx]){ p_idx++; }
		t_idx++;
		if(p_idx >= p_len-1){ 
			cnt++;
			p_idx = pi[p_idx-1]; 
		}
	}
	return cnt;
}

// Find sub-string pattern using tree prefix/suffix
// BM-algorithm(Bad Character rule)
int bad_boyer_moore(char *p, char*s){
	int cnt = 0;
	int p_idx = 0;
	// Count pattern length without NULL
	int p_len = data_str_len(p)-1;

	// BM rule preprocessing
	
	// BM(bad chracter rule)
	while(*s){
		
		s++;
	}
	return cnt;
}


