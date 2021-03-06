//=============================================================================
//
// タイム処理 [time.cpp]
// Author : 星克昌
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "time.h"
#include "sprite.h"
#include "fade.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(50)	// サイズ
#define TEXTURE_HEIGHT				(60)	// 
#define TEXTURE_MAX					(1)		// テクスチャの数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/number.png",
};


static BOOL						g_Use;						// TRUE:使っている  FALSE:未使用
static float					g_w, g_h;					// 幅と高さ
static XMFLOAT3					g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号

static int						g_Time;						// タイム

static BOOL						g_Load = FALSE;

static int						count;						// タイムを減らすためのカウント変数
static BOOL						g_Timer_Switch;				// 時間を止めるスイッチ デバッグ用

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTime(void)
{
	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// タイムの初期化
	g_Use = TRUE;
	g_w = TEXTURE_WIDTH;
	g_h = TEXTURE_HEIGHT;
	g_Pos = { 500.0f, 60.0f, 0.0f };
	g_TexNo = 0;

	g_Time = TIME_MAX;	// タイムの初期化
	count = 60;			// 60FPSなので60で初期化

	g_Timer_Switch = FALSE;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTime(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTime(void)
{
	if (GetMode() == MODE_GAME)
	{

#ifdef _DEBUG

		// タイマースイッチ デバッグ用
		g_Timer_Switch = g_Timer_Switch % 2;

		if (GetKeyboardTrigger(DIK_4))
		{
			g_Timer_Switch++;
		}
#endif

		if (g_Timer_Switch == FALSE)
		{
			// タイムカウントを減らす処理
			if (count <= 0)
			{
				AddTime(-1);	// タイムを１減らす
				count = 60;		// 60FPSなので60を入れる
			}
		}

		count--;				// 毎フレームカウント変数を引く


		// 制限時間が0になったら
		if (g_Time <= 0)
		{
			// ゲームオーバー画面に遷移
			SetFade(FADE_OUT, MODE_GAMEOVER);
			g_Time = 0;			// マイナス表示にならないように0に固定させる
		}

	}


#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTime(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	// 桁数分処理する
	int number = g_Time;
	//3桁目を消すためにifを使って分けている
	if (g_Time >= 100)
	{
		for (int i = 0; i < TIME_DIGIT; i++)
		{
			// 今回表示する桁の数字
			float x = (float)(number % 10);

			// タイムの位置やテクスチャー座標を反映
			float px = g_Pos.x - g_w * i;	// タイムの表示位置X
			float py = g_Pos.y;				// タイムの表示位置Y
			float pw = g_w;					// タイムの表示幅
			float ph = g_h;					// タイムの表示高さ

			float tw = 1.0f / 10;			// テクスチャの幅
			float th = 1.0f / 1;			// テクスチャの高さ
			float tx = x * tw;				// テクスチャの左上X座標
			float ty = 0.0f;				// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

			// 次の桁へ
			number /= 10;
		}

	}
	else
	{
		for (int i = 0; i < TIME_DIGIT - 1; i++)
		{
			// 今回表示する桁の数字
			float x = (float)(number % 10);

			// タイムの位置やテクスチャー座標を反映
			float px = g_Pos.x - g_w * i;	// タイムの表示位置X
			float py = g_Pos.y;				// タイムの表示位置Y
			float pw = g_w;					// タイムの表示幅
			float ph = g_h;					// タイムの表示高さ

			float tw = 1.0f / 10;			// テクスチャの幅
			float th = 1.0f / 1;			// テクスチャの高さ
			float tx = x * tw;				// テクスチャの左上X座標
			float ty = 0.0f;				// テクスチャの左上Y座標

			if (g_Time <= 30)
			{
				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
			}
			else
			{
				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
			}

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

			// 次の桁へ
			number /= 10;
		}

	}
}


//=============================================================================
// タイムを加算・減算する
// 引数:add :追加する時間。マイナスも可能
//=============================================================================
void AddTime(int add)
{
	g_Time += add;
	if (g_Time > TIME_MAX)
	{
		g_Time = TIME_MAX;
	}

}

//=============================================================================
// タイム情報を取得
//=============================================================================
int GetTime(void)
{
	return g_Time;
}

