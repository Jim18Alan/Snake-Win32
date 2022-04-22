#include <windows.h>

#define SIZESNAKE 20

#define CUERPO 	2
#define CABEZA 	1
#define COLA 	0

#define IZQ 	1
#define DER 	2
#define ARRIVA 	3
#define ABAJO 	4


struct pos{
	int x;
	int y;
};
typedef struct pos POS;
 
struct pedacito{
	POS pos;
	int tipo;
	int dir;
};
typedef struct pedacito PEDACITO;

PEDACITO * nuevoSnake(int);
void drawSnake(HDC hdc, const PEDACITO *, RECT);   //=el rect nos srive para conocer el extremo de la ventana



/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;
	static PEDACITO * snake = NULL;
	static int sizeSnake = 5;
	
	switch(Message) {
		case WM_CREATE: {
			snake = nuevoSnake(sizeSnake);

			break;
		}
		case WM_PAINT: {
			hdc = BeginPaint(hwnd, &ps);
			drawSnake(hdc, snake,rect );
			EndPaint(hwnd, &ps);
			break;
		}
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Caption",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		640, /* width */
		480, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}


PEDACITO * nuevoSnake(int sizeSnake){
	PEDACITO * snake = NULL;
	int i; 

	if(sizeSnake < 2)
		sizeSnake = 2;
	snake = (PEDACITO *) malloc (sizeof( PEDACITO) * sizeSnake);
	if(snake == NULL){
		MessageBox(NULL, "Sin memoria", "Error", MB_OK | MB_ICONERROR );
		exit(0);
	}

	snake[COLA].tipo = COLA;
	snake[COLA].pos.x = 1;
	snake[COLA].pos.y = 1;
	snake[COLA].dir = DER;

	for (i = 1; i < sizeSnake-1; i++){
		snake[i].tipo = CUERPO;
		snake[i].pos.x = i + 1;
		snake[i].pos.y = 1;
	}

	snake[i].tipo = CABEZA;

	snake[i].pos.x = i + 1;
	snake[i].pos.y = 1;
	snake[i].dir = DER;

	return snake;

}


void drawSnake(HDC hdc, const PEDACITO * snake, RECT rect){
	int  i = 1;

	switch(snake[COLA].dir){
		case DER:
			MoveToEx(hdc, snake[COLA].pos.x * SIZESNAKE  + SIZESNAKE,
					      snake[COLA].pos.y * SIZESNAKE, NULL);
			LineTo(hdc, snake[COLA].pos.x * SIZESNAKE ,
						snake[COLA].pos.y *  SIZESNAKE + SIZESNAKE/2);
			LineTo(hdc, snake[COLA].pos.x * SIZESNAKE + SIZESNAKE,
						snake[COLA].pos.y *  SIZESNAKE + SIZESNAKE);
			LineTo(hdc, snake[COLA].pos.x * SIZESNAKE  + SIZESNAKE,
						snake[COLA].pos.y *  SIZESNAKE);
			break;
		case IZQ:
			break;
		case ARRIVA:
			break;
		case  ABAJO:
			break;
	}
	while(snake[i].tipo != CABEZA){
		RoundRect(hdc, snake[i].pos.x * SIZESNAKE, snake[i].pos.y * SIZESNAKE,
					   snake[i].pos.x * SIZESNAKE + SIZESNAKE, snake[i].pos.y * SIZESNAKE + SIZESNAKE, 5,5);
		i++;
	}
	RoundRect(hdc, snake[i].pos.x * SIZESNAKE, snake[i].pos.y * SIZESNAKE,
					   snake[i].pos.x * SIZESNAKE + SIZESNAKE, snake[i].pos.y * SIZESNAKE + SIZESNAKE, 5,5);
	

}

