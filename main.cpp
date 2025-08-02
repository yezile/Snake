//2024��4��23��14:35 ��ʼ���� 1.0.3
//������: ��ʽ��� 1.0.3
/*
������
1.0.2����˵��.txt
*/

#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
using namespace std;

//������
#define KEYDOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)

//������ɫ���иĶ���
#define BLACK 0x000000
#define WHITE 0xFFFFFF
#define GREY 0x606A57
#define YELLOW 0xFFEE00
#define BLUE 0x4169E1
#define GREEN 0x10EF10
#define RED 0xEF2222
#define PURPLE 0x9030EB

//����
#define NULL0 1001
#define BODY 1002
#define HEAD 1003
#define APPLE 1004
#define WALL 1005
#define NECK 1006

#define HEAD2 1011
#define BODY2 1010
#define NECK2 1013

#define wasd 0
#define next1 1

//���� �ĸ�����
int keya[4] = {'w', 'a', 's', 'd'};
int keyb[4] = {24, 27, 25, 26};
int keyc[4] = {'8', '4', '2', '6'};
map<char, int>akey;

//��ǰ����������
int key1, key2, key3;

int nandu;

int applex = 1, appley = 1;

//����
int dx[5] = {0, 0, -1, 1};
int dy[5] = {1, -1, 0, 0};

int mode = 1;

struct Position {
	int x, y;
	//��¼λ��
};

struct snack {

	queue<Position>snackPosition;//�ߵ�λ��
	Position head, neck; //ͷ�Ͳ��ӵ�λ��
	double Xspeed, speed, Sspeed; //�趨�ٶȡ���ǰ�ٶȡ���ʾ�ٶ�
	int length;//����
	int remainingLength;//ʣ�೤��
} player1, player2;

//��ͼ����
int a[100][100];

int GameOver;

int N, M;

//�������ֵ
int random(int l, int r) {
	int ans = 1;
	int as = rand() % (r - l + 1) + l, bs = rand() % (r - l + 1) + l, cs = rand() % (r - l + 1) + l;
	int p = rand() % (3 - 1 + 1) + 1;
	if (p == 1) ans = as;
	if (p == 2) ans = bs;
	if (p == 3) ans = cs;
	return ans;
}

int win = 0;

//��ʼ��
void rgbInit(); //ANSI�ĺ���
void init1();
void init2();
void init3();
//��������
void gotoxy(int x, int y); //���λ��
void rgbColor(int w, int b); //��ɫ����
void pr(int w, int b, int x, int y); //�����ɫRGB��λ��
void pr(int w, int x, int y, string s); //�򵥵��ַ������
//��Ϸ����
void keydownControl(int i); //���̶���
void printCurrent(int x, int y, int z); //�޸ķ���
void printApple();     //����ƻ��
void setNan(); //�����Ѷ�

void runGame1();       //������Ϸ
void runGame2();       //������Ϸ
void runGame3();
void restartGame();   //��ʼ��Ϸ
void pauseGame();     //��ͣ��Ϸ
void endGame();       //������Ϸ
void advertisement(); //���Ź��

int game_;

int main() {
	rgbColor(WHITE, BLACK);
	N = 40, M = 29;
	init1();

	while (1) {
		while (KEYDOWN(VK_SPACE));

		rgbColor(WHITE, BLACK);
		if (game_ != 4) {
			rgbColor(WHITE, BLACK);
			system("cls");
			restartGame();

			int result = MessageBox( NULL, TEXT("�Ƿ�ʼ��Ϸ "), TEXT("̰����"), MB_ICONINFORMATION | MB_YESNO);
			if (result == IDNO) {
				rgbColor(WHITE, BLACK);
				system("cls");
				continue;
			}
		}
		rgbColor(WHITE, BLACK);
		system("cls");

		Sleep(100);
		if (mode == 1)runGame1();
		if (mode == 2)runGame2();
		if (mode == 3) runGame3();
		if (game_ == 6) {
			rgbColor(WHITE, BLACK);
			system("cls");
			cout << "�����˳�����";
			Sleep(random(20, 100));
			return 0;
		}

	}

	return 0;
}
//=========================================================
//=========================================================
//ANSI�ĺ���
void rgbInit() {
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);//������
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);//������
	DWORD dwInMode, dwOutMode;
	GetConsoleMode(hIn, &dwInMode);//��ȡ����̨����ģʽ
	GetConsoleMode(hOut, &dwOutMode);//��ȡ����̨���ģʽ
	dwInMode |= 0x0200;//����
	dwOutMode |= 0x0004;
	SetConsoleMode(hIn, dwInMode);//���ÿ���̨����ģʽ
	SetConsoleMode(hOut, dwOutMode);//���ÿ���̨���ģʽ
}
//---------------------------------------------------------
//��ɫ����
void rgbColor(int w, int b) {
	int wr = w / 0x10000;
	int wg = w / 0x100 % 0x100;
	int wb = w % 0x100;
	int br = b / 0x10000;
	int bg = b / 0x100 % 0x100;
	int bb = b % 0x100;
	printf("\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm", wr, wg, wb, br, bg, bb);
}
//---------------------------------------------------------
//���λ��
void gotoXY(int x, int y) {
	COORD  coord;
	coord.X = x;
	coord.Y = y;
	HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(a, coord);
}
//---------------------------------------------------------
//�����ɫRGB��λ��
void pr(int w, int b, int x, int y) {
	rgbColor(w, b);
	gotoXY(x, y);
}
//---------------------------------------------------------
//�򵥵��ַ������
void pr(int w, int x, int y, string s) {
	pr(w, BLACK, x, y);
	cout << s;
	pr(WHITE, BLACK, 0xffffff, 0xffffff);
}
//----------------------------------------------------------
//��ʼ��
void init1() {


	//�Ƴ����ٱ༭ģʽ
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hStdin, &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE;
	SetConsoleMode(hStdin, mode);


	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);//��ȡ����̨�����Ϣ
	CursorInfo.bVisible = false; //���ؿ���̨���
	SetConsoleCursorInfo(handle, &CursorInfo);//���ÿ���̨���״̬
	CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };  // �����Ϣ
	rgbInit();
	SetConsoleTitleA("̰����");

	HWND hwnd = GetForegroundWindow();
	int cx = GetSystemMetrics(SM_CXSCREEN);//��Ҫ���ݵ��ԵĴ�С���üӼ�
	int cy = GetSystemMetrics(SM_CYSCREEN);//��Ҫ���ݵ��ԵĴ�С���üӼ�
	LONG_PTR last_style = GetWindowLongPtr(hwnd, GWL_STYLE);
	RECT last_rect;
	GetWindowRect(hwnd, &last_rect);
	LONG l_WinStyle = GetWindowLong(hwnd, GWL_STYLE);
	SetWindowLong(hwnd, GWL_STYLE, (l_WinStyle | WS_POPUP | WS_MAXIMIZE));
	SetWindowPos(hwnd, NULL, 0, 0, cx, cy, 0);
	UpdateWindow(hwnd);
	srand(time(NULL) + time(0) - time(0) * 1.1 + rand());

	//����
	akey['a'] = akey['A'] = akey[24] = akey['4'] = 2;
	akey['w'] = akey['W'] = akey[27] = akey['8'] = 1;
	akey['d'] = akey['D'] = akey[25] = akey['6'] = 3;
	akey['s'] = akey['S'] = akey[26] = akey['2'] = 0;

}
//---------------------------------------------------------
//��ʼ�� 2
void init2() {


	rgbColor(WHITE, BLACK);
	gotoXY(1, 6);
	cout << "ƻ��λ�ã�";

	if (mode == 1) {
		gotoXY(3, 8);
		cout << "ͷλ�ã�";
	}
	if (mode == 2) {
		gotoXY(1, 8);
		cout << "���1���峤�ȣ�";
		gotoXY(1, 10);
		cout << "���2���峤�ȣ�";
	} else if (mode == 1) {
		gotoXY(1, 10);
		cout << "���峤�ȣ�";
	} else {
		gotoXY(2, 8);
		cout << "������峤�ȣ�";
		gotoXY(2, 10);
		cout << "  AI���峤�ȣ�";
	}
	rgbColor(WHITE, BLACK);
	gotoXY(1, 12);
	cout << "�ٶȣ� " << player1.Sspeed;

	for (int i = 0; i <= N + 1; i++) {
		for (int j = 0; j <= M + 1; j++) {
			printCurrent(i, j, a[i][j]);
			//����
		}
	}
	if (mode == 3) {
		gotoXY(N * 2 + 35, 15);
		printf("���л�AI���𣺰� 1 2 3 4 5 �л�");
		gotoXY(N * 2 + 35, 16);
		printf("�� �� �� �� �� �� ǿ");

	}
	if (mode == 1 || mode == 3) {
		rgbColor(0x01FFFF, BLACK);

		gotoXY(N * 2 + 33, 6);
		printf("�����ƣ�");
		gotoXY(N * 2 + 35, 8);
		printf("�������ƶ����� A 4");
		gotoXY(N * 2 + 35, 9);
		printf("�������ƶ����� D 6");
		gotoXY(N * 2 + 35, 10);
		printf("�������ƶ����� S 2");
		gotoXY(N * 2 + 35, 11);
		printf("�������ƶ����� W 8");
		gotoXY(N * 2 + 35, 12);
		printf("���ӿ��ٶȣ��ո��");
		gotoXY(N * 2 + 35, 13);
		printf("����ͣ��Ϸ���س��� Esc��");
	} else {
		rgbColor(0x01FFFF, BLACK);
		gotoXY(N * 2 + 33, 6 - 2);
		printf("�����ƣ����1����");
		gotoXY(N * 2 + 35, 8 - 2);
		printf("�������ƶ���A");
		gotoXY(N * 2 + 35, 9 - 2);
		printf("�������ƶ���D");
		gotoXY(N * 2 + 35, 10 - 2);
		printf("�������ƶ���S");
		gotoXY(N * 2 + 35, 11 - 2);
		printf("�������ƶ���W");
//		gotoXY(N*2+35, 12-2);
//		printf("���ӿ��ٶȣ�Q");
		gotoXY(N * 2 + 33, 14 - 2);
		printf("�����ƣ����2����");
		gotoXY(N * 2 + 35, 16 - 2);
		printf("�������ƶ����� 4");
		gotoXY(N * 2 + 35, 17 - 2);
		printf("�������ƶ����� 6");
		gotoXY(N * 2 + 35, 18 - 2);
		printf("�������ƶ����� 2");
		gotoXY(N * 2 + 35, 19 - 2);
		printf("�������ƶ����� 8");
//		gotoXY(N*2+35, 20-2);
//		printf("���ӿ��ٶȣ�P");
		gotoXY(N * 2 + 35, 22 - 2);
		printf("����ͣ��Ϸ���س��� Esc��");
	}

}
//---------------------------------------------------------
//��ʼ�� 3
void init3() {
	rgbColor(WHITE, BLACK);
	system("cls");
	for (int i = 0; i <= max(N, M) + 1; i++) {
		//ǽ
		a[i][0] = a[0][i] = WALL;
		a[N + 1][i] = a[i][M + 1] = WALL;
	}

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= M; j++) {
			a[i][j] = NULL0;
			//��
		}
	}
	game_ = 0;
	while (player1.snackPosition.size()) {
		player1.snackPosition.pop();
	}
	while (player2.snackPosition.size()) {
		player2.snackPosition.pop();
	}
//	player1.snackPosition.push({N/2+1,M/3*2+1});
//	printCurrent(N/2+1,M/3*2+1,HEAD);
	if (mode == 1) {
		player1.snackPosition.push({N / 2 + 1, M / 3 * 2 + 1});
		printCurrent(N / 2 + 1, M / 3 * 2 + 1, HEAD);
	} else {
		player1.snackPosition.push({N / 3 + 1, M / 3 * 2 + 1});
		printCurrent(N / 3 + 1, M / 3 * 2 + 1, HEAD);
		player2.snackPosition.push({N / 3 * 2 + 1, M / 3 * 2 + 1});
		printCurrent(N / 3 * 2 + 1, M / 3 * 2 + 1, HEAD2);
	}

	//��ʼ����

	key1 = 1;
	key2 = 1;

}
//---------------------------------------------------------
//���̶���
void getKey(int i) {
	if (i == wasd) {
		clock_t start, finish;
		double totaltime;//��ʱ
		start = clock();
		int a1 = 1, a2 = 1, a3 = 1, a0 = 1;
		while (1) {
			if (KEYDOWN('a') || KEYDOWN('A')) key1 = 2;
			if (KEYDOWN('w') || KEYDOWN('W')) key1 = 1;
			if (KEYDOWN('d') || KEYDOWN('D')) key1 = 3;
			if (KEYDOWN('s') || KEYDOWN('S')) key1 = 0;
			if (KEYDOWN(VK_NUMPAD4) || KEYDOWN(VK_LEFT))  key2 = (mode == 2 ? 2 : key2), key1 = (mode == 1 || mode == 3 ? 2 : key1);
			if (KEYDOWN(VK_NUMPAD8) || KEYDOWN(VK_UP))    key2 = (mode == 2 ? 1 : key2), key1 = (mode == 1 || mode == 3 ? 1 : key1);
			if (KEYDOWN(VK_NUMPAD6) || KEYDOWN(VK_RIGHT)) key2 = (mode == 2 ? 3 : key2), key1 = (mode == 1 || mode == 3 ? 3 : key1);
			if (KEYDOWN(VK_NUMPAD2) || KEYDOWN(VK_DOWN))  key2 = (mode == 2 ? 0 : key2), key1 = (mode == 1 || mode == 3 ? 0 : key1);
			if (mode == 3) {
				for (int i = 1; i <= 2; i++) {
					if (KEYDOWN('0' + i)) {
						nandu = i;
						setNan();
					}
				}
				if (nandu == 1) {
					key2 = (random(0, 3) == 3 ? random(0, 3) : key2);
				}
				if (nandu == 2) {
					int heng = 0, z = 0;
					if (applex > player2.head.x)  heng = 1;
					if (applex == player2.head.x) heng = 0;
					if (applex < player2.head.x)  heng = -1;
					if (appley > player2.head.y)  z = 1;
					if (appley == player2.head.y) z = 0;
					if (appley < player2.head.y)  z = -1;

					int tx1 = dx[key2] + player2.head.x;
					int ty1 = dy[key2] + player2.head.y;
					int nowa1 = a[tx1][ty1];
					int tx2 = dx[key2] + tx1;
					int ty2 = dy[key2] + ty1;
					int nowa2 = a[tx2][ty2];
					gotoXY(1, 16);
					cout << "���� " << key2;
					if (nowa2 == BODY2 || nowa2 == BODY || nowa2 == NECK || nowa2 == WALL || nowa1 == BODY2 || nowa1 == BODY
					        || nowa1 == NECK || nowa1 == WALL) {

						string st[10] = {"", "", "��", "��"};

						if (key2 == 1) {
							a1 == 0;
							if (a2 == 1) key2 = 2;
							else key2 = 3;
						}
						if (key2 == 0) {
							a0 == 0;
							if (a2 == 1) key2 = 2;
							else key2 = 3;
						}
						if (key2 == 2) {
							a2 == 0;
							if (a0 == 1) key2 = 0;
							else key2 = 1;
						}
						if (key2 == 3) {
							a3 == 0;
							if (a0 == 1) key2 = 0;
							else key2 = 1;
						}
					} else {
						gotoXY(1, 16);
						cout << "���� ��       ";
						if (heng == 1 && key2 != 2 && (random(1, 2) == 1 || z == 0) && a3) key2 = 3;
						else if (heng == -1 && key2 != 3 && (random(1, 2) == 1 || z == 0) && a2) key2 = 2;
						else if (z == 1 && a0) key2 = 0;
						else if (z == -1 && a1) key2 = 1;
						else 	key2 = (random(0, 10) == 3 ? random(0, 3) : key2);
					}

				}
			}
			if ((KEYDOWN(VK_SPACE)) && (mode == 1 || mode == 3)) player1.speed = 0; //����
			else player1.speed = player1.Xspeed;
			if (KEYDOWN(27) || KEYDOWN(VK_RETURN)) key1 = 5;
			Sleep(15);
			finish = clock();
			totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
			if (totaltime >= player1.speed) break;
		}
	}
}
//---------------------------------------------------------
//����ƻ��
void printApple() {

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= M; j++) {
			if (a[i][j] == NULL0) {
				applex = 0;
				break;
			}
		}
		if (applex == 0) break;
	}
	if (applex) return ;
	do {
		if (random(1, 20) == 1) {
			applex = random(1, N); //�����
			appley = random(1, M);
		} else {
			applex = random(2, N - 1); //�����
			appley = random(2, M - 1);
		}
		if (a[applex][appley] == NULL0) {
			printCurrent(applex, appley, APPLE);
			pr(RED, BLACK, 13, 6);
			printf("��(%d,%d)    ", applex, appley); //ƻ������λ
			return ;
		}
	} while (a[applex][appley] != APPLE);


}
//---------------------------------------------------------
//�޸ķ���
void printCurrent(int x, int y, int z) {
	x += 13, y += 2;
	if (z == NECK) {
		pr(0x10FF10, BLACK, x * 2, y);
		printf("��");
	}
	if (z == NECK2) {
		pr(YELLOW, BLACK, x * 2, y);
		printf("��");
	}
	if (z == BODY) {
		pr(BLUE, BLACK, x * 2, y);
		printf("��");
	}
	if (z == BODY2) {
		pr(0x7F8030, BLACK, x * 2, y);
		printf("��");
	}
	if (z == HEAD) {
		pr(GREEN, BLACK, x * 2, y);
		printf("��");
	}
	if (z == HEAD2) {
		pr(YELLOW, BLACK, x * 2, y);
		printf("��");
	}
	if (z == APPLE) {
		pr(RED, BLACK, x * 2, y);
		printf("��");
	}
	if (z == NULL0) {
		pr(WHITE, BLACK, x * 2, y);
		printf("  ");
	}
	if (z == WALL) {
		pr(WHITE, BLACK, x * 2, y);
		printf("��");
	}
	a[x - 13][y - 2] = z;
}
//---------------------------------------------------------
//��Ϸ����
void runGame1() {
	rgbColor(WHITE, BLACK);
	system("cls");//����
	init3();
	init2();
//	N/2+1,M/3*2+1
	player1.length = 0, player1.head.x = N / 2 + 1, player1.head.y = M / 3 * 2 + 1, player1.neck.x = N / 2 + 1,
	player1.neck.y = M / 3 * 2;
	player1.remainingLength = 2;
	GameOver = 0;
	int bj = 0;
	printApple();//��ʼƻ��
	int sb = 2;
	while (game_ != 5 && game_ != 3) {
		bj++;
		if (bj == 8) {
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			CONSOLE_CURSOR_INFO CursorInfo;
			GetConsoleCursorInfo(handle, &CursorInfo);//��ȡ����̨�����Ϣ
			CursorInfo.bVisible = false; //���ؿ���̨���
			SetConsoleCursorInfo(handle, &CursorInfo);//���ÿ���̨���״̬
			CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };  // �����Ϣ
			bj = 0;
		}

		if (game_ == 4) {
			rgbColor(WHITE, BLACK);
			system("cls");//����
			init3();
			init2();
			printApple();//��ʼƻ��
//			N/2+1,M/3*2+1
			sb = 2;
			player1.length = 0, player1.head.x = N / 2 + 1, player1.head.y = M / 3 * 2 + 1, player1.neck.x = N / 2 + 1,
			player1.neck.y = M / 3 * 2;
			player1.remainingLength = 2;
			continue;
		}
		int lastKey = key1; //�ϴεĲ���
		if (sb) {
			sb--;
		} else {
			getKey(wasd); //������
		}
		if (key1 == 5) {
			pauseGame();
			key1 = lastKey;
		}

		if ((key1 == 1 && lastKey == 0) || (key1 == 0 && lastKey == 1) || (key1 == 2 && lastKey == 3) || (key1 == 3
		        && lastKey == 2)) {
			key1 = lastKey;
			//����Ƿ�������ô�Զ�����
		}
		int tx = dx[key1] + player1.head.x; //��һ������
		int ty = dy[key1] + player1.head.y;
		int nowa = a[tx][ty];
		if (nowa == BODY || nowa == WALL) {
			//��ǽ��ײ����
			game_ = 5;
			endGame();
			if (game_ == 6) return ;
			sb = 2;
			printApple();//��ʼƻ��
			continue;

		}
		player1.snackPosition.push({tx, ty}); //�µ�λ��

		printCurrent(tx, ty, HEAD); //�µ�ͷ��ͷ�µ�λ�ã�
		printCurrent(player1.head.x, player1.head.y, NECK); //ͷԭ����λ��
		printCurrent(player1.neck.x, player1.neck.y, BODY); //����ԭ����λ��
		player1.neck.x = player1.head.x, player1.neck.y = player1.head.y;
		player1.head.x = tx, player1.head.y = ty;

		pr(GREEN, BLACK, 13, 8);
		printf("��(%d,%d)   ", tx, ty); //ͷ��λ��
		if (nowa != APPLE && player1.remainingLength == 0) {
			gotoXY(3, 14);
			rgbColor(WHITE, BLACK);
			cout << "                 ";
			printCurrent(player1.snackPosition.front().x, player1.snackPosition.front().y, NULL0);
			player1.snackPosition.pop();
		} else {

			player1.length++;//��������
			if (player1.remainingLength != 0 && nowa != APPLE) {
				player1.remainingLength--;
				gotoXY(3, 14);
				cout << "ʣ�����峤�ȣ�" << player1.remainingLength << "   ";
			} else {
				printApple();
			}
			pr(BLUE, BLACK, 13, 10);
			cout << player1.length << "    ";
		}
	}

	return ;
}
//---------------------------------------------------------
//��Ϸ����
void runGame2() {
	rgbColor(WHITE, BLACK);
	system("cls");//����
	init3();
	init2();
//	N/2+1,M/3*2+1
	player1.length = 0, player1.head.x = N / 3 + 1, player1.head.y = M / 3 * 2 + 1, player1.neck.x = N / 3 + 1,
	player1.neck.y = M / 3 * 2;
	player1.remainingLength = 2;
	player2.length = 0, player2.head.x = N / 3 * 2 + 1, player2.head.y = M / 3 * 2 + 1, player2.neck.x = N / 3 * 2 + 1,
	player2.neck.y = M / 3 * 2;
	player2.remainingLength = 2;
	GameOver = 0;
	int bj = 0;
	printApple();//��ʼƻ��
	int sb = 2;
	rgbColor(BLUE, BLACK);
	gotoXY(16, 8);
	cout << player1.length << "   ";
	rgbColor(0x7F8030, BLACK);
	gotoXY(16, 10);
	cout << player2.length << "   ";
	while (game_ != 5 && game_ != 3) {
		bj++;
		if (bj == 8) {
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			CONSOLE_CURSOR_INFO CursorInfo;
			GetConsoleCursorInfo(handle, &CursorInfo);//��ȡ����̨�����Ϣ
			CursorInfo.bVisible = false; //���ؿ���̨���
			SetConsoleCursorInfo(handle, &CursorInfo);//���ÿ���̨���״̬
			CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };  // �����Ϣ
			bj = 0;
		}

		if (game_ == 4) {
			rgbColor(WHITE, BLACK);
			system("cls");//����
			init3();
			init2();
			printApple();//��ʼƻ��
//			N/2+1,M/3*2+1
			player1.length = 0, player1.head.x = N / 3 + 1, player1.head.y = M / 3 * 2 + 1, player1.neck.x = N / 3 + 1,
			player1.neck.y = M / 3 * 2;
			player1.remainingLength = 2;
			player2.length = 0, player2.head.x = N / 3 * 2 + 1, player2.head.y = M / 3 * 2 + 1, player2.neck.x = N / 3 * 2 + 1,
			player2.neck.y = M / 3 * 2;
			player2.remainingLength = 2;
			sb = 2;
			continue;

		}
		int lastKey1 = key1; //�ϴεĲ���
		int lastKey2 = key2; //�ϴεĲ���
		if (sb) {
			sb--;
		} else {
			getKey(wasd); //������
		}
		if (key1 == 5) {
			pauseGame();
			key1 = lastKey1;
		}

		if ((key1 == 1 && lastKey1 == 0) || (key1 == 0 && lastKey1 == 1) || (key1 == 2 && lastKey1 == 3) || (key1 == 3
		        && lastKey1 == 2)) {
			key1 = lastKey1;
			//����Ƿ�������ô�Զ�����
		}
		if ((key2 == 1 && lastKey2 == 0) || (key2 == 0 && lastKey2 == 1) || (key2 == 2 && lastKey2 == 3) || (key2 == 3
		        && lastKey2 == 2)) {
			key2 = lastKey2;
			//����Ƿ�������ô�Զ�����
		}
		int tx1 = dx[key1] + player1.head.x; //��һ������
		int ty1 = dy[key1] + player1.head.y;
		int nowa1 = a[tx1][ty1];
		int tx2 = dx[key2] + player2.head.x; //��һ������
		int ty2 = dy[key2] + player2.head.y;
		int nowa2 = a[tx2][ty2];
		if (nowa1 == BODY || 0 || nowa1 == BODY2 || nowa1 == NECK2 || nowa1 == WALL) {
			win = ((nowa2 == BODY || 0 || nowa2 == BODY || nowa2 == NECK || nowa2 == WALL) ? 0 : 2);
			//��ǽ��ײ����
			game_ = 5;
			endGame();
			if (game_ == 6) return ;
			sb = 2;
			printApple();//��ʼƻ��
			continue;
		}
		if (nowa2 == BODY2 || 0 || nowa2 == BODY || nowa2 == NECK || nowa2 == WALL) {
			win = 1;
			//��ǽ��ײ����
			game_ = 5;
			endGame();
			if (game_ == 6) return ;
			sb = 2;
			printApple();//��ʼƻ��
			continue;
		}

		player1.snackPosition.push({tx1, ty1}); //�µ�λ��
		player2.snackPosition.push({tx2, ty2}); //�µ�λ��

		printCurrent(tx1, ty1, HEAD); //�µ�ͷ��ͷ�µ�λ�ã�
		printCurrent(player1.head.x, player1.head.y, NECK); //ͷԭ����λ��
		printCurrent(player1.neck.x, player1.neck.y, BODY); //����ԭ����λ��
		player1.neck.x = player1.head.x, player1.neck.y = player1.head.y;
		player1.head.x = tx1, player1.head.y = ty1;

		printCurrent(tx2, ty2, HEAD2); //�µ�ͷ��ͷ�µ�λ�ã�
		printCurrent(player2.head.x, player2.head.y, NECK2); //ͷԭ����λ��
		printCurrent(player2.neck.x, player2.neck.y, BODY2); //����ԭ����λ��
		player2.neck.x = player2.head.x, player2.neck.y = player2.head.y;
		player2.head.x = tx2, player2.head.y = ty2;
		if (tx1 == tx2 && ty1 == ty2 || nowa1 == HEAD2 || nowa2 == HEAD) {
			win = 0;
			game_ = 5;
			endGame();
			if (game_ == 6) return ;
			sb = 2;
			printApple();//��ʼƻ��
			continue;
		}
		if (nowa1 != APPLE && player1.remainingLength == 0) {
			gotoXY(1, 14);
			rgbColor(WHITE, BLACK);
			cout << "                        ";
			printCurrent(player1.snackPosition.front().x, player1.snackPosition.front().y, NULL0);
			player1.snackPosition.pop();
		} else {

			player1.length++;//��������
			if (player1.remainingLength != 0 && nowa1 != APPLE) {
				player1.remainingLength--;
				rgbColor(BLUE, BLACK);
				gotoXY(1, 14);
				cout << "���1ʣ�����峤�ȣ�" << player1.remainingLength << "   ";
			} else {
				printApple();
			}
			rgbColor(BLUE, BLACK);
			gotoXY(16, 8);
			cout << player1.length << "   ";
		}
		if (nowa2 != APPLE && player2.remainingLength == 0) {
			gotoXY(1, 15);
			rgbColor(WHITE, BLACK);
			cout << "                        ";
			printCurrent(player2.snackPosition.front().x, player2.snackPosition.front().y, NULL0);
			player2.snackPosition.pop();
		} else {

			player2.length++;//��������
			if (player2.remainingLength != 0 && nowa2 != APPLE) {
				player2.remainingLength--;
				rgbColor(0x7F8030, BLACK);
				gotoXY(1, 15);
				cout << "���2ʣ�����峤�ȣ�" << player2.remainingLength << "   ";

			} else {
				printApple();
			}

			rgbColor(0x7F8030, BLACK);
			gotoXY(16, 10);
			cout << player2.length << "   ";
		}
	}

	return ;
}
//---------------------------------------------------------
//��Ϸ����
void runGame3() {
	rgbColor(WHITE, BLACK);
	system("cls");//����
	init3();
	init2();
//	N/2+1,M/3*2+1
	player1.length = 0, player1.head.x = N / 3 + 1, player1.head.y = M / 3 * 2 + 1, player1.neck.x = N / 3 + 1,
	player1.neck.y = M / 3 * 2;
	player1.remainingLength = 2;
	player2.length = 0, player2.head.x = N / 3 * 2 + 1, player2.head.y = M / 3 * 2 + 1, player2.neck.x = N / 3 * 2 + 1,
	player2.neck.y = M / 3 * 2;
	player2.remainingLength = 2;
	GameOver = 0;
	int bj = 0;
	printApple();//��ʼƻ��
	int sb = 2;
	rgbColor(BLUE, BLACK);
	gotoXY(16, 8);
	cout << player1.length << "   ";
	rgbColor(0x7F8030, BLACK);
	gotoXY(16, 10);
	nandu = 1;
	cout << player2.length << "   ";
	while (game_ != 5 && game_ != 3) {
		bj++;
		setNan();
		if (bj == 8) {
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			CONSOLE_CURSOR_INFO CursorInfo;
			GetConsoleCursorInfo(handle, &CursorInfo);//��ȡ����̨�����Ϣ
			CursorInfo.bVisible = false; //���ؿ���̨���
			SetConsoleCursorInfo(handle, &CursorInfo);//���ÿ���̨���״̬
			CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };  // �����Ϣ
			bj = 0;
		}

		if (game_ == 4) {
			rgbColor(WHITE, BLACK);
			system("cls");//����
			init3();
			init2();
			printApple();//��ʼƻ��
//			N/2+1,M/3*2+1
			player1.length = 0, player1.head.x = N / 3 + 1, player1.head.y = M / 3 * 2 + 1, player1.neck.x = N / 3 + 1,
			player1.neck.y = M / 3 * 2;
			player1.remainingLength = 2;
			player2.length = 0, player2.head.x = N / 3 * 2 + 1, player2.head.y = M / 3 * 2 + 1, player2.neck.x = N / 3 * 2 + 1,
			player2.neck.y = M / 3 * 2;
			player2.remainingLength = 2;
			sb = 2;
			continue;

		}
		int lastKey1 = key1; //�ϴεĲ���
		int lastKey2 = key2; //�ϴεĲ���
		if (sb) {
			sb--;
		} else {
			getKey(wasd); //������
		}
		if (key1 == 5) {
			pauseGame();
			key1 = lastKey1;
		}

		if ((key1 == 1 && lastKey1 == 0) || (key1 == 0 && lastKey1 == 1) || (key1 == 2 && lastKey1 == 3) || (key1 == 3
		        && lastKey1 == 2)) {
			key1 = lastKey1;
			//����Ƿ�������ô�Զ�����
		}
		if ((key2 == 1 && lastKey2 == 0) || (key2 == 0 && lastKey2 == 1) || (key2 == 2 && lastKey2 == 3) || (key2 == 3
		        && lastKey2 == 2)) {
			key2 = lastKey2;
			//����Ƿ�������ô�Զ�����
		}
		int tx1 = dx[key1] + player1.head.x; //��һ������
		int ty1 = dy[key1] + player1.head.y;
		int nowa1 = a[tx1][ty1];
		int tx2 = dx[key2] + player2.head.x; //��һ������
		int ty2 = dy[key2] + player2.head.y;
		int nowa2 = a[tx2][ty2];
		if (nowa1 == BODY || 0 || nowa1 == BODY2 || nowa1 == NECK2 || nowa1 == WALL) {
			win = ((nowa2 == BODY || 0 || nowa2 == BODY || nowa2 == NECK || nowa2 == WALL) ? 0 : 2);
			//��ǽ��ײ����
			game_ = 5;
			endGame();
			if (game_ == 6) return ;
			sb = 2;
			printApple();//��ʼƻ��
			continue;
		}
		if (nowa2 == BODY2 || 0 || nowa2 == BODY || nowa2 == NECK || nowa2 == WALL) {
			win = 1;
			//��ǽ��ײ����
			game_ = 5;
			endGame();
			if (game_ == 6) return ;
			sb = 2;
			printApple();//��ʼƻ��
			continue;
		}

		player1.snackPosition.push({tx1, ty1}); //�µ�λ��
		player2.snackPosition.push({tx2, ty2}); //�µ�λ��

		printCurrent(tx1, ty1, HEAD); //�µ�ͷ��ͷ�µ�λ�ã�
		printCurrent(player1.head.x, player1.head.y, NECK); //ͷԭ����λ��
		printCurrent(player1.neck.x, player1.neck.y, BODY); //����ԭ����λ��
		player1.neck.x = player1.head.x, player1.neck.y = player1.head.y;
		player1.head.x = tx1, player1.head.y = ty1;

		printCurrent(tx2, ty2, HEAD2); //�µ�ͷ��ͷ�µ�λ�ã�
		printCurrent(player2.head.x, player2.head.y, NECK2); //ͷԭ����λ��
		printCurrent(player2.neck.x, player2.neck.y, BODY2); //����ԭ����λ��
		player2.neck.x = player2.head.x, player2.neck.y = player2.head.y;
		player2.head.x = tx2, player2.head.y = ty2;
		if (tx1 == tx2 && ty1 == ty2 || nowa1 == HEAD2 || nowa2 == HEAD) {
			win = 0;
			game_ = 5;
			endGame();
			if (game_ == 6) return ;
			sb = 2;
			printApple();//��ʼƻ��
			continue;
		}
		if (nowa1 != APPLE && player1.remainingLength == 0) {
			gotoXY(1, 14);
			rgbColor(WHITE, BLACK);
			cout << "                        ";
			printCurrent(player1.snackPosition.front().x, player1.snackPosition.front().y, NULL0);
			player1.snackPosition.pop();
		} else {

			player1.length++;//��������
			if (player1.remainingLength != 0 && nowa1 != APPLE) {
				player1.remainingLength--;
				rgbColor(BLUE, BLACK);
				gotoXY(1, 14);
				cout << "���1ʣ�����峤�ȣ�" << player1.remainingLength << "   ";
			} else {
				printApple();
			}
			rgbColor(BLUE, BLACK);
			gotoXY(16, 8);
			cout << player1.length << "   ";
		}
		if (nowa2 != APPLE && player2.remainingLength == 0) {
			gotoXY(1, 15);
			rgbColor(WHITE, BLACK);
			cout << "                        ";
			printCurrent(player2.snackPosition.front().x, player2.snackPosition.front().y, NULL0);
			player2.snackPosition.pop();
		} else {

			player2.length++;//��������
			if (player2.remainingLength != 0 && nowa2 != APPLE) {
				player2.remainingLength--;
				rgbColor(0x7F8030, BLACK);
				gotoXY(1, 15);
				cout << "   AIʣ�����峤�ȣ�" << player2.remainingLength << "   ";

			} else {
				printApple();
			}

			rgbColor(0x7F8030, BLACK);
			gotoXY(16, 10);
			cout << player2.length << "   ";
		}
	}

	return ;
}
//---------------------------------------------------------
//��ʼ��Ϸ
void restartGame() {
	int tiSpeed = 80;

	int xx[10] = {0, 23, 33, 51};
	string text1[10] = {"", "1�����ˡ�", "2����� VS ��ҡ�", "3����� VS AI��"};
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);//��ȡ����̨�����Ϣ
	CursorInfo.bVisible = false; //���ؿ���̨���
	SetConsoleCursorInfo(handle, &CursorInfo);//���ÿ���̨���״̬
	CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };  // �����Ϣ
	gotoXY(20, 3);
	cout << "---=== = ";
	rgbColor(YELLOW, BLACK);
	cout << "̰ �� ��";
	rgbColor(WHITE, BLACK);
	cout << " = ===---";
	gotoXY(22, 6);
	cout << "�ٶ�(0~100)��80";
	gotoXY(22, 7);

	cout << "(�����������)";
	gotoXY(22, 8);
	cout << "(����Ϊ�Ӽ�10)";
	gotoXY(22, 9);
	cout << "(����Ϊ�Ӽ�1)";
	gotoXY(22, 11);
	cout << "��Ϸ��ʽ��1�����ˡ�|2����� VS ��ҡ�|3����� VS AI��";
	gotoXY(22, 12);
	cout << "(�� 1 2 3 �л�)";
	gotoXY(22, 15);
	cout << "���ո����ʼ��Ϸ";
	int color_1 = YELLOW, hh = 1;
	while (true) {
		if (KEYDOWN(VK_SPACE)) break;
		if (KEYDOWN(VK_DOWN)) tiSpeed -= 10;
		if (KEYDOWN(VK_LEFT)) tiSpeed -= 1;
		if (KEYDOWN(VK_UP)) tiSpeed += 10;
		if (KEYDOWN(VK_RIGHT)) tiSpeed += 1;
		if (KEYDOWN('1')) mode = 1;
		if (KEYDOWN('2')) mode = 2;
		if (KEYDOWN('3')) mode = 3;
		if (tiSpeed > 100) tiSpeed = 100;
		if (tiSpeed < 0) tiSpeed = 0;
		rgbColor(PURPLE, BLACK);
		gotoXY(35, 6);
		cout << tiSpeed << "  ";



		rgbColor(WHITE, BLACK);
		gotoXY(22, 11);
		cout << "��Ϸ��ʽ��1�����ˡ�|2����� VS ��ҡ�|3����� VS AI��";
		gotoXY(xx[mode] + 9, 11);
		rgbColor(YELLOW, BLACK);
		cout << text1[mode];



		Sleep(120);
		gotoXY(29, 3);
		rgbColor(color_1, BLACK);

		cout << "̰ �� ��";
		color_1 += (random(0x00, 0x30) * 0x010000+random(0x00, 0x30) * 0x000100+random(0x00, 0x30) * 0x010000) * hh;
		if (color_1 > 0xFFFFFF) {
			hh = -1;
		}
		if (color_1 < 0x333333) {
			hh = 1;
		}
	}
	player1.speed = player1.Xspeed = ((double)(100 - tiSpeed + 1)) / 100;
	player1.Sspeed = tiSpeed;
	player2.speed = player2.Xspeed = ((double)(100 - tiSpeed + 1)) / 100;
	player2.Sspeed = tiSpeed;
}
//---------------------------------------------------------
//�����Ѷ�
void setNan() {
	gotoXY(N * 2 + 38, 16);
	rgbColor(GREY, BLACK);
	printf("�� �� �� �� ��");
	gotoXY(N * 2 + 35 + nandu * 3, 16);
	rgbColor(WHITE, BLACK);
	string st[15] = {"��", "��", "��", "��", "��"};
	cout << st[nandu - 1];
}
//---------------------------------------------------------
//��ͣ��Ϸ
void pauseGame() {
	gotoXY(30, 5);
	rgbColor(0xDFDFFA, 0x929292);
	cout << " ����ͣ��Ϸ ";
	gotoXY(30, 7);
	cout << "  Esc�� ������Ϸ ";
	gotoXY(30, 8);
	cout << "  �ո�� ������ҳ ";
	gotoXY(30, 9);
	cout << "  �س��� ���¿�ʼ ";
	while (KEYDOWN(VK_RETURN) || KEYDOWN(VK_ESCAPE));
	while (1) {
		if (KEYDOWN(27)) {
			init2();
			while (KEYDOWN(27)) {

			}
			return ;
		}
		if (KEYDOWN(VK_SPACE)) {
			int result = MessageBox( NULL, TEXT("�Ƿ񷵻���ҳ"), TEXT("̰���� ��ʾ"), MB_ICONINFORMATION | MB_YESNO);
			while (KEYDOWN(VK_RETURN));
			if (result == IDNO) continue;
			rgbColor(WHITE, BLACK);
			system("cls");
			game_ = 3;
			return ;
		}
		if (KEYDOWN(VK_RETURN)) {

			int result = MessageBox( NULL, TEXT("�Ƿ����¿�ʼ"), TEXT("̰���� ��ʾ"), MB_ICONINFORMATION | MB_YESNO);
			while (KEYDOWN(VK_RETURN));
			if (result == IDNO) continue;
			rgbColor(WHITE, BLACK);
			system("cls");
			game_ = 4;
			return ;

		}
		if (mode == 3) {
			for (int i = 1; i <= 5; i++) {
				if (KEYDOWN('0' + i)) {
					nandu = i;
					setNan();
				}
			}
		}
	}
}
//---------------------------------------------------------
//������Ϸ
void endGame() {
	gotoXY(30, 5);
	rgbColor(0xDFDFFA, 0x929292);
	if (mode != 2) {
		if ((mode == 1 && player1.length < N * M - 1) || (mode == 3 && win != 1)) {
			cout << " Game Over ";
		} else {
			cout << " ʤ���� ";
		}
	} else {
		if (win == 0) cout << " ͬ���ھ� ";
		else cout << " ���" << win << "ʤ����";
	}

	while (KEYDOWN(VK_SPACE) || KEYDOWN(VK_ESCAPE));
	gotoXY(30, 7);
	cout << "  �ո�� ������ҳ ";
	gotoXY(30, 8);
	cout << "  �س��� ���¿�ʼ ";
	gotoXY(30, 9);
	if (player1.length != 299 && mode == 1) {
		cout << "  P�� ����渴�� ";
		gotoXY(30, 10);
	}
	if (mode == 2) {

		cout << "  P�� ���¿�ʼ������ʤ����ҳ��ȣ�";
		gotoXY(30, 10);
	}
	rgbColor(0xAAAA22, 0x929292);
	cout << "  Esc�� �˳���Ϸ ";
	while (1) {
		if (KEYDOWN(VK_SPACE)) {
			int result = MessageBox( NULL, TEXT("�Ƿ񷵻���ҳ"), TEXT("̰���� ��ʾ"), MB_ICONINFORMATION | MB_YESNO);
			while (KEYDOWN(VK_RETURN));
			if (result == IDNO) continue;
			game_ = 5;
			init2();
			return ;
		}
		if ((KEYDOWN('p') || KEYDOWN('P')) && player1.length != 299 && mode == 1) {
			advertisement();
			init3();
			init2();
			player1.remainingLength = player1.length + player1.remainingLength;
			player1.length = 0, player1.head.x = N / 2 + 1, player1.head.y = M / 3 * 2 + 1, player1.neck.x = N / 2 + 1,
			player1.neck.y = M / 3 * 2;
			return ;
		}
		if ((KEYDOWN('p') || KEYDOWN('P')) && player1.length != 299 && mode == 2) {
			init3();
			init2();
			if (win == 1) {
				player1.remainingLength = player1.length + player1.remainingLength;
				player2.remainingLength = 2;
			} else if (win == 2) {
				player2.remainingLength = player2.length + player2.remainingLength;
				player1.remainingLength = 2;
			} else {
				player2.remainingLength = player2.length + player2.remainingLength;
				player1.remainingLength = player1.length + player1.remainingLength;
			}
			player1.length = 0, player1.head.x = N / 3 + 1, player1.head.y = M / 3 * 2 + 1, player1.neck.x = N / 3 + 1,
			player1.neck.y = M / 3 * 2;
			player2.length = 0, player2.head.x = N / 3 * 2 + 1, player2.head.y = M / 3 * 2 + 1, player2.neck.x = N / 3 * 2 + 1,
			player2.neck.y = M / 3 * 2;
			return ;
		}
		if (KEYDOWN(VK_RETURN)) {
			int result = MessageBox( NULL, TEXT("�Ƿ����¿�ʼ"), TEXT("̰���� ��ʾ"), MB_ICONINFORMATION | MB_YESNO);
			while (KEYDOWN(VK_RETURN));
			if (result == IDNO) continue;
			rgbColor(WHITE, BLACK);
			system("cls");
			game_ = 4;
			return ;
		}
		if (KEYDOWN(27)) {
			int result = MessageBox( NULL, TEXT("�Ƿ��˳�"), TEXT("̰���� ��ʾ"), MB_ICONINFORMATION | MB_YESNO);
			while (KEYDOWN(27));
			if (result == IDNO) continue;
			rgbColor(WHITE, BLACK);
			system("cls");
			game_ = 6;
			return ;

		}
	}
}
//---------------------------------------------------------
//���Ź��
void advertisement() {
	rgbColor(WHITE, BLACK);
	system("cls");
	GameOver++;
	clock_t start, finish;
	double totaltime = 0; //��ʱ
	start = clock();
	while (1) {
		rgbColor(WHITE, BLACK);
		cout << "                                                                   ";
		gotoXY(0, 0);
		printf("��� ʣ��%d��  ", (int)(3 * (GameOver + 2) - totaltime));
		gotoXY(1, random(1, 10));
		rgbColor(random(0x666666, 0xFFFFFF), BLACK);
		cout << "��á���";


		Sleep(200);
		finish = clock();
		totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
		if (totaltime >= 0 * (GameOver + 2)) {
//			system("start https://www.luogu.com.cn/user/1061745");
//			if(player1.Sspeed<60&&player1.length+player1.remainingLength==2) system("start https://netcut.cn/p/c7f40b836eceeeaa");
			gotoXY(0, 0);
			cout << "�س��� ������Ϸ      ";
			while (1) {
				if (KEYDOWN(VK_RETURN)) {
					while (KEYDOWN(VK_RETURN));
					return ;
				}
			}

		}

	}
}
