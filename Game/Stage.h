#pragma once

//�X�e�[�W�N���X�B�G�̔z�u�Ȃǂ̃X�e�[�W���ƂɈႤ���̂͂����ɏ���
class Stage
{
private:
	Stage();
	~Stage();

	//�X�e�[�W�ԍ�
	int stage;
public:
	Stage(Stage& s) = delete;
	Stage& operator=(Stage& s) = delete;
	Stage* getInstance();

	void initialize(const int& stageNum);
	void update();
	void draw();
};

