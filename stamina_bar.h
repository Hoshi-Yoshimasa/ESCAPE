//=============================================================================
//
// スタミナゲージ処理 [stamina_bar.h]
// Author : 星克昌
//
//=============================================================================
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// スタミナバー構造体
struct STAMINA_BAR
{
	float					w, h;					// 幅と高さ
	XMFLOAT3				pos;					// スタミナバーの座標
	XMFLOAT3				rot;					// スタミナバーの回転量
	int						texNo;					// 何番目のテクスチャーを使用するのか
};



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitStaminaBar(void);
void UninitStaminaBar(void);
void UpdateStaminaBar(void);
void DrawStaminaBar(void);
void SetStaminaBar(void);


