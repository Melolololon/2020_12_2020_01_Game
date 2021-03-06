#pragma once
#include<Windows.h>
#include<ctime>
#include<random>
#include"Vector.h"
#include"Color.h"
#include"TextLoader.h"
#include"DirectX12.h"
#include"Audio.h"


#pragma region 定数
#pragma region 次元

const int dimention2D = 0;
const int dimention3D = 1;

#pragma endregion

#pragma region キーボード

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

	static int createPipelineCounter;//現在のパイプライン生成数
	static int createPostEffectPuiperineCounter;//

	static HDC hdc;//dcのハンドル

	static bool isSetFPS60;
	static unsigned int fps;//現在設定されているfps
	static int refReat;//リフレッシュレート
	static DWORD startProsessTime;//計測開始時の時間
	static DWORD nowTime;//現在の時間

public:


#pragma region 初期化などの処理

	/// <summary>
	/// ライブラリの初期化を行います。どのライブラリの関数よりも先に呼び出してください
	/// </summary>
	/// <param name="windowWidth">ウィンドウ横幅</param>
	/// <param name="windowHeight">ウィンドウ縦幅</param>
	static void initialize(int windowWidth, int windowHeight,const Color& screenColor,const wchar_t* windowName);

	/// <summary>
	/// 描画準備をします。更新処理部分の一番上で呼び出してください
	/// </summary>
	static void roopStartProcess();

	/// <summary>
	/// 描画します。描画処理の一番最後の呼び出してください
	/// </summary>
	static void roopEndProcess();

	/// <summary>
	/// ライブラリの終了フラグをtrueにします
	/// </summary>
	static void endFlagTrue();

	/// <summary>
	/// 終了フラグを取得します
	/// </summary>
	/// <returns>終了フラグ</returns>
	static bool getIsEnd();

	/// <summary>
	/// 終了処理を行います
	/// </summary>
	static void end();
#pragma endregion

#pragma region WinAPI関係
	static HWND getHWND();

#pragma endregion



#pragma region パイプライン

	/// <summary>
	/// パイプラインを変更します
	/// </summary>
	/// <param name="num">パイプラインの番号が入っている変数</param>
	static void setPipeline(pipeline num);

	/// <summary>
	/// シェーダーの情報を送り、パイプラインを作成します(ライブラリのものを利用する場合、{L"NULL","",""}と書いてください)
	/// </summary>
	/// <param name="vShaderData">頂点シェーダーのデータ</param>
	/// <param name="gSyaderData">ジオメトリシェーダーのデータ</param>
	/// <param name="pShaderData">ピクセルシェーダーのデータ</param>
	/// <param name="useUserInputLayout">自分で設定したInputLayoutを適応するか</param>
	/// <returns>パイプラインの番号</returns>
	static pipeline createUserPipelineState(
		PipelineData pipelineData,
		ShaderData vShaderData,
		ShaderData gSyaderData,
		ShaderData pShaderData,
		bool useUserInputLayout);


	/// <summary>
	/// シェーダーに送るデータをセットします
	/// </summary>
	/// <param name="dataP"></param>
	/// <param name="dataSize"></param>
	//static void setConstMapData(void** dataP, unsigned int dataSize);

	/// <summary>
	/// 行列を取得します
	/// </summary>
	/// <param name="matrix"></param>
	/// <param name="heapNum"></param>
	/// <param name="number"></param>
	static void getMatrix(float matrix[4][4], int* heapNum, int number);

	/// <summary>
	/// ビュー、プロジェクション行列を乗算した行列を取得します
	/// </summary>
	/// <param name="matrix"></param>
	static void getCameraMatrix(float matrix[4][4]);


	/// <summary>
	/// インプットレイアウトの設定をします
	/// </summary>
	/// <param name="semantics">セマンティクス名</param>
	/// <param name="num">データ数(Vector3の場合、三次元なので3を入力)</param>
	static void setInputLayout(const char* semantics, int num);

	//InputLayoutもパイプラインみたいに番号取得できるようにして生成時に渡すようにする?

	/// <summary>
	/// 現在セットされているインプットレイアウトを削除します
	/// </summary>
	static void deleteInputLayout();


#pragma region パイプライン設定
	//これ消す

	/// <summary>
	/// 作成するパイプラインの深度テストを行うかどうかのフラグを設定します
	/// </summary>
	/// <param name="flag"></param>
	static void setDespTestFlag(bool flag);
#pragma endregion



#pragma region ポストエフェクト
	static void setPostEffectPipeline(const pipeline& num);

	static pipeline createUserPostEffectPipelineState(const ShaderData& pShaderData);
#pragma endregion


#pragma endregion


#pragma region 設定

	/// <summary>
	/// FPSを60に固定します
	/// </summary>
	/// <param name="framesPerSecond">フラグ</param>
	static void setFramesPerSecond60(bool flag);

	/// <summary>
	/// 画面を指定した色で初期化します
	/// </summary>
	/// <param name="color">初期化するための色</param>
	//static void setScreenColor(Color color);



#pragma endregion

#pragma region 生成

#pragma region 頂点 インデックスバッファ生成


#pragma region モデル読み込み


	/// <summary>
	/// obj形式のモデルの頂点情報を読み込みます
	/// </summary>
	/// <param name="path"></param>
	/// <param name="loadUV">UVを読み込むかどうか</param>
	/// <param name="loadNormal">法線ベクトルを読み込むかどうか</param>
	/// <param name="materialFireName">読み取ったマテリアルのファイル(.mtl)名を入れるstring型の変数(読み取らない場合、nullptr)</param>
	/// <param name="p"></param>
	static void loadOBJVertex(const char* path, bool loadUV, bool loadNormal, std::string* materialFireName, vertex* p);
#pragma endregion

#pragma region ライブラリ実装オブジェクト

	/// <summary>
	/// 点を作成します
	/// </summary>
	/// <param name="createNum">生成数</param>
	/// <param name="p"></param>
	static void createPoint(int createNum, point* p);

	/// <summary>
	/// 四角形の頂点情報を作成します
	/// </summary>
	/// <param name="size">辺の長さ({横の長さ,縦の長さ})</param>
	/// <param name="dimention">次元</param>
	/// <param name="p">データを入れる変数のポインタ</param>
	/// <returns></returns>
	static void createBoard(Vector2 size, int dimention, vertex* p);

	/// <summary>
	/// 円の頂点情報を作成します
	/// </summary>
	/// <param name="r">半径</param>
	/// <param name="dimention"></param>
	/// <param name="p"></param>
	/// <returns></returns>
	static void createCircle(float r, int dimention, vertex* p);

	/// <summary>
	/// 立方体の頂点情報を作成します
	/// </summary>
	/// <param name="size">辺の長さ</param>
	/// <param name="p"></param>
	/// <returns></returns>
	static void create3DBox(Vector3 size, vertex* p);

	/// <summary>
	/// 三角錐の頂点情報を作成します
	/// </summary>
	/// <param name="r"></param>
	/// <param name="vertexNumber"></param>
	/// <param name="centerPosition"></param>
	/// <param name="upVertex"></param>
	/// <param name="p"></param>
	/// <returns></returns>
	static void createTriangularPyramid(float r, int vertexNumber, Vector3 centerPosition, float upVertex, vertex* p);

	/// <summary>
	/// テクスチャを綺麗に貼れる立方体の頂点情報を作成します
	/// </summary>
	/// <param name="size"></param>
	/// <param name="p"></param>
	/// <returns></returns>
	static void createManyVertex3DBox(Vector3 size, vertex* p);

#pragma endregion

#pragma region 自分で実装
	//構造体Vertex(頂点構造体)をDirectX12Structからヘッダを新しく作って移す


	/// <summary>
	/// ライブラリの構造体(Vertex)を使用し、自分で頂点情報を設定してオブジェクトを生成します
	/// </summary>
	/// <param name="vertex">頂点データ(法線ベクトルは計算しなくてよい)</param>
	/// <param name="index">インデックス</param>
	/// <param name="p"></param>
	static void createUserObject(std::vector<Vector3>& vertexPos, std::vector<Vector2>& vertexUV, std::vector<unsigned short>& index, vertex* p);


	/// <summary>
	/// 自作のデータを使用し、自分で頂点情報を設定してオブジェクトを生成します
	/// </summary>
	/// <param name="vertexData">頂点データ(この関数に渡した後にこの変数の値を書き換えると、シェーダーに送る値が変わる)</param>
	/// <param name="vertexDataSize">頂点データ単体のデータサイズ</param>
	/// <param name="vertexSumDataSize">頂点データ * 要素数のデータサイズ</param>
	/// <param name="index">インデックス</param>
	/// <param name="p"></param>
	static void createUserObject2(void** vertexData, unsigned int vertexDataSize, unsigned int vertexSumDataSize, std::vector<unsigned short>&index, vertex* p);



#pragma endregion


#pragma endregion

#pragma region 定数 テクスチャ作成
	/// <summary>
	/// オブジェクトのマテリアルを読み込み、heapを初期化します
	/// </summary>
	/// <param name="materialDirectoryPath">ディレクトリパス(ファイル名Objに01.objがある場合、"Obj/")</param>
	/// <param name="materialFileName">objのファイル名(拡張子含む)</param>
	/// <param name="objectNum">生成数</param>
	/// <param name="heapP"></param>
	static void loadOBJMaterial(std::string materialDirectoryPath, std::string materialFileName, int objectNum, heap* heapP);

	/// <summary>
	/// マテリアルを読み込み、かつ自作のシェーダーに送るデータを渡します
	/// </summary>
	/// <param name="materialDirectoryPath">ディレクトリパス(ファイル名Objに01.objがある場合、"Obj/")</param>
	/// <param name="materialFileName">objのファイル名(拡張子含む)</param>
	/// <param name="objectNum">生成数</param>
	/// <param name="dataP">void**に変換したデータのポインタ</param>
	/// <param name="dataSize">データサイズ(sizeofで求めたもの)</param>
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
	/// 指定したテクスチャをポリゴンに張り付けます
	/// </summary>
	/// <param name="texturePath">テクスチャのパス</param>
	/// <param name="objectNum">オブジェクト生成数</param>
	/// <param name="setConstDataFlag">自作した変数、構造体を使うかどうか</param>
	/// <param name="p"></param>
	static void createHeapData(const wchar_t* texturePath, int objectNum, heap* p);

	/// <summary>
	/// 単色でポリゴンを染めます
	/// </summary>
	/// <param name="color">色</param>
	/// <param name="objectNum">オブジェクト生成数</param>
	/// <param name="setConstDataFlag">自作した変数、構造体を使うかどうか</param>
	/// <param name="p"></param>
	static void createHeapData2(Color color, int objectNum, heap* p);


	//static void setConstMapData(void** dataP, unsigned int dataSize);

	/// <summary>
	/// 自作の定数バッファに送るデータを作成し、heapを初期化します(テクスチャ貼り付け版)
	/// </summary>
	/// <param name="texturePath"></param>
	/// <param name="objectNum"></param>
	/// <param name="dataP">データのポインタ(void**に変換する)</param>
	/// <param name="dataSize">データサイズ(sizeofで求めて渡す)</param>
	/// <param name="p"></param>
	static void createUserHeapData(const wchar_t* texturePath, int objectNum, void** dataP, unsigned int dataSize, heap* p);

#pragma endregion

#pragma region vertexとheap同時生成
	//vertexとheapをまとめたmodel構造体作る?
	//モデルのマテリアル取得する関数作る?

	/// <summary>
	/// これ消す
	/// </summary>
	/// <param name="path"></param>
	/// <param name="materialDirectoryPath"></param>
	/// <param name="loadUV"></param>
	/// <param name="loadNormal"></param>
	/// <param name="objectNum"></param>
	/// <param name="vertP"></param>
	/// <param name="heapP"></param>
	static void loadOBJ(const char* path, std::string materialDirectoryPath, bool loadUV, bool loadNormal, int objectNum, vertex* vertP, heap* heapP);
	//これいらん(無駄な頂点読み込みがあるから)

	//static void loadOBJAndCreateUserData
	//(
	//	const wchar_t* path, bool loadUV, bool loadNormal, 
	//	void** vertexData, unsigned int vertexDataSize, unsigned int vertexSumDataSize, std::vector<unsigned short>&index,
	//	void** dataP, unsigned int dataSize,
	//	int objectNum, vertex* vertP, heap* heapP
	//);

#pragma endregion

#pragma endregion




#pragma region スプライト
	//テクスチャの番号を返す
	/// <summary>
	/// テクスチャを読み込みます
	/// </summary>
	/// <param name="texturePath">テクスチャのパス</param>
	/// <returns></returns>
	static texture loadTexture(const wchar_t* texturePath);

	//static int createSpriteTexture(Color color);

	/// <summary>
	/// スプライトを作成します
	/// </summary>
	/// <param name="size">spriteのポインタ</param>
	/// <returns></returns>
	static void createSprite(sprite* sprite);

	/// <summary>
	/// スプライトフォントを読み込みます
	/// </summary>
	/// <param name="texturePath"></param>
	/// <param name="lineNum">横、縦の文字数</param>
	/// <param name="fontSize">文字サイズ</param>
	/// <returns></returns>
	static font loadSpriteFont(const wchar_t* texturePath, Vector2 lineNum, Vector2 fontSize);

	static Vector2 getTextureSize(texture textureHandle);
#pragma endregion

#pragma region 描画

	/// <summary>
	/// 生成したデータで画像を描画します
	/// </summary>
	/// <param name="createNum">createBoxなどのdraw + 形状のメソッドで生成したデータの番号</param>
	/// <param name="dataNum">createDataで生成したデータの番号</param>
	/// <param name="number">何個目のやつを描画するか(ヒープの何個目のCBVを指定するか)</param>
	/// <returns></returns>
	static void drawGraphic(vertex polygonVertexNumber, heap polygonDataNum, int numbe);

#pragma region スプライト

#pragma region 形状
	/// <summary>
	/// 2Dの四角形を描画します
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="size">大きさ</param>
	/// <param name="color">色</param>
	/// <param name="spriteHandle">スプライトハンドル</param>
	static void drawBox(const Vector2 position, const Vector2& size, const Color& color, sprite spriteHandle);
#pragma endregion


	/// <summary>
	/// スプライトを描画します
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="spriteNumber"></param>
	/// <param name="textureNumber"></param>
	static void drawSprite(Vector2 position, sprite spriteNumber, texture* textureNumber);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="position"></param>
	/// <param name="maxSqare"></param>
	/// <param name="currentNum">1から開始</param>
	/// <param name="spriteNumber"></param>
	/// <param name="textureNumber"></param>
	static void drawSpriteAnimation(Vector2 position, Vector2 maxSqare, Vector2 currentNum, sprite spriteNumber, texture* textureNumber);

	/// <summary>
	/// アニメーションさせるために関数です
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="currentStartNum">左上の位置(画像の座標)</param>
	/// <param name="currentEndNum">右下の位置</param>
	/// <param name="spriteNumber">スプライト番号</param>
	/// <param name="textureNumber">テクスチャ番号</param>
	static void drawSpriteAnimation2(Vector2 position, Vector2 currentStartNum, Vector2 currentEndNum, sprite spriteNumber, texture* textureNumber);
#pragma endregion

	/// <summary>
	/// 点を使用してテクスチャを描画します
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="point"></param>
	/// <param name="texture"></param>
	/// <param name="num">番号</param>
	static void drawPointAndTexture(Vector3 pos, point point, texture texture, int num);

#pragma endregion

#pragma region 削除
	/// <summary>
	/// ポリゴン情報を削除します
	/// </summary>
	/// <param name="polyNum"></param>
	static void deleteVertexData(vertex polygonVertexNumber);

	/// <summary>
	/// ポリゴンデータを削除します
	/// </summary>
	/// <param name="dataNum"></param>
	static void deleteHeapData(heap polygonDataNum);

	/// <summary>
	/// スプライトを削除します
	/// </summary>
	/// <param name="sprite"></param>
	static void deleteSprite(sprite sprite);
#pragma endregion

#pragma region 見た目

#pragma region スムースシェーディング
	static void setSmoothingFlag(bool flag);
#pragma endregion


	/// <summary>
	/// 色を乗算します
	/// </summary>
	/// <param name="color"></param>
	/// <param name="polygonDataNum"></param>
	/// <param name="number"></param>
	static void setMulColor(Color color, heap polygonDataNum, int number);

	/// <summary>
	/// 色を加算します
	/// </summary>
	/// <param name="color"></param>
	/// <param name="polygonDataNum"></param>
	/// <param name="number"></param>
	static void setAddColor(Color color, heap polygonDataNum, int number);

	/// <summary>
	/// 色を減算します
	/// </summary>
	/// <param name="color"></param>
	/// <param name="polygonDataNum"></param>
	/// <param name="number"></param>
	static void setSubColor(Color color, heap polygonDataNum, int number);


	/// <summary>
	/// オブジェクトを平面化するかを設定します
	/// </summary>
	/// <param name="flag">平面化するかどうか</param>
	static void setIsPlane(bool flag);

	/// <summary>
	/// ビルボードをするかを設定します
	/// </summary>
	/// <param name="x">X軸ビルボードするかどうか</param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	static void setIsBillboard(bool x, bool y, bool z);

	static void setSpriteMulColor(Color color, sprite spriteNum);
	static void setSpriteAddColor(Color color, sprite spriteNum);
	static void setSpriteSubColor(Color color, sprite spriteNum);

	static void setPointMulColor(Color color, point pointNum, int num);
#pragma endregion

#pragma region 操作
	/// <summary>
	/// オブジェクトの座標をセットしたり動かしたりするのに使用します
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="dataNum"></param>
	/// <param name="number"></param>
	static void setPosition(Vector3 position, heap polygonDataNum, int number);


	/// <summary>
	/// サイズを変更します
	/// </summary>
	/// <param name="scale">サイズ((1,1,1)で等倍)</param>
	/// <param name="dataNum"></param>
	/// <param name="number"></param>
	static void setScale(Vector3 scale, heap polygonDataNum, int number);

	/// <summary>
	/// オブジェクトをZ、X、Yの順に回転させます
	/// </summary>
	/// <param name="angle">角度</param>
	/// <param name="dataNum"></param>
	/// <param name="number"></param>
	static void setAngle(Vector3 angle, heap polygonDataNum, int number);

	/// <summary>
	/// ポリゴンを法線ベクトルの方向に押し出します
	/// </summary>
	/// <param name="ex">押し出し具合(0で通常のモデルを表示)</param>
	/// <param name="polygonDataNumber"></param>
	/// <param name="number"></param>
	static void pushPolygon(float ex, heap polygonDataNumber, int number);

	/// <summary>
	/// これ消す
	/// </summary>
	/// <param name="position"></param>
	/// <param name="sptiteNumber"></param>
	static void setSpritePosition(Vector2 position, sprite sptiteNumber);

	/// <summary>
	/// スプライトのサイズを変えます(アンカーポイントを調整する)
	/// </summary>
	/// <param name="scale">サイズ((1,1)を等倍とする)</param>
	/// <param name="sptiteNumber"></param>
	static void setSpriteScale(Vector2 scale, sprite sptiteNumber);

	/// <summary>
	/// スプライトの角度を変えます
	/// </summary>
	/// <param name="angle">角度(Z軸を基準)</param>
	/// <param name="sptiteNumber"></param>
	static void setSpriteAngle(float angle, sprite sptiteNumber);

	/// <summary>
	/// 点で作成した四角形のスケールの設定(元のサイズは(1,1,1))
	/// </summary>
	/// <param name="scale"></param>
	/// <param name="pointNum"></param>
	/// <param name="num"></param>
	static void setPointScale(Vector2 scale, point pointNum, int num);


#pragma region ポストエフェクト
	/// <summary>
	/// レンダーターゲットの座標を変更します
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rtNum">どのレンダーターゲットを指定するか(今は意味なし)</param>
	static void setRenderTargetPosition(const Vector3& pos, const int& rtNum);

	static void setRenderTargetAngle(const Vector3& angle, const int& rtNum);

	static void setRenderTargetScale(const Vector3& scale, const int& rtNum);

	//trueにすると上(Y軸)向くの修正する
	/// <summary>
	/// レンダーターゲットがカメラの影響を受けるようにするかどうかのフラグを設定します
	/// </summary>
	/// <param name="flag"></param>
	/// <param name="rtNum"></param>
	static void setPostEffectCameraFlag(const bool& flag,const int& rtNum);
#pragma endregion

#pragma endregion

#pragma region アニメーション(ここxとyを同じにする)


	/// <summary>
	/// アニメーションさせます(四角のサイズ固定)
	/// </summary>
	/// <param name="vertexNum">頂点データナンバー</param>
	/// <param name="maxWidth">横の合計枚数</param>
	/// <param name="rectHeight">縦の合計枚数</param>
	/// <param name="animationNumX">現在何枚目か</param>
	/// <param name="animationNumY">現在何枚目か</param>
	static void changeAnimation(vertex vertexNum, int maxWidth, int maxHeight, int animationNumX, int animationNumY);

	/// <summary>
	/// アニメーションさせます(四角のサイズ可変)
	/// </summary>
	/// <param name="vertexNum">頂点</param>
	/// <param name="dataNum">heapデータ</param>
	/// <param name="startAreaX"></param>
	/// <param name="startAreaY"></param>
	/// <param name="endAreaX"></param>
	/// <param name="endAreaY"></param>
	static void changeAnimation2(vertex vertexNum, heap dataNum, int startAreaX, int startAreaY, int endAreaX, int endAreaY);


#pragma endregion

#pragma region カメラ
	/// <summary>
	/// カメラの初期位置をセットします。これを基準に行列でカメラを回転させます
	/// </summary>
	/// <param name="position">カメラの位置を指定します</param>
	/// <param name="target">カメラの注視点を指定します</param>
	/// <param name="up">カメラの上ベクトルを指定します(これいらないかも)</param>
	static void setCameraMatrixPoint(Vector3 position, Vector3 target, Vector3 up);

	/// <summary>
	/// カメラの情報をセットします
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="target">注視点</param>
	/// <param name="up">上ベクトル</param>
	static void setCamera(Vector3 position, Vector3 target, Vector3 up);

	/// <summary>
	/// これ消す
	/// </summary>
	/// <param name="eyeVelocity"></param>
	/// <param name="targetVelocity"></param>
	static void setCameraVelocity(Vector3 eyeVelocity, Vector3 targetVelocity);

	/// <summary>
	/// カメラの座標、注視点、上ベクトルの値を回転行列で変更します
	/// </summary>
	/// <param name="eyeAngle">座標</param>
	/// <param name="targetAngle">注視点</param>
	/// <param name="upAngle">これ消す</param>
	static void setCameraAngle(Vector3 eyeAngle, Vector3 targetAngle, Vector3 upAngle);

	/// <summary>
	/// カメラの表示範囲を指定します
	/// </summary>
	/// <param name="nearNum">表示範囲(手前)</param>
	/// <param name="farNum">表示範囲(奥)</param>
	static void setCameraNearAndFar(float nearNum, float farNum);
#pragma endregion

#pragma region ライト

	/// <summary>
	/// ライトの向きをセットします
	/// </summary>
	/// <param name="vector">ライトのベクトル(向き)</param>
	static void setLightVector(Vector3 vector);

	static void setLightColor(Color lightColor);
#pragma endregion

#pragma region 頂点情報取得
	//この二ついらない?

	/// /// <summary>
	/// オブジェクトの頂点座標を取得します
	/// </summary>
	/// <param name="vertNum"></param>
	/// <returns></returns>
	static std::vector<std::vector<Vector3>> getVertexPosition(int* vertNum);

	/// <summary>
	/// オブジェクトの頂点座標を上書きします
	/// </summary>
	/// <param name="vertPos"></param>
	/// <param name="vertNum"></param>
	/// <returns></returns>
	static bool overrideWriteVertexPosition(std::vector<std::vector<Vector3>>vertPos, int* vertNum);



#pragma endregion

#pragma region 行列による変換
	/// <summary>
	/// ベクトルをカメラ座標のY座標を基準としたアングルに合わせて、回転行列で回転させます
	/// </summary>
	/// <param name="eyeVelocity">回転させるベクトル</param>
	/// <param name="flag">正規化するかどうか</param>
	/// <returns></returns>
	static Vector3 rotateVectorToCameraPosition(Vector3 vector, bool flag);

	/// <summary>
///	setCameraAngleで回転、移動させたカメラの座標を取得します
/// </summary>
/// <returns></returns>
	static Vector3 getRotateCameraPosition();

	/// <summary>
///	setCameraAngleで回転、移動させたカメラの注視点を取得します
/// </summary>
/// <returns></returns>
	static Vector3 getRotateCameraTarget();
#pragma endregion

#pragma region 文字表示
	/// <summary>
	/// 読み込んだフォントを使用し、文字を描画します
	/// </summary>
	/// <param name="position">表示位置</param>
	/// <param name="size">文字サイズ</param>
	/// <param name="text">表示する文字</param>
	/// <param name="spriteTextureNum"></param>
	static void drawsSpriteFontString(Vector2 position, Vector2 size, std::string text, font* spriteTextureNum);

#pragma endregion

#pragma region 読み込み
	/// <summary>
	/// csvを読み取り、intの2次元配列に格納します
	/// </summary>
	/// <param name="path"></param>
	/// <param name="vector"></param>
	/// <returns></returns>
	static bool loadTextIntVector(const char* path, std::vector<std::vector<int>>& vector);
#pragma endregion

#pragma region 親子構造
	/// <summary>
	/// オブジェクトの親を設定します
	/// </summary>
	/// <param name="heapNum"></param>
	/// <param name="number"></param>
	/// <param name="parentObjHeapNum"></param>
	/// <param name="parentNum"></param>
	static void setParent(heap heapNum, int number, heap parentObjHeapNum, int parentNum);

#pragma endregion

#pragma region 計算

	/// <summary>
	///  法線ベクトルを計算します(自作頂点データの法線ベクトルを計算するときなどに使用します)
	/// </summary>
	/// <param name="pos1">座標1</param>
	/// <param name="pos2">2</param>
	/// <param name="pos3">3</param>
	/// <param name="normal1">法線ベクトルを入れる変数</param>
	/// <param name="normal2">同じ</param>
	/// <param name="normal3">同じ</param>
	static void calculationNormal
	(
		Vector3 pos1, Vector3 pos2, Vector3 pos3,
		Vector3& normal1, Vector3& normal2, Vector3& normal3
	);
#pragma endregion

#pragma region サウンド
	/// <summary>
	/// サウンドを再生します(毎回読み込みが発生)
	/// </summary>
	/// <param name="path"></param>
	static void playSound(const char* path);

	/// <summary>
	/// サウンドを読み込みます
	/// </summary>
	/// <param name="path">ファイルのパス</param>
	/// <param name="name">サウンド名(任意の名前)</param>
	static void loadSound(const char* path, std::string name,bool loop);

	/// <summary>
	/// 読み込んだサウンド再生します
	/// </summary>
	/// <param name="name">サウンド名</param>
	static void playLoadSound(std::string name);

	/// <summary>
	/// 現在再生してる読み込んだサウンドを一時停止します
	/// </summary>
	/// <param name="name"></param>
	/// <param name="resetFlag">曲をリセットするかどうか</param>
	static void stopLoadSound(std::string name, bool resetFlag);
#pragma endregion



#pragma region その他

	/// <summary>
	/// 乱数を取得します
	/// </summary>
	/// <param name="number">乱数幅(0含める)</param>
	/// <returns></returns>
	static int getRandomNumber(int number);

	/// <summary>
	/// float型の乱数を取得します
	/// </summary>
	/// <param name="number">乱数幅(0含める)</param>
	/// <returns></returns>
	static float getRandomNumberFloat(int number);
#pragma endregion


};

