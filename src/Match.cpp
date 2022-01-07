#include "Match.h"


void Match::simulate(int seed) {

	if (was_played)
		return;
	was_played = true;
	// set score
	//score[0] = 100 - seed;
	//score[1] = 101 - seed;
	srand(seed);
		
	auto mean = [](Team t, std::function<stat_type(Player& p)> g) {
		double sum = 0;
		for (auto& p : t.get_players()) {

			sum += g(p);

		};
		return sum / t.get_players().size();
	};

	score[0] = 
		(mean(this->team[0], [](Player& p) { return p.get_stat(ATK); }) + rand() % 11 - 5) * 
		(mean(this->team[0], [](Player& p) { return p.get_stat(END); }) + rand() % 11 - 5) / 100 -
		(mean(this->team[1], [](Player& p) { return p.get_stat(DEF); }) + rand() % 11 - 5) *
		(mean(this->team[1], [](Player& p) { return p.get_stat(END); }) + rand() % 11 - 5) / 200;
	score[0] = score[0] >= 0 ? score[0] : 0;

	score[1] = 
		(mean(this->team[1], [](Player& p) { return p.get_stat(ATK); }) + rand() % 11 - 5) *
		(mean(this->team[1], [](Player& p) { return p.get_stat(END); }) + rand() % 11 - 5) / 100 -
		(mean(this->team[0], [](Player& p) { return p.get_stat(DEF); }) + rand() % 11 - 5) *
		(mean(this->team[0], [](Player& p) { return p.get_stat(END); }) + rand() % 11 - 5) / 200;
	score[1] = score[1] >= 0 ? score[1] : 0;

}


Schedule generate_schedule(std::vector<Team> teams) {

	int times_to_play = 2;
	int matches_for_each_team = (teams.size() - 1) * times_to_play;

	int days = matches_for_each_team * 2;
	
	Schedule s(days);
	int k = 0;
	for (int i = 0; i < teams.size(); ++i) {

		for (int j = i + 1; j < teams.size(); ++j) {

			s[k % days].push_back(Match(&teams[i], &teams[j]));
			k += 2;

		}

	}

	return s;

}


std::ostream& operator<<(std::ostream& os, Schedule sch) {

	sch[0][0].simulate();
	//�������� �� days_in_row ������� ����
	//���� �� size div days_in_row
	os << std::setw(days_in_row * full_len + (days_in_row + 1) * 2) << std::setfill('#') << "" << std::endl;
	for (int i = 0; i < (sch.size() / days_in_row); ++i) {

		print_one_row(os, sch, i, days_in_row);
		os << std::setw(days_in_row * full_len + (days_in_row + 1) * 2) << std::setfill('#') << "" << std::endl;

	}

	print_one_row(os, sch, sch.size() / days_in_row, sch.size() % days_in_row);
	os << std::setw(days_in_row * full_len + (days_in_row + 1) * 2) << std::setfill('#') << "" << std::endl;
	
	return os;

}


void print_one_row(std::ostream& os, Schedule sch, int i, int days) {

	
	//����� ���� 
	os << "//";
	for (int j = 0; j < days; ++j) {

		std::string day = day_name + std::to_string(i * days + j + 1);
		int left = ceil((full_len - day.size()) / 2.0);
		int right = floor((full_len - day.size()) / 2.0);
		os << std::setw(left) << std::setfill('_') << "" << day << std::setw(right) << std::setfill('_') << "";
		os << "//";

	}
	os << std::endl;
	/////////

	//������������ ���������� ������ �� days_in_row ����
	std::vector<unsigned long long> v;
	for (int j = 0; j < days; ++j) {

		v.push_back(sch[days * i + j].size());

	}
	int max = v[std::distance(v.begin(), std::max_element(v.begin(), v.end()))];

	//������� ����������� ��� �� days_in_row ����
	for (int j = 0; j < max; ++j) {


		//����� �������� ������
		os << "//";
		for (int k = 0; k < days; ++k) {

			if (j < sch[i * days + k].size()) {
				
				std::string teams_name = sch[i * days + k][j].get_team(0)->get_short_name() + delimeter + sch[i * days + k][j].get_team(1)->get_short_name();
				int left = ceil((full_len - teams_name.size()) / 2.0);
				int right = floor((full_len - teams_name.size()) / 2.0);
				os << std::setw(left) << std::setfill('_') << "" << teams_name << std::setw(right) << std::setfill('_') << "";

			}
			else {

				os << std::setw(full_len) << std::setfill('_') << "";


			}
			os << "//";

		}

		os << std::endl;

		//����� �����
		os << "//";
		for (int k = 0; k < days; ++k) {

			if ((j < sch[i * days + k].size()) and (sch[i * days + k][j].played())) {

				std::string teams_score = 
					std::to_string(sch[i * days + k][j].get_score(0)) + 
					delimeter + 
					std::to_string(sch[i * days + k][j].get_score(1));

				int left = ceil((full_len - teams_score.size()) / 2.0);
				int right = floor((full_len - teams_score.size()) / 2.0);
				os << std::setw(left) << std::setfill('_') << "" << teams_score << std::setw(right) << std::setfill('_') << "";

			}
			else {

				os << std::setw(full_len) << std::setfill('_') << "";


			}
			os << "//";


		}
		os << std::endl;

	}
}