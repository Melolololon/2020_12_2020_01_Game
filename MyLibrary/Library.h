#pragma once
#include<Windows.h>
#include<ctime>
#include<random>
#include"Vector.h"
#include"Color.h"
#include"TextLoader.h"
#include"DirectX12.h"
#include"Audio.h"


#pragma region �萔
#pragma region ����

const int dimention2D = 0;
const int dimention3D = 1;

#pragma endregion

#pragma region �L�[�{�[�h

#pragma endregion
#pragma endregion

#pragma region typedef
using point = int*;
using vertex = int*;
using heap = int*;
using sprite = int*;
using texture = int;
using pipeline = int;
using font = int;
#pragma endregion

enum LibraryPipeline
{
	PIPELINE_NORMAL,
	PIPELINE_GRAY,
	PIPELINE_NOT_DESP_TEST,
	PIPELINE_NOT_BUKK_CULL,
	PIPELINE_NOT_DESP_WRITE,
	PIPELINE_NOT_SHADOW,
	PIPELINE_MATERIAL,
};

class Library
{
private:
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	static int count;
	static int createPointCount;

	static std::unique_ptr<Audio> audio;

	static DirectX12* directx12;
	static WNDCLASSEX w;
	static MSG msg;
	static HWND hwnd;
	static bool isDestroy;
	static bool isEnd;



	static int loadFontTextureCounter;
	static int loadTextureCounter;
	static int createSpriteConter;

	static int createPipelineCounter;//���݂̃p�C�v���C��������
	static int createPostEffectPuiperineCounter;//

	static HDC hdc;//dc�̃n���h��

	static bool isSetFPS60;
	static unsigned int fps;//���ݐݒ肳��Ă���fps
	static int refReat;//���t���b�V�����[�g
	static DWORD startProsessTime;//�v���J�n���̎���
	static DWORD nowTime;//���݂̎���

public:


#pragma region �������Ȃǂ̏���

	/// <summary>
	/// ���C�u�����̏��������s���܂��B�ǂ̃��C�u�����̊֐�������ɌĂяo���Ă�������
	/// </summary>
	/// <param name="windowWidth">�E�B���h�E����</param>
	/// <param name="windowHeight">�E�B���h�E�c��</param>
	static void initialize(int windowWidth, int windowHeight,const Color& screenColor);

	/// <summary>
	/// �`�揀�������܂��B�X�V���������̈�ԏ�ŌĂяo���Ă�������
	/// </summary>
	static void roopStartProcess();

	/// <summary>
	/// �`�悵�܂��B�`�揈���̈�ԍŌ�̌Ăяo���Ă�������
	/// </summary>
	static void roopEndProcess();

	/// <summary>
	/// ���C�u�����̏I���t���O��true�ɂ��܂�
	/// </summary>
	static void endFlagTrue();

	/// <summary>
	/// �I���t���O���擾���܂�
	/// </summary>
	/// <returns>�I���t���O</returns>
	static bool getIsEnd();

	/// <summary>
	/// �I���������s���܂�
	/// </summary>
	static void end();
#pragma endregion

#pragma region WinAPI�֌W
	static HWND getHWND();

#pragma endregion



#pragma region �p�C�v���C��

	/// <summary>
	/// �p�C�v���C����ύX���܂�
	/// </summary>
	/// <param name="num">�p�C�v���C���̔ԍ��������Ă���ϐ�</param>
	static void setPipeline(pipeline num);

	/// <summary>
	/// �V�F�[�_�[�̏��𑗂�A�p�C�v���C�����쐬���܂�(���C�u�����̂��̂𗘗p����ꍇ�A{L"NULL","",""}�Ə����Ă�������)
	/// </summary>
	/// <param name="vShaderData">���_�V�F�[�_�[�̃f�[�^</param>
	/// <param name="gSyaderData">�W�I���g���V�F�[�_�[�̃f�[�^</param>
	/// <param name="pShaderData">�s�N�Z���V�F�[�_�[�̃f�[�^</param>
	/// <param name="useUserInputLayout">�����Őݒ肵��InputLayout��K�����邩</param>
	/// <returns>�p�C�v���C���̔ԍ�</returns>
	static pipeline createUserPipelineState(
		PipelineData pipelineData,
		ShaderData vShaderData,
		ShaderData gSyaderData,
		ShaderData pShaderData,
		bool useUserInputLayout);


	/// <summary>
	/// �V�F�[�_�[�ɑ���f�[�^���Z�b�g���܂�
	/// </summary>
	/// <param name="dataP"></param>
	/// <param name="dataSize"></param>
	//static void setConstMapData(void** dataP, unsigned int dataSize);

	/// <summary>
	/// �s����擾���܂�
	/// </summary>
	/// <param name="matrix"></param>
	/// <param name="heapNum"></param>
	/// <param name="number"></param>
	static void getMatrix(float matrix[4][4], int* heapNum, int number);

	/// <summary>
	/// �r���[�A�v���W�F�N�V�����s�����Z�����s����擾���܂�
	/// </summary>
	/// <param name="matrix"></param>
	static void getCameraMatrix(float matrix[4][4]);


	/// <summary>
	/// �C���v�b�g���C�A�E�g�̐ݒ�����܂�
	/// </summary>
	/// <param name="semantics">�Z�}���e�B�N�X��</param>
	/// <param name="num">�f�[�^��(Vector3�̏ꍇ�A�O�����Ȃ̂�3�����)</param>
	static void setInputLayout(const char* semantics, int num);

	//InputLayout���p�C�v���C���݂����ɔԍ��擾�ł���悤�ɂ��Đ������ɓn���悤�ɂ���?

	/// <summary>
	/// ���݃Z�b�g����Ă���C���v�b�g���C�A�E�g���폜���܂�
	/// </summary>
	static void deleteInputLayout();


#pragma region �p�C�v���C���ݒ�
	//�������

	/// <summary>
	/// �쐬����p�C�v���C���̐[�x�e�X�g���s�����ǂ����̃t���O��ݒ肵�܂�
	/// </summary>
	/// <param name="flag"></param>
	static void setDespTestFlag(bool flag);
#pragma endregion



#pragma region �|�X�g�G�t�F�N�g
	static void setPostEffectPipeline(const pipeline& num);

	static pipeline createUserPostEffectPipelineState(const ShaderData& pShaderData);
#pragma endregion


#pragma endregion


#pragma region �ݒ�

	/// <summary>
	/// FPS��60�ɌŒ肵�܂�
	/// </summary>
	/// <param name="framesPerSecond">�t���O</param>
	static void setFramesPerSecond60(bool flag);

	/// <summary>
	/// ��ʂ��w�肵���F�ŏ��������܂�
	/// </summary>
	/// <param name="color">���������邽�߂̐F</param>
	//static void setScreenColor(Color color);



#pragma endregion

#pragma region ����

#pragma region ���_ �C���f�b�N�X�o�b�t�@����


#pragma region ���f���ǂݍ���


	/// <summary>
	/// obj�`���̃��f���̒��_����ǂݍ��݂܂�
	/// </summary>
	/// <param name="path"></param>
	/// <param name="loadUV">UV��ǂݍ��ނ��ǂ���</param>
	/// <param name="loadNormal">�@���x�N�g����ǂݍ��ނ��ǂ���</param>
	/// <param name="materialFireName">�ǂݎ�����}�e���A���̃t�@�C��(.mtl)��������string�^�̕ϐ�(�ǂݎ��Ȃ��ꍇ�Anullptr)</param>
	/// <param name="p"></param>
	static void loadOBJVertex(const char* path, bool loadUV, bool loadNormal, std::string* materialFireName, vertex* p);
#pragma endregion

#pragma region ���C�u���������I�u�W�F�N�g

	/// <summary>
	/// �_���쐬���܂�
	/// </summary>
	/// <param name="createNum">������</param>
	/// <param name="p"></param>
	static void createPoint(int createNum, point* p);

	/// <summary>
	/// �l�p�`�̒��_�����쐬���܂�
	/// </summary>
	/// <param name="size">�ӂ̒���({���̒���,�c�̒���})</param>
	/// <param name="dimention">����</param>
	/// <param name="p">�f�[�^������ϐ��̃|�C���^</param>
	/// <returns></returns>
	static void createBoard(Vector2 size, int dimention, vertex* p);

	/// <summary>
	/// �~�̒��_�����쐬���܂�
	/// </summary>
	/// <param name="r">���a</param>
	/// <param name="dimention"></param>
	/// <param name="p"></param>
	/// <returns></returns>
	static void createCircle(float r, int dimention, vertex* p);

	/// <summary>
	/// �����̂̒��_�����쐬���܂�
	/// </summary>
	/// <param name="size">�ӂ̒���</param>
	/// <param name="p"></param>
	/// <returns></returns>
	static void create3DBox(Vector3 size, vertex* p);

	/// <summary>
	/// �O�p���̒��_�����쐬���܂�
	/// </summary>
	/// <param name="r"></param>
	/// <param name="vertexNumber"></param>
	/// <param name="centerPosition"></param>
	/// <param name="upVertex"></param>
	/// <param name="p"></param>
	/// <returns></returns>
	static void createTriangularPyramid(float r, int vertexNumber, Vector3 centerPosition, float upVertex, vertex* p);

	/// <summary>
	/// �e�N�X�`�����Y��ɓ\��闧���̂̒��_�����쐬���܂�
	/// </summary>
	/// <param name="size"></param>
	/// <param name="p"></param>
	/// <returns></returns>
	static void createManyVertex3DBox(Vector3 size, vertex* p);

#pragma endregion

#pragma region �����Ŏ���
	//�\����Vertex(���_�\����)��DirectX12Struct����w�b�_��V��������Ĉڂ�


	/// <summary>
	/// ���C�u�����̍\����(Vertex)���g�p���A�����Œ��_����ݒ肵�ăI�u�W�F�N�g�𐶐����܂�
	/// </summary>
	/// <param name="vertex">���_�f�[�^(�@���x�N�g���͌v�Z���Ȃ��Ă悢)</param>
	/// <param name="index">�C���f�b�N�X</param>
	/// <param name="p"></param>
	static void createUserObject(std::vector<Vector3>& vertexPos, std::vector<Vector2>& vertexUV, std::vector<unsigned short>& index, vertex* p);


	/// <summary>
	/// ����̃f�[�^���g�p���A�����Œ��_����ݒ肵�ăI�u�W�F�N�g�𐶐����܂�
	/// </summary>
	/// <param name="vertexData">���_�f�[�^(���̊֐��ɓn������ɂ��̕ϐ��̒l������������ƁA�V�F�[�_�[�ɑ���l���ς��)</param>
	/// <param name="vertexDataSize">���_�f�[�^�P�̂̃f�[�^�T�C�Y</param>
	/// <param name="vertexSumDataSize">���_�f�[�^ * �v�f���̃f�[�^�T�C�Y</param>
	/// <param name="index">�C���f�b�N�X</param>
	/// <param name="p"></param>
	static void createUserObject2(void** vertexData, unsigned int vertexDataSize, unsigned int vertexSumDataSize, std::vector<unsigned short>&index, vertex* p);



#pragma endregion


#pragma endregion

#pragma region �萔 �e�N�X�`���쐬
	/// <summary>
	/// �I�u�W�F�N�g�̃}�e���A����ǂݍ��݁Aheap�����������܂�
	/// </summary>
	/// <param name="materialDirectoryPath">�f�B���N�g���p�X(�t�@�C����Obj��01.obj������ꍇ�A"Obj/")</param>
	/// <param name="materialFileName">obj�̃t�@�C����(�g���q�܂�)</param>
	/// <param name="objectNum">������</param>
	/// <param name="heapP"></param>
	static void loadOBJMaterial(std::string materialDirectoryPath, std::string materialFileName, int objectNum, heap* heapP);

	/// <summary>
	/// �}�e���A����ǂݍ��݁A������̃V�F�[�_�[�ɑ���f�[�^��n���܂�
	/// </summary>
	/// <param name="materialDirectoryPath">�f�B���N�g���p�X(�t�@�C����Obj��01.obj������ꍇ�A"Obj/")</param>
	/// <param name="materialFileName">obj�̃t�@�C����(�g���q�܂�)</param>
	/// <param name="objectNum">������</param>
	/// <param name="dataP">void**�ɕϊ������f�[�^�̃|�C���^</param>
	/// <param name="dataSize">�f�[�^�T�C�Y(sizeof�ŋ��߂�����)</param>
	/// <param name="heapP"></param>
	static void loadObjMaterialUseUserData
	(
		std::string materialDirectoryPath,
		std::string materialFileName,
		int objectNum,
		void** dataP,
		unsigned int dataSize,
		heap* heapP
	);

	/// <summary>
	/// �w�肵���e�N�X�`�����|���S���ɒ���t���܂�
	/// </summary>
	/// <param name="texturePath">�e�N�X�`���̃p�X</param>
	/// <param name="objectNum">�I�u�W�F�N�g������</param>
	/// <param name="setConstDataFlag">���삵���ϐ��A�\���̂��g�����ǂ���</param>
	/// <param name="p"></param>
	static void createHeapData(const wchar_t* texturePath, int objectNum, heap* p);

	/// <summary>
	/// �P�F�Ń|���S������߂܂�
	/// </summary>
	/// <param name="color">�F</param>
	/// <param name="objectNum">�I�u�W�F�N�g������</param>
	/// <param name="setConstDataFlag">���삵���ϐ��A�\���̂��g�����ǂ���</param>
	/// <param name="p"></param>
	static void createHeapData2(Color color, int objectNum, heap* p);


	//static void setConstMapData(void** dataP, unsigned int dataSize);

	/// <summary>
	/// ����̒萔�o�b�t�@�ɑ���f�[�^���쐬���Aheap�����������܂�(�e�N�X�`���\��t����)
	/// </summary>
	/// <param name="texturePath"></param>
	/// <param name="objectNum"></param>
	/// <param name="dataP">�f�[�^�̃|�C���^(void**�ɕϊ�����)</param>
	/// <param name="dataSize">�f�[�^�T�C�Y(sizeof�ŋ��߂ēn��)</param>
	/// <param name="p"></param>
	static void createUserHeapData(const wchar_t* texturePath, int objectNum, void** dataP, unsigned int dataSize, heap* p);

#pragma endregion

#pragma region vertex��heap��������
	//vertex��heap���܂Ƃ߂�model�\���̍��?
	//���f���̃}�e���A���擾����֐����?

	/// <summary>
	/// �������
	/// </summary>
	/// <param name="path"></param>
	/// <param name="materialDirectoryPath"></param>
	/// <param name="loadUV"></param>
	/// <param name="loadNormal"></param>
	/// <param name="objectNum"></param>
	/// <param name="vertP"></param>
	/// <param name="heapP"></param>
	static void loadOBJ(const char* path, std::string materialDirectoryPath, bool loadUV, bool loadNormal, int objectNum, vertex* vertP, heap* heapP);
	//���ꂢ���(���ʂȒ��_�ǂݍ��݂����邩��)

	//static void loadOBJAndCreateUserData
	//(
	//	const wchar_t* path, bool loadUV, bool loadNormal, 
	//	void** vertexData, unsigned int vertexDataSize, unsigned int vertexSumDataSize, std::vector<unsigned short>&index,
	//	void** dataP, unsigned int dataSize,
	//	int objectNum, vertex* vertP, heap* heapP
	//);

#pragma endregion

#pragma endregion




#pragma region �X�v���C�g
	//�e�N�X�`���̔ԍ���Ԃ�
	/// <summary>
	/// �e�N�X�`����ǂݍ��݂܂�
	/// </summary>
	/// <param name="texturePath">�e�N�X�`���̃p�X</param>
	/// <returns></returns>
	static texture loadTexture(const wchar_t* texturePath);

	//static int createSpriteTexture(Color color);

	/// <summary>
	/// �X�v���C�g���쐬���܂�
	/// </summary>
	/// <param name="size">sprite�̃|�C���^</param>
	/// <returns></returns>
	static void createSprite(sprite* sprite);

	/// <summary>
	/// �X�v���C�g�t�H���g��ǂݍ��݂܂�
	/// </summary>
	/// <param name="texturePath"></param>
	/// <param name="lineNum">���A�c�̕�����</param>
	/// <param name="fontSize">�����T�C�Y</param>
	/// <returns></returns>
	static font loadSpriteFont(const wchar_t* texturePath, Vector2 lineNum, Vector2 fontSize);

	static Vector2 getTextureSize(texture textureHandle);
#pragma endregion

#pragma region �`��

	/// <summary>
	/// ���������f�[�^�ŉ摜��`�悵�܂�
	/// </summary>
	/// <param name="createNum">createBox�Ȃǂ�draw + �`��̃��\�b�h�Ő��������f�[�^�̔ԍ�</param>
	/// <param name="dataNum">createData�Ő��������f�[�^�̔ԍ�</param>
	/// <param name="number">���ڂ̂��`�悷�邩(�q�[�v�̉��ڂ�CBV���w�肷�邩)</param>
	/// <returns></returns>
	static void drawGraphic(vertex polygonVertexNumber, heap polygonDataNum, int numbe);

#pragma region �X�v���C�g

#pragma region �`��
	/// <summary>
	/// 2D�̎l�p�`��`�悵�܂�
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="size">�傫��</param>
	/// <param name="color">�F</param>
	/// <param name="spriteHandle">�X�v���C�g�n���h��</param>
	static void drawBox(const Vector2 position, const Vector2& size, const Color& color, sprite spriteHandle);
#pragma endregion


	/// <summary>
	/// �X�v���C�g��`�悵�܂�
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="spriteNumber"></param>
	/// <param name="textureNumber"></param>
	static void drawSprite(Vector2 position, sprite spriteNumber, texture* textureNumber);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="position"></param>
	/// <param name="maxSqare"></param>
	/// <param name="currentNum">1����J�n</param>
	/// <param name="spriteNumber"></param>
	/// <param name="textureNumber"></param>
	static void drawSpriteAnimation(Vector2 position, Vector2 maxSqare, Vector2 currentNum, sprite spriteNumber, texture* textureNumber);

	/// <summary>
	/// �A�j���[�V���������邽�߂Ɋ֐��ł�
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="currentStartNum">����̈ʒu(�摜�̍��W)</param>
	/// <param name="currentEndNum">�E���̈ʒu</param>
	/// <param name="spriteNumber">�X�v���C�g�ԍ�</param>
	/// <param name="textureNumber">�e�N�X�`���ԍ�</param>
	static void drawSpriteAnimation2(Vector2 position, Vector2 currentStartNum, Vector2 currentEndNum, sprite spriteNumber, texture* textureNumber);
#pragma endregion

	/// <summary>
	/// �_���g�p���ăe�N�X�`����`�悵�܂�
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="point"></param>
	/// <param name="texture"></param>
	/// <param name="num">�ԍ�</param>
	static void drawPointAndTexture(Vector3 pos, point point, texture texture, int num);

#pragma endregion

#pragma region �폜
	/// <summary>
	/// �|���S�������폜���܂�
	/// </summary>
	/// <param name="polyNum"></param>
	static void deleteVertexData(vertex polygonVertexNumber);

	/// <summary>
	/// �|���S���f�[�^���폜���܂�
	/// </summary>
	/// <param name="dataNum"></param>
	static void deleteHeapData(heap polygonDataNum);

	/// <summary>
	/// �X�v���C�g���폜���܂�
	/// </summary>
	/// <param name="sprite"></param>
	static void deleteSprite(sprite sprite);
#pragma endregion

#pragma region ������

#pragma region �X���[�X�V�F�[�f�B���O
	static void setSmoothingFlag(bool flag);
#pragma endregion


	/// <summary>
	/// �F����Z���܂�
	/// </summary>
	/// <param name="color"></param>
	/// <param name="polygonDataNum"></param>
	/// <param name="number"></param>
	static void setMulColor(Color color, heap polygonDataNum, int number);

	/// <summary>
	/// �F�����Z���܂�
	/// </summary>
	/// <param name="color"></param>
	/// <param name="polygonDataNum"></param>
	/// <param name="number"></param>
	static void setAddColor(Color color, heap polygonDataNum, int number);

	/// <summary>
	/// �F�����Z���܂�
	/// </summary>
	/// <param name="color"></param>
	/// <param name="polygonDataNum"></param>
	/// <param name="number"></param>
	static void setSubColor(Color color, heap polygonDataNum, int number);


	/// <summary>
	/// �I�u�W�F�N�g�𕽖ʉ����邩��ݒ肵�܂�
	/// </summary>
	/// <param name="flag">���ʉ����邩�ǂ���</param>
	static void setIsPlane(bool flag);

	/// <summary>
	/// �r���{�[�h�����邩��ݒ肵�܂�
	/// </summary>
	/// <param name="x">X���r���{�[�h���邩�ǂ���</param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	static void setIsBillboard(bool x, bool y, bool z);

	static void setSpriteMulColor(Color color, sprite spriteNum);
	static void setSpriteAddColor(Color color, sprite spriteNum);
	static void setSpriteSubColor(Color color, sprite spriteNum);

	static void setPointMulColor(Color color, point pointNum, int num);
#pragma endregion

#pragma region ����
	/// <summary>
	/// �I�u�W�F�N�g�̍��W���Z�b�g�����蓮�������肷��̂Ɏg�p���܂�
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="dataNum"></param>
	/// <param name="number"></param>
	static void setPosition(Vector3 position, heap polygonDataNum, int number);


	/// <summary>
	/// �T�C�Y��ύX���܂�
	/// </summary>
	/// <param name="scale">�T�C�Y((1,1,1)�œ��{)</param>
	/// <param name="dataNum"></param>
	/// <param name="number"></param>
	static void setScale(Vector3 scale, heap polygonDataNum, int number);

	/// <summary>
	/// �I�u�W�F�N�g��Z�AX�AY�̏��ɉ�]�����܂�
	/// </summary>
	/// <param name="angle">�p�x</param>
	/// <param name="dataNum"></param>
	/// <param name="number"></param>
	static void setAngle(Vector3 angle, heap polygonDataNum, int number);

	/// <summary>
	/// �|���S����@���x�N�g���̕����ɉ����o���܂�
	/// </summary>
	/// <param name="ex">�����o���(0�Œʏ�̃��f����\��)</param>
	/// <param name="polygonDataNumber"></param>
	/// <param name="number"></param>
	static void pushPolygon(float ex, heap polygonDataNumber, int number);

	/// <summary>
	/// �������
	/// </summary>
	/// <param name="position"></param>
	/// <param name="sptiteNumber"></param>
	static void setSpritePosition(Vector2 position, sprite sptiteNumber);

	/// <summary>
	/// �X�v���C�g�̃T�C�Y��ς��܂�(�A���J�[�|�C���g�𒲐�����)
	/// </summary>
	/// <param name="scale">�T�C�Y((1,1)�𓙔{�Ƃ���)</param>
	/// <param name="sptiteNumber"></param>
	static void setSpriteScale(Vector2 scale, sprite sptiteNumber);

	/// <summary>
	/// �X�v���C�g�̊p�x��ς��܂�
	/// </summary>
	/// <param name="angle">�p�x(Z�����)</param>
	/// <param name="sptiteNumber"></param>
	static void setSpriteAngle(float angle, sprite sptiteNumber);

	/// <summary>
	/// �_�ō쐬�����l�p�`�̃X�P�[���̐ݒ�(���̃T�C�Y��(1,1,1))
	/// </summary>
	/// <param name="scale"></param>
	/// <param name="pointNum"></param>
	/// <param name="num"></param>
	static void setPointScale(Vector2 scale, point pointNum, int num);


#pragma region �|�X�g�G�t�F�N�g
	/// <summary>
	/// �����_�[�^�[�Q�b�g�̍��W��ύX���܂�
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="rtNum">�ǂ̃����_�[�^�[�Q�b�g���w�肷�邩(���͈Ӗ��Ȃ�)</param>
	static void setRenderTargetPosition(const Vector3& pos, const int& rtNum);

	static void setRenderTargetAngle(const Vector3& angle, const int& rtNum);

	static void setRenderTargetScale(const Vector3& scale, const int& rtNum);

	//true�ɂ���Ə�(Y��)�����̏C������
	/// <summary>
	/// �����_�[�^�[�Q�b�g���J�����̉e�����󂯂�悤�ɂ��邩�ǂ����̃t���O��ݒ肵�܂�
	/// </summary>
	/// <param name="flag"></param>
	/// <param name="rtNum"></param>
	static void setPostEffectCameraFlag(const bool& flag,const int& rtNum);
#pragma endregion

#pragma endregion

#pragma region �A�j���[�V����(����x��y�𓯂��ɂ���)


	/// <summary>
	/// �A�j���[�V���������܂�(�l�p�̃T�C�Y�Œ�)
	/// </summary>
	/// <param name="vertexNum">���_�f�[�^�i���o�[</param>
	/// <param name="maxWidth">���̍��v����</param>
	/// <param name="rectHeight">�c�̍��v����</param>
	/// <param name="animationNumX">���݉����ڂ�</param>
	/// <param name="animationNumY">���݉����ڂ�</param>
	static void changeAnimation(vertex vertexNum, int maxWidth, int maxHeight, int animationNumX, int animationNumY);

	/// <summary>
	/// �A�j���[�V���������܂�(�l�p�̃T�C�Y��)
	/// </summary>
	/// <param name="vertexNum">���_</param>
	/// <param name="dataNum">heap�f�[�^</param>
	/// <param name="startAreaX"></param>
	/// <param name="startAreaY"></param>
	/// <param name="endAreaX"></param>
	/// <param name="endAreaY"></param>
	static void changeAnimation2(vertex vertexNum, heap dataNum, int startAreaX, int startAreaY, int endAreaX, int endAreaY);


#pragma endregion

#pragma region �J����
	/// <summary>
	/// �J�����̏����ʒu���Z�b�g���܂��B�������ɍs��ŃJ��������]�����܂�
	/// </summary>
	/// <param name="position">�J�����̈ʒu���w�肵�܂�</param>
	/// <param name="target">�J�����̒����_���w�肵�܂�</param>
	/// <param name="up">�J�����̏�x�N�g�����w�肵�܂�(���ꂢ��Ȃ�����)</param>
	static void setCameraMatrixPoint(Vector3 position, Vector3 target, Vector3 up);

	/// <summary>
	/// �J�����̏����Z�b�g���܂�
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="target">�����_</param>
	/// <param name="up">��x�N�g��</param>
	static void setCamera(Vector3 position, Vector3 target, Vector3 up);

	/// <summary>
	/// �������
	/// </summary>
	/// <param name="eyeVelocity"></param>
	/// <param name="targetVelocity"></param>
	static void setCameraVelocity(Vector3 eyeVelocity, Vector3 targetVelocity);

	/// <summary>
	/// �J�����̍��W�A�����_�A��x�N�g���̒l����]�s��ŕύX���܂�
	/// </summary>
	/// <param name="eyeAngle">���W</param>
	/// <param name="targetAngle">�����_</param>
	/// <param name="upAngle">�������</param>
	static void setCameraAngle(Vector3 eyeAngle, Vector3 targetAngle, Vector3 upAngle);

	/// <summary>
	/// �J�����̕\���͈͂��w�肵�܂�
	/// </summary>
	/// <param name="nearNum">�\���͈�(��O)</param>
	/// <param name="farNum">�\���͈�(��)</param>
	static void setCameraNearAndFar(float nearNum, float farNum);
#pragma endregion

#pragma region ���C�g

	/// <summary>
	/// ���C�g�̌������Z�b�g���܂�
	/// </summary>
	/// <param name="vector">���C�g�̃x�N�g��(����)</param>
	static void setLightVector(Vector3 vector);

	static void setLightColor(Color lightColor);
#pragma endregion

#pragma region ���_���擾
	//���̓����Ȃ�?

	/// /// <summary>
	/// �I�u�W�F�N�g�̒��_���W���擾���܂�
	/// </summary>
	/// <param name="vertNum"></param>
	/// <returns></returns>
	static std::vector<Vector3> getVertexPosition(int* vertNum);

	/// <summary>
	/// �I�u�W�F�N�g�̒��_���W���㏑�����܂�
	/// </summary>
	/// <param name="vertPos"></param>
	/// <param name="vertNum"></param>
	/// <returns></returns>
	static bool overrideWriteVertexPosition(std::vector<Vector3>vertPos, int* vertNum);



#pragma endregion

#pragma region �s��ɂ��ϊ�
	/// <summary>
	/// �x�N�g�����J�������W��Y���W����Ƃ����A���O���ɍ��킹�āA��]�s��ŉ�]�����܂�
	/// </summary>
	/// <param name="eyeVelocity">��]������x�N�g��</param>
	/// <param name="flag">���K�����邩�ǂ���</param>
	/// <returns></returns>
	static Vector3 rotateVectorToCameraPosition(Vector3 vector, bool flag);

	/// <summary>
///	setCameraAngle�ŉ�]�A�ړ��������J�����̍��W���擾���܂�
/// </summary>
/// <returns></returns>
	static Vector3 getRotateCameraPosition();

	/// <summary>
///	setCameraAngle�ŉ�]�A�ړ��������J�����̒����_���擾���܂�
/// </summary>
/// <returns></returns>
	static Vector3 getRotateCameraTarget();
#pragma endregion

#pragma region �����\��
	/// <summary>
	/// �ǂݍ��񂾃t�H���g���g�p���A������`�悵�܂�
	/// </summary>
	/// <param name="position">�\���ʒu</param>
	/// <param name="size">�����T�C�Y</param>
	/// <param name="text">�\�����镶��</param>
	/// <param name="spriteTextureNum"></param>
	static void drawsSpriteFontString(Vector2 position, Vector2 size, std::string text, font* spriteTextureNum);

#pragma endregion

#pragma region �ǂݍ���
	/// <summary>
	/// csv��ǂݎ��Aint��2�����z��Ɋi�[���܂�
	/// </summary>
	/// <param name="path"></param>
	/// <param name="vector"></param>
	/// <returns></returns>
	static bool loadTextIntVector(const char* path, std::vector<std::vector<int>>& vector);
#pragma endregion

#pragma region �e�q�\��
	/// <summary>
	/// �I�u�W�F�N�g�̐e��ݒ肵�܂�
	/// </summary>
	/// <param name="heapNum"></param>
	/// <param name="number"></param>
	/// <param name="parentObjHeapNum"></param>
	/// <param name="parentNum"></param>
	static void setParent(heap heapNum, int number, heap parentObjHeapNum, int parentNum);

#pragma endregion

#pragma region �v�Z

	/// <summary>
	///  �@���x�N�g�����v�Z���܂�(���쒸�_�f�[�^�̖@���x�N�g�����v�Z����Ƃ��ȂǂɎg�p���܂�)
	/// </summary>
	/// <param name="pos1">���W1</param>
	/// <param name="pos2">2</param>
	/// <param name="pos3">3</param>
	/// <param name="normal1">�@���x�N�g��������ϐ�</param>
	/// <param name="normal2">����</param>
	/// <param name="normal3">����</param>
	static void calculationNormal
	(
		Vector3 pos1, Vector3 pos2, Vector3 pos3,
		Vector3& normal1, Vector3& normal2, Vector3& normal3
	);
#pragma endregion

#pragma region �T�E���h
	/// <summary>
	/// �T�E���h���Đ����܂�(����ǂݍ��݂�����)
	/// </summary>
	/// <param name="path"></param>
	static void playSound(const char* path);

	/// <summary>
	/// �T�E���h��ǂݍ��݂܂�
	/// </summary>
	/// <param name="path">�t�@�C���̃p�X</param>
	/// <param name="name">�T�E���h��(�C�ӂ̖��O)</param>
	static void loadSound(const char* path, std::string name);

	/// <summary>
	/// �ǂݍ��񂾃T�E���h�Đ����܂�
	/// </summary>
	/// <param name="name">�T�E���h��</param>
	static void playLoadSound(std::string name);

	/// <summary>
	/// ���ݍĐ����Ă�ǂݍ��񂾃T�E���h���ꎞ��~���܂�
	/// </summary>
	/// <param name="name"></param>
	/// <param name="resetFlag">�Ȃ����Z�b�g���邩�ǂ���</param>
	static void stopLoadSound(std::string name, bool resetFlag);
#pragma endregion



#pragma region ���̑�

	/// <summary>
	/// �������擾���܂�
	/// </summary>
	/// <param name="number">������(0�܂߂�)</param>
	/// <returns></returns>
	static int getRandomNumber(int number);

	/// <summary>
	/// float�^�̗������擾���܂�
	/// </summary>
	/// <param name="number">������(0�܂߂�)</param>
	/// <returns></returns>
	static float getRandomNumberFloat(int number);
#pragma endregion


};
