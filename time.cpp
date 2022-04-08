//=============================================================================
//
// �^�C������ [time.cpp]
// Author : ������
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "time.h"
#include "sprite.h"
#include "fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(50)	// �T�C�Y
#define TEXTURE_HEIGHT				(60)	// 
#define TEXTURE_MAX					(1)		// �e�N�X�`���̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/number.png",
};


static BOOL						g_Use;						// TRUE:�g���Ă���  FALSE:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�

static int						g_Time;						// �^�C��

static BOOL						g_Load = FALSE;

static int						count;						// �^�C�������炷���߂̃J�E���g�ϐ�
//=============================================================================
// ����������
//=============================================================================
HRESULT InitTime(void)
{
	ID3D11Device *pDevice = GetDevice();

	//�e�N�X�`������
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


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �^�C���̏�����
	g_Use = TRUE;
	g_w = TEXTURE_WIDTH;
	g_h = TEXTURE_HEIGHT;
	g_Pos = { 500.0f, 60.0f, 0.0f };
	g_TexNo = 0;

	g_Time = TIME_MAX;	// �^�C���̏�����
	count = 60;			// 60FPS�Ȃ̂�60�ŏ�����

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
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
// �X�V����
//=============================================================================
void UpdateTime(void)
{
	if (GetMode() == MODE_GAME)
	{
		// �^�C���J�E���g�����炷����
		if (count <= 0)
		{
			AddTime(-1);	// �^�C�����P���炷
			count = 60;		// 60FPS�Ȃ̂�60������
		}

		count--;			// ���t���[���J�E���g�ϐ�������


		// �������Ԃ�0�ɂȂ�����
		if (g_Time <= 0)
		{
			// �Q�[���I�[�o�[��ʂɑJ��
			SetFade(FADE_OUT, MODE_GAMEOVER);
			g_Time = 0;			// �}�C�i�X�\���ɂȂ�Ȃ��悤��0�ɌŒ肳����
		}

	}


#ifdef _DEBUG	// �f�o�b�O����\������
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTime(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	// ��������������
	int number = g_Time;
	//3���ڂ��������߂�if���g���ĕ����Ă���
	if (g_Time >= 100)
	{
		for (int i = 0; i < TIME_DIGIT; i++)
		{
			// ����\�����錅�̐���
			float x = (float)(number % 10);

			// �^�C���̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Pos.x - g_w * i;	// �^�C���̕\���ʒuX
			float py = g_Pos.y;				// �^�C���̕\���ʒuY
			float pw = g_w;					// �^�C���̕\����
			float ph = g_h;					// �^�C���̕\������

			float tw = 1.0f / 10;			// �e�N�X�`���̕�
			float th = 1.0f / 1;			// �e�N�X�`���̍���
			float tx = x * tw;				// �e�N�X�`���̍���X���W
			float ty = 0.0f;				// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);

			// ���̌���
			number /= 10;
		}

	}
	else
	{
		for (int i = 0; i < TIME_DIGIT - 1; i++)
		{
			// ����\�����錅�̐���
			float x = (float)(number % 10);

			// �^�C���̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Pos.x - g_w * i;	// �^�C���̕\���ʒuX
			float py = g_Pos.y;				// �^�C���̕\���ʒuY
			float pw = g_w;					// �^�C���̕\����
			float ph = g_h;					// �^�C���̕\������

			float tw = 1.0f / 10;			// �e�N�X�`���̕�
			float th = 1.0f / 1;			// �e�N�X�`���̍���
			float tx = x * tw;				// �e�N�X�`���̍���X���W
			float ty = 0.0f;				// �e�N�X�`���̍���Y���W

			if (g_Time <= 30)
			{
				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
			}
			else
			{
				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
			}

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);

			// ���̌���
			number /= 10;
		}

	}
}


//=============================================================================
// �^�C�������Z�E���Z����
// ����:add :�ǉ����鎞�ԁB�}�C�i�X���\
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
// �^�C�������擾
//=============================================================================
int GetTime(void)
{
	return g_Time;
}
