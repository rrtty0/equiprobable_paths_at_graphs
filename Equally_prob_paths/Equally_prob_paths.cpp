#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <vector>
#include <map>

using namespace std;


class Probability
{
private:
	int numerator;
	int denominator;

public:
	Probability()
	{
	}

	void set(int numerator, int denominator)
	{
		reduction(numerator, denominator);
		this->numerator = numerator;
		this->denominator = denominator;
	}

	int get_numerator()
	{
		return numerator;
	}

	int get_denominator()
	{
		return denominator;
	}

	void reduction(int& a, int& b)
	{
		int r,
			q,
			a_x = 0,
			a_y = 1,
			r_x,
			r_y,
			b_x = 1,
			b_y = 0;
		if (a != 0 && b != 0)
		{
			while (a != 0 && b != 0)
			{
				q = a / b;
				r = a - q * b;
				r_x = a_x + q * b_x;
				r_y = a_y + q * b_y;
				a = b;
				a_x = b_x;
				a_y = b_y;
				b = r;
				b_x = r_x;
				b_y = r_y;
			}
			a = b_x;
			b = b_y;
		}
	}

	~Probability()
	{
	}
};

class Line
{
private:
	int first_node;
	int second_node;
	Probability prob_of_line;

public:

	Line()
	{
	}

	void set(int first_node, int second_node)
	{
		this->first_node = first_node;
		this->second_node = second_node;
	}

	void set_probability(int value1, int value2)
	{
		prob_of_line.set(value1, value2);
	}

	int get_first_node()
	{
		return first_node;
	}

	int get_second_node()
	{
		return second_node;
	}

	int get_probability_numerator()
	{
		return prob_of_line.get_numerator();
	}

	int get_probability_denominator()
	{
		return prob_of_line.get_denominator();
	}

	const bool operator<(const Line& line) const
	{
		if ((this->first_node < line.first_node) || (this->first_node <= line.first_node && this->second_node < line.second_node))
			return true;
		else
			return false;
	}

	bool operator==(int number)
	{
		return (prob_of_line.get_numerator() == number && prob_of_line.get_denominator() == number);
	}

	~Line()
	{
	}
};


bool read_data_from_file();
int get_adjacency_matrix(vector<char>);
void get_err(int);
void ways_from_node_to_node(int, vector<int> = {});
bool check_cycles(vector<int>, int);
void get_result();
map<Line, int> get_number_visit_lines();
map<int, int> get_number_visit_nodes(map<Line, int>);
void get_output_matrix(map<Line, int>, map<int, int>);
void write_result_to_file(bool);

int matrix_size,                              //размер матрицы смежности
start_node,                                   //номер начальной вершины
finish_node;                                  //номер конечной вершины
vector <vector <int>> adjacency_matrix;       //матрица смежности
vector <vector <int>> all_ways_matrix;        //все пути между вершинами
vector <vector <vector <int>>> output_matrix; //матрица результат
bool cycles_in_matrix = false;


int main()
{
	bool true_input = true;

	setlocale(LC_ALL, "rus");
	printf("\t\t\t%s\n\n", "РАВНОВЕРОЯТНЫЕ ПУТИ В ГРАФЕ");

	if ((true_input = read_data_from_file()) == true)
	{
		ways_from_node_to_node(start_node - 1);
		if (!cycles_in_matrix)
			get_result();
		else
		{
			get_err(10);
			true_input = false;
		}
	}
	write_result_to_file(true_input);
	adjacency_matrix.clear();
	all_ways_matrix.clear();
	output_matrix.clear();
	system("pause");
	return 0;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Функция чтения исходных данных из файла input.txt
bool read_data_from_file()
{
	FILE* ptr_to_file;
	bool correct_input = true;
	char symbol;
	vector <char> current_string;
	int i,
		flag;

	fopen_s(&ptr_to_file, "equally_prob_paths/input.txt", "r");

	if (ptr_to_file)
	{
		if ((symbol = fgetc(ptr_to_file)) != EOF)
		{
			rewind(ptr_to_file);
			if (fscanf_s(ptr_to_file, "%d", &start_node) != 1)
			{
				get_err(0);
				correct_input = false;
			}

			if (correct_input && fscanf_s(ptr_to_file, "%d", &finish_node) != 1)
			{
				get_err(1);
				correct_input = false;
			}

			while (correct_input && (symbol = fgetc(ptr_to_file)) != '0' && symbol != '1')
			{
				if (symbol == EOF)
				{
					get_err(2);
					correct_input = false;
				}
			}

			if (correct_input && symbol != EOF)
				fseek(ptr_to_file, -(long)(sizeof(char)), 1);

			while (correct_input && (symbol = fgetc(ptr_to_file)) != EOF)
			{
				i = 0;
				fseek(ptr_to_file, -(long)(sizeof(char)), 1);

				while (correct_input && (symbol = fgetc(ptr_to_file)) != '\n' && symbol != EOF)
				{
					current_string.resize(current_string.size() + 1);
					current_string[i++] = symbol;
				}

				if (!(i == 0 && symbol != EOF))
				{
					flag = get_adjacency_matrix(current_string);

					if (flag == -1)
					{
						get_err(3);
						correct_input = false;
					}

					if (flag == 0)
					{
						get_err(4);
						correct_input = false;
					}
				}
				current_string.clear();
			}

			if (correct_input)
			{
				if (adjacency_matrix.size() != adjacency_matrix[0].size())
				{
					get_err(5);
					correct_input = false;
				}
				else
				{
					matrix_size = adjacency_matrix.size();
					if (matrix_size < start_node)
					{
						get_err(6);
						correct_input = false;
					}
					if (matrix_size < finish_node)
					{
						get_err(7);
						correct_input = false;
					}
				}
			}
		}
		else
		{
			get_err(8);
			correct_input = false;
		}
	}
	else
	{
		get_err(9);
		correct_input = false;
	}
	_fcloseall();

	return correct_input;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Функция формирования матрицы смежности, считанной из input.txt
int get_adjacency_matrix(vector<char> str)
{
	vector<char>::iterator p1 = str.begin();
	vector<vector<int>>::iterator p2;

	adjacency_matrix.resize(adjacency_matrix.size() + 1);

	while (p1 != str.end())
	{
		while (p1 != str.end() && *p1 == ' ')
			p1++;
		if (p1 != str.end())
		{
			if (*p1 == '1' || *p1 == '0')
			{
				adjacency_matrix[adjacency_matrix.size() - 1].resize(adjacency_matrix[adjacency_matrix.size() - 1].size() + 1);
				adjacency_matrix[adjacency_matrix.size() - 1][adjacency_matrix[adjacency_matrix.size() - 1].size() - 1] = *p1 - 48;
				p1++;
			}
			else
			{
				if (*p1 != 9)
				{
					adjacency_matrix.clear();
					return -1;
				}
				else
					p1++;
			}
		}
	}

	if (adjacency_matrix[adjacency_matrix.size() - 1].size() == 0)
		adjacency_matrix.resize(adjacency_matrix.size() - 1);
	else
		for (p2 = adjacency_matrix.begin(); p2 != adjacency_matrix.end() - 1; p2++)
			if (p2->size() != adjacency_matrix[adjacency_matrix.size() - 1].size())
			{
				adjacency_matrix.clear();
				return 0;
			}

	return 1;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Функция вывода ошибки
void get_err(int err_code)
{
	cout << "-Результат не был получен!" << endl << endl;
	cout << "-Причина:" << endl;
	switch (err_code)
	{
	case 0:
		cout << "Номер начальной вершины не введен или введен не корректно!\n\n";
		break;
	case 1:
		cout << "Номер конечной вершины не введен или введен не корректно!\nДля уверенности проверьте корректность ввода и начальной вершины!\n\n";
		break;
	case 2:
		cout << "Непредвиденный конец файла input.txt! Вы не ввели матрицу смежности!\n\n";
		break;
	case 3:
		cout << "В матрице, как и во всем файле input.txt, не должно быть посторонних символов! Допустимые символы - 1 и 0!\n\n";
		break;
	case 4:
		cout << "Строки матрицы должны быть одинаковыми по длине!\n\n";
		break;
	case 5:
		cout << "Матрица должна быть квадратной (Количество строк матрицы смежности должно равняться кол-ву ее столбцов)!\n\n";
		break;
	case 6:
		cout << "Номер начальной вершины слишком большой!\n\n";
		break;
	case 7:
		cout << "Номер конечной вершины слишком большой!\n\n";
		break;
	case 8:
		cout << "Файл input.txt пуст!\n\n";
		break;
	case 9:
		cout << "! Файл input.txt не найден!\n\n! Проверьте наличия файла input.txt в папке проекта, где находится данный .cpp файл программы!\n\n";
		break;
	case 10:
		cout << "Граф не должен содержать циклов!\n\n";
		break;
	}
	cout << "! В случае возникновения вопросов по вводу данных в файл input.txt - обращайтесь к автору программы!\n\n";
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Функция поиска всех путей между двумя заданными вершинами
void ways_from_node_to_node(int current_node, vector<int> pref)
{
	int j;

	for (j = 0; j < matrix_size && !cycles_in_matrix; j++)
	{
		if (adjacency_matrix[current_node][j] == 1)
		{
			cycles_in_matrix = check_cycles(pref, current_node + 1);
			if (!cycles_in_matrix)
			{
				if (pref.empty())
					pref.push_back(current_node + 1);
				else
					pref.push_back(current_node + 1);

				if (j == finish_node - 1)
				{
					all_ways_matrix.resize(all_ways_matrix.size() + 1);
					all_ways_matrix[all_ways_matrix.size() - 1].insert(all_ways_matrix[all_ways_matrix.size() - 1].begin(), pref.begin(), pref.end());
					all_ways_matrix[all_ways_matrix.size() - 1].push_back(j + 1);
				}
				else
					ways_from_node_to_node(j, pref);
				pref.resize(pref.size() - 1);
			}
		}
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Функция проверки наличия циклов(!мешающих поиску путей между
//двумя заданными вершинами!) в графе
bool check_cycles(vector<int> pref, int current_node)
{
	vector<int>::iterator it;

	for (it = pref.begin(); it != pref.end(); it++)
		if (*it == current_node)
			return true;

	return false;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Функция формирования результата
void get_result()
{
	map <Line, int> number_visit_lines;
	map <int, int> number_visit_nodes;

	number_visit_lines = get_number_visit_lines();
	number_visit_nodes = get_number_visit_nodes(number_visit_lines);
	get_output_matrix(number_visit_lines, number_visit_nodes);

	cout << "-Результат успешно получен!\n\n";
	cout << "-Чтобы его увидеть откройте файл output.txt в папке проекта, где лежит данный\n.cpp файл программы!\n\n";
	number_visit_lines.clear();
	number_visit_nodes.clear();
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Функция получения количества посещений каждого ребра
//при прохождении по всем путям между двумя заданными
//вершинами ровно один раз
map<Line, int> get_number_visit_lines()
{
	map <Line, int> number_visit_lines;
	map<Line, int>::iterator itmap;
	Line current_line;

	for (int i = 0; i < all_ways_matrix.size(); i++)
		for (int j = 0; j < all_ways_matrix[i].size() - 1; j++)
		{
			current_line.set(all_ways_matrix[i][j], all_ways_matrix[i][j + 1]);
			if ((itmap = number_visit_lines.find(current_line)) == number_visit_lines.end())
				number_visit_lines.insert(pair<Line, int>(current_line, 1));
			else
				number_visit_lines.at(current_line) += 1;
		}
	return number_visit_lines;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//функция получения кол-ва посещений каждой вершины, при проходе по
//всем путям между двумя заданными вершинами ровно один раз
map<int, int> get_number_visit_nodes(map<Line, int> number_visit_lines)
{
	map <int, int> number_visit_nodes;
	map<Line, int>::iterator itmap1;
	map<int, int>::iterator itmap2;
	Line help_exemplar;

	for (itmap1 = number_visit_lines.begin(); itmap1 != number_visit_lines.end(); itmap1++)
	{
		help_exemplar = itmap1->first;
		if ((itmap2 = number_visit_nodes.find(help_exemplar.get_first_node())) == number_visit_nodes.end())
			number_visit_nodes.insert(pair<int, int>(help_exemplar.get_first_node(), itmap1->second));
		else
			number_visit_nodes.at(help_exemplar.get_first_node()) += itmap1->second;
	}
	return number_visit_nodes;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Функция формирования матрицы, содержащей искомые вероятности
void get_output_matrix(map<Line, int> number_visit_lines, map<int, int> number_visit_nodes)
{
	map<Line, int>::iterator itmap;
	Line help_exemplar;

	output_matrix.resize(matrix_size);
	for (int i = 0; i < matrix_size; i++)
		output_matrix[i].resize(matrix_size);

	for (int i = 0; i < matrix_size; i++)
		for (int j = 0; j < matrix_size; j++)
			output_matrix[i][j].resize(1);

	for (itmap = number_visit_lines.begin(); itmap != number_visit_lines.end(); itmap++)
	{
		help_exemplar = itmap->first;
		help_exemplar.set_probability(itmap->second, number_visit_nodes[help_exemplar.get_first_node()]);
		if (help_exemplar == 1)
			output_matrix[help_exemplar.get_first_node() - 1][help_exemplar.get_second_node() - 1][0] = help_exemplar.get_probability_numerator();
		else
		{
			output_matrix[help_exemplar.get_first_node() - 1][help_exemplar.get_second_node() - 1].resize(2);
			output_matrix[help_exemplar.get_first_node() - 1][help_exemplar.get_second_node() - 1][0] = help_exemplar.get_probability_numerator();
			output_matrix[help_exemplar.get_first_node() - 1][help_exemplar.get_second_node() - 1][1] = help_exemplar.get_probability_denominator();
		}
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Функция записи результата в файл
void write_result_to_file(bool correct_input)
{
	FILE* ptr_to_file;
	errno_t err;

	err = fopen_s(&ptr_to_file, "equally_prob_paths/output.txt", "w");
	if (err == 0)
	{
		if (correct_input)
		{
			fprintf(ptr_to_file, "%s:\n\n", "ИСХОДНЫЕ ДАННЫЕ");
			fprintf(ptr_to_file, "%s: %d\n", "Начальная вершина", start_node);
			fprintf(ptr_to_file, "%s: %d\n", "Конечная вершина", finish_node);
			fprintf(ptr_to_file, "%s:\n", "Исходная матрица смежности");

			for (int i = 0; i < matrix_size; i++)
				for (int j = 0; j < matrix_size; j++)
					if (j == 0)
						fprintf(ptr_to_file, "\n%d", adjacency_matrix[i][j]);
					else
						fprintf(ptr_to_file, "\t%d", adjacency_matrix[i][j]);

			fprintf(ptr_to_file, "\n\n\n\n%s:\n", "МАТРИЦА РЕЗУЛЬТАТ");

			for (int i = 0; i < matrix_size; i++)
			{
				for (int j = 0; j < matrix_size; j++)
				{
					for (int k = 0; k < output_matrix[i][j].size(); k++)
					{
						if (k == 1)
							fprintf(ptr_to_file, "/%d", output_matrix[i][j][k]);
						else
							if (j == 0)
								fprintf(ptr_to_file, "\n%d", output_matrix[i][j][k]);
							else
								fprintf(ptr_to_file, "\t%d", output_matrix[i][j][k]);
					}
				}
			}
		}
		else
			fprintf(ptr_to_file, "%s", "Введены некорректные исходные данные!\nЗапустите программу еще раз и посмотрите причину ошибки - она будет указана в консоли");
	}
	else
	{
		cout << "Файл output.txt не был открыт для записи!\n\n";
		cout << "В случае возникновения вопросов по записи в файл output.txt - обращайтесь к автору программы!\n\n";
	}

	_fcloseall();
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++