//=============================================================================
//
// �t�F���X�̃��f������ [fence.h]
// Author : ������
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_FENCE		(25)				// �t�F���X�̐�

#define	FENSE_SIZE		(5.0f)				// �����蔻��̑傫��
#define FENCE_OFFSET_Y	(24.0f)				// �t�F���X��n�ʂɂ��킹��

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct FENCE
{
	XMFLOAT4X4			mtxWorld;			// ���[���h�}�g���b�N�X
	XMFLOAT3			pos;				// ���f���̈ʒu
	XMFLOAT3			rot;				// ���f���̌���(��])
	XMFLOAT3			scl;				// ���f���̑傫��(�X�P�[��)

	BOOL				use;
	BOOL				load;
	DX11_MODEL			model;				// ���f�����
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// ���f���̐F

	float				size;				// �����蔻��̑傫��
	int					shadowIdx;			// �e�̃C���f�b�N�X�ԍ�
	
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitFence(void);
void UninitFence(void);
void UpdateFence(void);
void DrawFence(void);

FENCE *GetFence(void);
