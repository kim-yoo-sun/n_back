#define _CRT_SECURE_NO_WARNINGS

#include <bangtal>
#include <cstdlib>
#include <ctime>

using namespace bangtal;

TimerPtr timer, timer_inst;
ScenePtr scene = Scene::create("N_Back Game", "Images/background.png");
ObjectPtr start1 = 0;
ObjectPtr start2 = 0;
ObjectPtr end = 0;
ObjectPtr notice = 0;
ObjectPtr yes_play_song = 0;
ObjectPtr no_play_song = 0;
ObjectPtr placement[12] = { 0, };
SoundPtr song = 0;

int arr_20[20] = { 0, }, result_true[20] = { 0, }, result_your[40] = { 0, };
int trial = 20; int count = 0;
int choose_num = 0; int num_right = 0;

int index_to_x(int index) {
	return (200 + 150 * (index % 4));
}

int index_to_y(int index) {
	return (550 - 170 * (index / 4));
}

void song_object() {
	yes_play_song = Object::create("Images/Play_Song_Yes.png", scene, 800, 700); yes_play_song->setScale(0.5);
	yes_play_song->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
		song->play(true);
		yes_play_song->hide();
		no_play_song->show();
		return true;
		});

	no_play_song = Object::create("Images/Play_Song_No.png", scene, 800, 700); no_play_song->setScale(0.5);
	no_play_song->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
		song->stop();
		no_play_song->hide();
		yes_play_song->show();
		return true;
		});

	yes_play_song->hide();
}

void makearray_start() {
	printf("\n\n\n");
	printf("======================================================================");
	printf("\n\n\n");

	srand((unsigned int)time(NULL));
	for (int i = 0; i < 20; i++) {
		arr_20[i] = rand() % 12;
		if (i > 3)
		{
			if (choose_num == 2)
			{
				if (arr_20[i] % 5 == 0)
					arr_20[i] = arr_20[i - 2];
			}
			if (choose_num == 3)
			{
				if (arr_20[i] % 5 == 0)
					arr_20[i] = arr_20[i - 3];
			}
		}
		printf("%d ", arr_20[i] + 1);
	}
	printf("\n\n");

	trial = 20;
	timer->set(3.0f); timer->start();
}

void makeresult() {
	if (choose_num == 2)
	{
		num_right = 0;
		printf("Correct Answer is :\n");
		for (int i = 2; i < 20; i++)
		{
			if (arr_20[i] == arr_20[i - 2])
				result_true[i - 2] = 1;
			else
				result_true[i - 2] = 2;
			printf(" %d ", result_true[i-2]);
		} // result_true 에 0~17 값이 저장된다.
		printf("\nYour Answer is :\n");
		for (int i = 0; i < 35; i++)
		{
			if (i % 2 == 0)
			{
				printf(" %d ", result_your[i]);
			}
		}
		for (int i = 0; i < 18; i++)
		{
			if (result_true[i] == result_your[2 * i])
			{
				num_right++;
			}
		}
		char message_in[100];
		sprintf(message_in, "Your score is %d out of 18 !!!", num_right);
		showMessage(message_in);
	}
	if (choose_num == 3)
	{
		num_right = 0;
		printf("Correct Answer is :\n");
		for (int i = 3; i < 20; i++)
		{
			if (arr_20[i] == arr_20[i - 3])
				result_true[i - 3] = 1;
			else
				result_true[i - 3] = 2;
			printf(" %d ", result_true[i-3]);
		} // result_true 에 0~16 값이 저장된다.
		printf("\nYour Answer is :\n");
		for (int i = 0; i < 33; i++)
		{
			if (i % 2 == 0)
			{
				printf(" %d ", result_your[i]);
			}
		}
		for (int i = 0; i < 17; i++)
		{
			if (result_true[i] == result_your[2 * i])
			{
				num_right++;
			}
		}
		char message_in[100];
		sprintf(message_in, "Your score is %d out of 17 !!!", num_right);
		showMessage(message_in);
	}
}

void reset() {
	trial = 20; count = 0;
	timer_inst = Timer::create(63.0f);
	timer_inst->setOnTimerCallback([&](auto)->bool {
		makeresult();
		return true;
		});
	for (int i = 0; i < 12; i++)
	{
		placement[i]->show();
	}
}

void init_game()
{
	scene->setOnKeyboardCallback([&](ScenePtr scene, KeyCode key, bool pressed)->bool {
		if (count < 40)
		{
			if (key == KeyCode::KEY_1)
				result_your[count] = 1;
			if (key == KeyCode::KEY_2)
				result_your[count] = 2;
			count++;
		}
		return true;
		});
	timer = Timer::create(3.0f);
	timer->setOnTimerCallback([&](auto)->bool {
		if (trial > 0) {
			timer->set(3.0f);
			timer->start();
			if (trial != 20)
			{
				placement[arr_20[20 - trial - 1]]->show();
			}
			placement[arr_20[20 - trial]]->hide();
		}
		trial--;
		return true;
		});

	start1 = Object::create("Images/2_back_start.png", scene, 20, 170);
	start1->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
		reset(); timer_inst->start(); showTimer(timer_inst);
		notice->hide(); choose_num = 2;
		makearray_start();
		return true;
		});

	start2 = Object::create("Images/3_back_start.png", scene, 20, 100);
	start2->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
		reset(); timer_inst->start(); showTimer(timer_inst);
		notice->hide(); choose_num = 3;
		makearray_start();
		return true;
		});

	end = Object::create("Images/end.png", scene, 750, 100);
	end->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
		endGame();
		return true;
		});

	notice = Object::create("Images/notice.png", scene, 170, 300); notice->setScale(1.1);

	startGame(scene);
}

int main() {
	char path[50];
	for (int i = 0; i < 12; i++) {
		sprintf(path, "Images/p_blue_base%d.png", i + 1);
		placement[i] = Object::create(path, scene, index_to_x(i), index_to_y(i));
		placement[i]->setScale(0.75);
		placement[i]->hide();
	};
	song = Sound::create("Sound/x-mas.mp3"); song->play(true);
	song_object();

	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	init_game();
	
	return 0;
}