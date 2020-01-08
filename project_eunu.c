#include <stdio.h>
#include <string.h>
#include <windows.h>

#define MAX 10
#define MAX_T 200

//-------------------------------------------------------------------------------
// 구조체 선언
// 사용자 정보
struct person
{
	int ID;					//사용자 아이디
	char PW[10];			//사용자 비밀번호
};

// 캐릭터 및 몬스터 정보
struct character
{
	char name[20];			//캐릭터 이름
	int level;				//레벨
	int exp;				//경험치
};

//-------------------------------------------------------------------------------
// 함수 선언
int disp_menu(void);
void enroll_id(struct person *Plog);
void login_out(struct person *Plog);
void enroll_char(struct person *Plog,struct character *user);
void fild(struct character *User,struct character *monster);
int fild_menu(void);

void make_id(struct person *Plog);
int check_id(struct person *Plog, int input_id);
int make_pw(struct person *Plog);
char *verify_pwd(char *sp);
void del_id(struct person *Plog, int num);

void status(struct character *User);
void find_mon(struct character *User,struct character *monster);
void fight(struct character *User,struct character *monster);

int my_strlen(char *sp);
void my_strcpy(char *dp, char *sp);
void my_gets(char *sp, int size);


//-------------------------------------------------------------------------------
// 전역변수

int login_num = -1; 			//로그인 여부(로그아웃 : -1/로그인 : 고유번호)
int log_count[MAX] = {0}; 	//회원 고유번호 부여 (회원가입시 고유번호 부여 고유번호 부여 후 변수[번호] =1 변경)
int need_exp[MAX+1] = {0,10,20,30,40,50,60,70,80,90,100};

//---------------------------------------------------------------------------------------------
// 함수


//메인함수
int main (void)
{
	struct person Plog[MAX]={0};
	struct character User[MAX]={0};
	struct character monster[MAX] =
	{
	 	{"cat", 1, 10},
	 	{"dog",	1, 10},
	 	{"fox", 2, 20},
	 	{"snake", 3, 30},
	 	{"wolf", 4, 40},
		{"bear", 5, 50},
		{"lion", 7, 70},
		{"tiger", 7, 70},
		{"dragon", 9, 90},
		{"eunu", 10, 100}
	} ;

	int num	= 0;

	system("title Find Eunu game");

	while (1)
	{
		num = disp_menu();
		if (num == 0) break;

		switch (num)
		{
		case 1:
			enroll_id(Plog); 	break;
		case 2:
			login_out(Plog); 	break;
		case 3:
			enroll_char(Plog,User);		break;
		case 4:
			system("cls");
			fild(User,monster);		break;
		default:
			printf("@ 메뉴 번호가 아닙니다!\n\n");
			break;
		}
	}
	return 0;
}

//메뉴화면
int disp_menu(void)
{
	int num;

	printf("\n=====[ Find Eunu game ]=====\n");
	printf("1. 아이디 등록/삭제\n");
	printf("2. 로그인/로그아웃\n");
	printf("3. 캐릭터 생성\n");
	printf("4. 사냥터 이동\n");
	printf("0. 종료\n");
	printf("---------------------------\n");

	printf("> 번호 선택 : ");
	scanf("%d", &num);

	return num;
}

//아이디 등록
void enroll_id(struct person *Plog)
{
	system("cls");

	if (login_num != -1)                   // 이미 로그인 된 상태면 반환
	{
		printf("@ 로그아웃 후 사용할 수 있습니다!\n\n");
		return;
	}

	make_id(Plog);

	return;
}

//로그인/로그아웃
void login_out(struct person *Plog)
{
	char *err_msg;
	char PW[10];
	int input_id;
	int index;

	if (login_num != -1)						// 이미 로그인 된 상태면 로그아웃
	{
		system("cls");
		printf("@ 로그아웃 되셨습니다!\n\n");
		login_num = -1;
		return;
	}
	else									//로그인 시도
	{
		while(1)
		{
			printf("> ID : ");
			scanf("%d",&input_id);

			if(input_id<=1&&input_id>=9999999999)
			{
				printf("@ 최대 10자리 수 까지의 양수만 입력 가능합니다.\n");
			}
			else
			{
				break;
			}
		}


		index = check_id(Plog, input_id);

		if(index==-1)
		{
			system("cls");
			printf("@ 일치하는 아이디가 없습니다.");
		}
		else												//아이디 확인완료 후 비밀번호 확인
		{
			do
			{
				while (1)									//비밀번호 오류검사
				{
					printf("> PW : ");
					my_gets(PW, sizeof(PW));				// 배열의 크기까지만 입력
					system("cls");
					err_msg = verify_pwd(PW);				// 문자열 형식 검사 후 에러 메시지 반환
					if (err_msg == NULL) break;
					printf("@ %s\n", err_msg);
				}

				if ( strcmp(PW, Plog[index].PW) == 0)		//비밀번호 일치여부 확인
				{
					system("cls");
					printf("@ 아이디 : %d님(NO. %d) 환영합니다.",Plog[index].ID, index);
					login_num=index;
					break;
				}
				printf("@ 비밀번호가 일치하지 않습니다!\n");
			}while (1);
		}


	}
}

//캐릭터 등록 및 삭제
void enroll_char(struct person *Plog,struct character *User)
{
	char yn;
	char name[20] ={0};
	char pwd[10];
	int i;

	if (login_num == -1)						// 이미 로그인 된 상태면 반환
	{
		printf("@ 로그인 후 사용할 수 있습니다!\n\n");
		return;
	}

	if(strcmp(name, User[login_num].name) != 0)				//캐릭터 보유시 삭제
	{
		printf("@ 이미 캐릭터를 보유하고 계십니다. 삭제하시겠습니까?");
		fflush(stdin);
		scanf("%c", &yn);
			if ((yn == 'Y') || (yn == 'y'))
			{
				//  비밀번호를 입력 받아 사번에 맞는 비밀번호인 경우 삭제 처리
				printf("> 비밀번호 입력 : ");
				gets(pwd);
				if (strcmp(pwd, Plog[login_num].PW) != 0 )
				{
					printf("@ 비밀번호가 다릅니다. 삭제할 수 없습니다.\n");
					return;
				}

				printf("@ 삭제 되었습니다!\n");
				for(i=0;i<MAX;i++)
				{
					User[login_num].name[i]='\0';
				}

				User[login_num].level= 0;
				User[login_num].exp= 0;
			}
			else
			{
				printf("@ 취소 되었습니다!\n");
			}
		return;
	}

	//캐릭터 생성
	printf("> 캐릭터 이름을 입력하세요 : ");
	my_gets(User[login_num].name, 20);

	User[login_num].level= 1;
	User[login_num].exp= 1;

	system("cls");
	printf("@ 캐릭터가 생성되었습니다.\n\n");
	status(User);



}

// 사냥터 진입
void fild(struct character *User,struct character *monster)
{
	int num;

	//사냥터 진입 조건 (1. 로그인 상태 2. 캐릭터 생성완료)
	if(login_num==-1)
	{
		printf("@ 로그인이 필요한 메뉴입니다. 메뉴로 돌아갑니다.\n");
		return;
	}
	else if(User[login_num].level==0)
	{
		printf("@ 캐릭터 생성이 필요합니다! 메뉴로 돌아갑니다.\n");
		return;
	}

	while(1)
	{
		num = fild_menu();

		if (num == 0) break;

		switch (num)
		{
		case 1:
			system("cls");
			status(User); 	break;
		case 2:
			find_mon(User,monster); 	break;
		default:
			printf("@ 메뉴 번호가 아닙니다!\n\n");
			break;
		}
	}
	return;

}

//사냥터 진입시 메뉴화면
int fild_menu(void)
{
	int num;

	printf("@ 사냥터에 오신것을 환영합니다.\n");
	printf("-------------------------------\n");
	printf("1. 나의 정보 확인하기\n");
	printf("2. 사냥감 찾기\n");
	printf("0. 메뉴로 돌아가기\n");
	printf("-------------------------------\n");

	printf("> 번호 선택 : ");
	scanf("%d", &num);

	return num;
}

//아이디 생성
void make_id(struct person *Plog)
{
	int input_id;
	int index;
	int i;
	char yn;

	printf("@ 고유 아이디를 생성 및 삭제합니다.(숫자만 입력가능)\n");
	printf("> ID : ");

	while(1)
	{
		scanf("%d",&input_id);

		if(input_id>=1&&input_id<=9999999999)
		{
			break;
		}
		else
		{
			printf("@ 최대 10자리 수 까지의 양수만 입력 가능합니다.");
		}
	}

	index = check_id(Plog, input_id);

	if(index!=-1)
	{
		printf("@ 아이디를 삭제합니다.\n");
		del_id(Plog, index);
	}
	else
	{
		printf("@ 존재하지 않는 아이디입니다. 아이디를 생성합니다.\n") ;
		printf("@ 아이디를 생성 하시겠습니까?(Y입력시 생성)\n");

		fflush(stdin);
		scanf("%c", &yn);

		if ((yn == 'Y') || (yn == 'y'))
		{
			index = make_pw(Plog);
			if(index == -1)
			{
				system("cls");
				printf("@ 비어있는 고유번호가 없습니다.");
				printf("@ 메뉴로 돌아갑니다.");
				return;
			}
			Plog[index].ID=input_id;
			system("cls");
			printf("\n@ 아이디가 생성되었습니다. \n> 고유번호 : %d\n> ID : %d\n",index,Plog[index].ID);
			return;
		}
		else
		printf("@ 취소되었습니다.");
	}
}

int check_id(struct person *Plog, int input_id)
{
	int i;

	for(i=0;i<MAX;i++)
	{
		if(input_id == Plog[i].ID)
		{
			return i;
		}
	}

	return -1;
}

int make_pw(struct person *Plog)
{
	char first[11];      		// 첫 번째 입력한 비밀번호 저장
	char second[11];       // 비밀번호 문자열 입력할 임시 배열
	char *err_msg;      	 // 입력 과정에서 형식이 맞지 않은 경우 출력할 에러 메시지
	char yn;
	int i, index;

	index = -1;

	for(i=0;i<MAX;i++)		// 비어있는 고유번호 검색
	{
		if(log_count[i]==0)
		{
			index=i;
			break;
		}
	}

	do
	{
		while (1)
		{
			printf("> 비밀번호(5-10자)(0입력시 종료)) : ");
			my_gets(first, sizeof(first));    			// 배열의 크기까지만 입력
			err_msg = verify_pwd(first);                // 문자열 형식 검사 후 에러 메시지 반환
			if (err_msg == NULL) break;

			printf("@ %s\n", err_msg);
		}

		printf("> 한번 더 입력하세요 : ");
		my_gets(second, sizeof(second));				//  배열 크기 전달
		if ( strcmp(first, second) == 0)				// 첫 번째와 두번째 입력 문자열이 같은지 비교
		{
			my_strcpy(Plog[index].PW, first);
			log_count[index]=1;
			return index;
		}
		printf("@ 비밀번호가 일치하지 않습니다!\n");
	}while (1);

}

// 비밀번호 형식 검사
char *verify_pwd(char *sp)
{
	char *errmsg[] = {
			"5자 이상 입력해야 합니다!",
			"알파벳과 숫자만 가능합니다!"	};
	char *tp;
	char ch;

	if (my_strlen(sp) < 5 )
	{
		return errmsg[0];
	}

	for (tp = sp; *tp != '\0'; tp++)
	{
		ch = *tp;
		if(  !(ch >= 'a' && ch <= 'z') && !( ch >= 'A' && ch <= 'Z' ) && !(ch >='0' && ch <='9') )
		{
			return errmsg[1];
		}
	}
	return NULL;
}

void del_id(struct person *Plog, int num)
{
	char yn = 0;
	char pwd[10]="";
	int i;

	printf("@ 이미 생성되어 있는 아이디 입니다.\n");
	printf("@ 아이디를 삭제 하시겠습니까?(Y입력시 삭제)\n");

	fflush(stdin);
	scanf("%c", &yn);

	if ((yn == 'Y') || (yn == 'y'))
	{
		//  비밀번호를 입력 받아 사번에 맞는 비밀번호인 경우 삭제 처리
		printf("> 비밀번호 입력 : ");
		gets(pwd);
		if (strcmp(pwd, Plog[num].PW) != 0 ) {
			printf("@ 비밀번호가 다릅니다. 삭제할 수 없습니다.\n");
			return;
		}

		printf("@ %d번이 삭제 되었습니다!\n", num);
		log_count[num]=0;
		Plog[num].ID=0;
	}
	else
	{
		printf("@ 취소 되었습니다!\n");
	}
	return;
}

//캐릭터 상태창
void status(struct character *User)
{
	printf("--------캐릭터 상태창--------\n");
	printf("# name : %s\n",User[login_num].name);
	printf("# level : %d\n",User[login_num].level);
	printf("# exp : %d / %d\n\n",User[login_num].exp, need_exp[(User[login_num].level)]);

	return;
}

//몬스터 대면
void find_mon(struct character *User,struct character *monster)
{
	int i,j;
	for(j=0;j<3;j++)
	{

		for(i=0;i<MAX_T;i++)
		{
			system("cls");
			printf("열심히 사냥감을 찾는 중입니다. 잠시 기다려주세요.\n\n");
			printf("\t |||||||||||\n");
			printf("\t (o_ o     )\n");
			printf("\t/(        /)\n");
			printf("\t    d   d\n");
		}

		for(i=0;i<MAX_T;i++)
		{
			system("cls");
			printf("열심히 사냥감을 찾는 중입니다. 잠시 기다려주세요.\n\n");
			printf("\t\t\t |||||||||||\n");
			printf("\t\t\t (/    o _o)/\n");
			printf("\t\t\t (         ) \n");
			printf("\t\t\t     b   b\n");
		}
	}
	system("cls");
	printf("\n\n\t\t |||||||||||\n");
	printf("\t\t (  o _ o  )\n");
	printf("\t\t ( !     ! ) \n");
	printf("\t\t    d   b\n\n");

	printf("@ 만만한 사냥감을 찾았습니다!!!\n");

	fight(User,monster);

	return;
}


void fight(struct character *User,struct character *monster)
{
	int i, index, temp, lvup;

	while(1)
	{
		printf("@ 사냥감들이 보이는데 무엇을 잡을까요?\n");

		for(i=0;i<MAX;i++)
		{
			if(User[login_num].level>=monster[i].level)
			{
				printf("# %d - %s (monster level : %d)\n\n",i+1, monster[i].name, monster[i].level);
			}
		}
		printf("번호 : ");
		scanf("%d",&index);

		if((monster[index].level>User[login_num].level)||((index<1)||(index>MAX)))
		{
			printf("@ 잘못된 입력입니다. 다시 입력해주세요.\n");
		}
		else break;
	}

	printf("\n# we chtch %s!!! \n", monster[index].name);
	printf("@ %d 만큼 경험치를 얻었습니다.\n", monster[index].exp);

	temp = monster[index].exp + User[login_num].exp;
	while(1)
	{
		if(User[login_num].level>10)
		{
			printf("축하합니다 당신은 만렙입니다.\n\n");
			User[login_num].exp	= 0;
			return;
		}
		else if(temp>=(need_exp[User[login_num].level]))
		{
			temp-=(need_exp[User[login_num].level]);
			User[login_num].level++;
			printf("@ 레벨이 1 상승했습니다.\n\n");

		}
		else break;
	}
	if(User[login_num].level<11)
	{
		User[login_num].exp	= temp;
	}
	else
	{
		User[login_num].exp	= 0;
	}
	status(User);
	return;
}

// 배열에 저장된 문자열의 길이 반환
int my_strlen(char *sp)
{
	int cnt = 0;

	while (*sp++ != '\0')	{
		cnt++;
	}
	return cnt;
}

// 문자열 복사 함수
void my_strcpy(char *dp, char *sp)
{
	while ( (*dp++ = *sp++) != '\0');
}

// 배열의 크기까지만 문자열 입력
void my_gets(char *sp, int size)
{
	char ch;
	int i = 0;

	fflush(stdin);
	ch = getchar();
	while ((i < (size - 1)) && (ch != '\n'))
	{
		sp[i] = ch;
		i++;
		ch = getchar();
	}
	sp[i] = '\0';
	fflush(stdin);
}
