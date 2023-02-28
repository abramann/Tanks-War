#ifndef _INPUT_H
#define _INPUT_H
#define DIRECTINPUT_VERSION 0x0800
#include "always.h"
#include <dinput.h>

#define ZERO_KEY DIK_0
#define ONE_KEY DIK_1
#define TWO_KEY DIK_2
#define THREE_KEY DIK_3
#define FOUR_KEY DIK_4
#define FIVE_KEY DIK_5
#define SIX_KEY DIK_6
#define SEVEN_KEY DIK_7
#define EIGHT_KEY DIK_8
#define NINE_KEY DIK_9
#define A_KEY DIK_A
#define B_KEY DIK_B
#define C_KEY DIK_C
#define D_KEY DIK_D
#define E_KEY DIK_E
#define F_KEY DIK_F
#define G_KEY DIK_G
#define H_KEY DIK_H
#define I_KEY DIK_I
#define J_KEY DIK_J
#define K_KEY DIK_K
#define L_KEY DIK_L
#define M_KEY DIK_M
#define N_KEY DIK_N
#define O_KEY DIK_O
#define P_KEY DIK_P
#define Q_KEY DIK_Q
#define R_KEY DIK_R
#define S_KEY DIK_S
#define T_KEY DIK_T
#define U_KEY DIK_U
#define V_KEY DIK_V
#define W_KEY DIK_W
#define X_KEY DIK_X
#define Y_KEY DIK_Y
#define Z_KEY DIK_Z
#define ENTER_KEY DIK_RETURN
#define ESCAPE_KEY DIK_ESCAPE
#define SPACE_KEY DIK_SPACE
#define BACKSPACE_KEY DIK_BACKSPACE
#define RSHIFT_KEY DIK_RSHIFT
#define LSHIFT_KEY DIK_LSHIFT

class Input
{

public:
	
	Input();
	~Input();
	bool initialize(HINSTANCE hInstance, HWND hWnd);
	void scan();
	void keyDown(BYTE k);
	void release();
	void reset();
	bool isKeyIn(BYTE k);
	bool anyKeyPressed();

private:

	LPDIRECTINPUT8 m_lpDirectInput;
	LPDIRECTINPUTDEVICE8 m_lpKeyboard, m_lpMouse;
	BYTE m_keyboardState[256];

};


#endif