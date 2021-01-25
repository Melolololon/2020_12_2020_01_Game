#pragma once

//ステージクラス。敵の配置などのステージごとに違うものはここに書く
class Stage
{
private:
	Stage();
	~Stage();

	//ステージ番号
	int stage;
public:
	Stage(Stage& s) = delete;
	Stage& operator=(Stage& s) = delete;
	Stage* getInstance();

	void initialize(const int& stageNum);
	void update();
	void draw();
};

