//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 星克昌
//
//=============================================================================
#pragma once


//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VALUE_MOVE_CAMERA	(1.0f)				// カメラの移動量
#define	VALUE_ROTATE_CAMERA	(XM_PI * 0.01f)		// カメラの回転量

struct CAMERA
{
	XMFLOAT4X4			mtxView;				// ビューマトリックス
	XMFLOAT4X4			mtxInvView;				// ビューマトリックス
	XMFLOAT4X4			mtxProjection;			// プロジェクションマトリックス
	
	XMFLOAT3			pos;					// カメラの視点(位置)
	XMFLOAT3			at;						// カメラの注視点
	XMFLOAT3			up;						// カメラの上方向ベクトル
	XMFLOAT3			rot;					// カメラの回転
	
	float				len;					// カメラの視点と注視点の距離

	INTERPOLATION_DATA	*tbl_adr;				// アニメデータのテーブル先頭アドレス
	int					tbl_size;				// 登録したテーブルのレコード総数
	float				move_time;				// 実行時間

};

enum CAMERA_MODE
{
	PLAYER_CAM_MODE,			// プレイヤー視点モード
	VIEW_CAM_MODE,				// 上空からの見下ろしモード
	ENEMY_CAM_MODE,				// 死神視点モード
	ALPHA_ENEMY_CAM_MODE,		// 透明死神視点モード

	CAMERA_MODE_MAX				// カメラのモードMAX数
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);

CAMERA *GetCamera(void);

void SetCameraAT(XMFLOAT3 pos);
