#ifndef ARIFMETIC_EXPRESSION_H
#define ARIFMETIC_EXPRESSION_H

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <math.h>
#include <tuple>

/*
TASK:
	1) ��������� + - * / ( ) - DONE
	2) ��������� �������� � ���� - DONE
	3) ��������� ������� - DONE
	4) ��������� �������� ������
	5) NEW TASK ������� �������� ������� �������� � ��������

		1) ����: ������� ��� ����� ��� ���������� �������� - ������� ����� (��� ������ �������)
		2) ��� ���������� � �������� ������ ���������� �������� � ����������� �� ���� �������� (�������� ��� �������)
		3) ������������ �������� ������, ���������� �������� sin(10+sin(10))==10 10 sin + sin


*/

class Lecsems {
private:

	friend class Arithmetic�alculation;

	static bool checkDoubleValue(const std::string& value) { // ����� �������� ����� �� �����������
		check_value cv = check_value::waiting_number;
		for (size_t count = 0; count <= value.size(); ++count) { // �������� ������ ������ ������ = ��������� �����
			switch (cv)
			{
			case Lecsems::check_value::error: // ������
				return false;
			case Lecsems::check_value::waiting_number: // ���� ������ �����
				if (value[count] == '.') cv = Lecsems::check_value::error; // ���� �����, ������� �� ������
				else cv = Lecsems::check_value::waiting_number_or_point; // ��������� ������ ������� �� ������� ����� ��� �����
				break;
			case Lecsems::check_value::waiting_number_or_point: // ���� ����� ��� �����
				if (value[count] == '.') cv = Lecsems::check_value::waiting_number_after_point; // ���� �����, �� ������� �� �������� ����� ����� �����
				else cv = Lecsems::check_value::waiting_number_or_point; // ��������� ������ - ������� �� ��� �� ��������� (������� ����� ��� �����)
				break;
			case Lecsems::check_value::waiting_number_after_point: // �������� ����� ����� �����
				if (value[count] == '.') cv = Lecsems::check_value::error; // ���� �����, �� ������
				else cv = Lecsems::check_value::waiting_number_after_point; // ��������� ������ - ������� �� ��� �� ��������� (�������� ����� ����� �����)
				break;
			default:
				break;
			}
		}
		return true; // ������� ���� ������ � ��� ������, ���� ������ = ����� - ����������
	}


	enum class check_value {
		error,
		waiting_number,
		waiting_number_or_point,
		waiting_number_after_point
	};

	enum class state {
		start,
		error,
		waiting_operator,
		waiting_open_brackets,
		waiting_operator_or_brackets,
		wait_numb_or_brackets,
		wait_unarn_op_or_operand_or_brackets,
		wait_operand,
		waiting_close_brackets
	};

	enum class Type_of_operation {
		non_operation,
		unary_operation,
		binary_operation
	};
	enum class Priority {
		numbers,
		letters,
		constants,
		functions,
		operations_add_or_sub,
		operations_mult_or_div,
		own_operations, // ���� ����������
		breackets
	};

	std::vector<std::tuple<std::string, Type_of_operation, Priority>> m_data;

	/////////////////
	/// CONSTANTS ///
	/////////////////
	std::vector<std::pair<std::string, double>> m_list_of_constants = { {"G", 9.81}, {"e", 2.71}, {"Pi", 3.14} }; // ������ ��� �� ����� � �������� ���������
	size_t size_of_list_of_consts() { // ���-�� �������� � ������� m_list_of_constants
		return m_list_of_constants.size();
	}
	void print_list_of_consts() const { // ����� ���� �������� ������ (���, ��������)
		std::cout << "LIST OF CONSTNTS: " << std::endl;
		for (size_t count = 0; count < m_list_of_constants.size(); ++count) {
			std::cout << m_list_of_constants[count].first << " = " << m_list_of_constants[count].second << std::endl;
		}
	}
	double get_value_of_const(const size_t& index) { // ���������� �������� ��������� 
		return m_list_of_constants[index].second;
	}
	std::string get_name_of_const(const size_t& index) { // ���������� ��� ���������
		return m_list_of_constants[index].first;
	}
	std::pair < std::string, double> get_pair_of_const(const size_t& index) { // ���������� ���� ��������� (���, ��������)
		return m_list_of_constants[index];
	}
	void add_const(std::string& name, std::string& value) { // ����������� ���������
		if (checkDoubleValue((value))) // �������� ��������� ����� ����������� ����� check �� �����������
			m_list_of_constants.push_back(std::make_pair(name, std::stod(value))); // ���� �������� 1, �� ��� ���� � ������ � �����������
		else
			throw ("ERROR: wrong value of const"); // ���� �������� 0, �� ����� ������
	}
	/////////////////
	/// FUNCTIONS ///
	/////////////////
	std::vector<std::string> m_list_of_fucntions = { "sin", "cos", "tg", "ctg", "ln", "exp" };
	size_t size_of_list_of_funcs() { // ���-�� ������� � ������� m_list_of_priority
		return m_list_of_fucntions.size();
	}
	void print_list_of_func() const { // ����� ���� �������
		std::cout << "LIST OF FUNCTIONS: " << std::endl;
		for (size_t count = 0; count < m_list_of_fucntions.size(); ++count) {
			std::cout << m_list_of_fucntions[count] << std::endl;
		}
	}
	std::string get_name_of_func(const size_t& index) { // ���������� ��� �������
		return m_list_of_fucntions[index];
	}
	void add_func(std::string& name) { // ����������� �������
		m_list_of_fucntions.push_back(name);
	}
	/////////////////
	/// OPERATION ///
	/////////////////

	std::vector<std::pair<std::string, Type_of_operation>> m_list_of_operations = { {"+", Type_of_operation::binary_operation}, {"*", Type_of_operation::binary_operation} ,{"/", Type_of_operation::binary_operation} ,{"-", Type_of_operation::binary_operation} ,
		{"^", Type_of_operation::binary_operation} , {"!", Type_of_operation::unary_operation} };

	size_t size_of_list_of_operation() { // ���-�� ��������� � ������� m_list_of_operations
		return m_list_of_operations.size();
	}
	void print_list_of_operation() const { // ����� ���� �������
		std::cout << "LIST OF OPERANDS: " << std::endl;
		for (size_t count = 0; count < m_list_of_operations.size(); ++count) {
			std::cout << m_list_of_operations[count].first << " = ";
			if (m_list_of_operations[count].second == Type_of_operation::binary_operation)
				std::cout << "binary operation" << std::endl;
			else std::cout << "unary operation" << std::endl;
		}
	}
	std::string get_operation(const size_t& index) { // ���������� ��� �������
		return m_list_of_operations[index].first;
	}
	void add_operation(std::string& name, bool& type) { // ����������� �������
		if (type)
			m_list_of_operations.push_back(std::make_pair(name, Type_of_operation::binary_operation));
		else m_list_of_operations.push_back(std::make_pair(name, Type_of_operation::unary_operation));
	}

public:
	Lecsems() {}
	Lecsems(const std::string& other) {
		for (size_t count = 0; count < other.size(); ++count) { // �������� ������ ������ ������ other

			if (other[count] == '+' || other[count] == '-')
				if (other[count] == '+')
					m_data.push_back({ "+" , Type_of_operation::binary_operation, Priority::operations_add_or_sub });
				else m_data.push_back({ "-" , Type_of_operation::binary_operation, Priority::operations_add_or_sub });
			else if (other[count] == '*' || other[count] == '/')
				if (other[count] == '*')
					m_data.push_back({ "*" , Type_of_operation::binary_operation, Priority::operations_mult_or_div });
				else m_data.push_back({ "/" , Type_of_operation::binary_operation, Priority::operations_mult_or_div });
			else if (other[count] == '(' || other[count] == ')')
				if (other[count] == '(')
					m_data.push_back({ "(" , Type_of_operation::non_operation, Priority::breackets });
				else m_data.push_back({ ")" , Type_of_operation::non_operation, Priority::breackets });
			else if (other[count] == '!')
				m_data.push_back({ "!" , Type_of_operation::unary_operation, Priority::own_operations });
			else if (other[count] == '^')
				m_data.push_back({ "^" , Type_of_operation::binary_operation, Priority::own_operations });
			else if ((other[count] >= 49 && other[count] <= 57) || other[count] == '.') { // ���� ����� (�������� ����� ASCII-�������) ��� �����. ���� ��� ��, �� ������� ����� ����� ���� ����, �� ���� ����� ��������� �������������� ������������ �����
				std::string temp; // ��������������� ������
				do {
					temp += other[count]; // ��������� ����� ��� �����
					++count; // ��������� ����� �� ��������� ������
				} while (((other[count] >= 48 && other[count] <= 57) || other[count] == '.') && count < other.size()); // ������, ���� ���������� ���� �����, ���� �����, ���� count ������ �� ������� ������
				if (!checkDoubleValue(temp)) // ���� �������� � ����������������� ������� ����� (��� ����� ���� �����������, ������� ���� ��������� ����� �� ������������)
					throw ("ERROR: wrong number"); // ���� ����� "������", �� ERROR
				else { // ���� ����� ����, �� ����� ���� � ������ (����� � ���� ������, � ��������� �����)
					m_data.push_back({ temp , Type_of_operation::non_operation, Priority::numbers });
					--count; // ��� ��� �� ������� ���� ������ ++count, ���� ������� --count
				}
			}
			else {
				std::string name; // ������ ��������������� ������ 
				do {
					name += other[count];
					++count;
				} while (((other[count] >= 65 && other[count] <= 90) || (other[count] >= 97 && other[count] <= 122)) && count < other.size()); // ������, ���� ��� ����� (������� ��� ���������) ��� count ������ �� ������� ������
				--count; // ������ ++, ������������� ������ --

				bool exitTemp = false; // �������������� "�������", ����� ����� �� �������� �����, ���� ��������� �������� � ������� m_constants.m_list_of_constants

				for (size_t COUNT = 0; COUNT < m_list_of_constants.size(); ++COUNT) {
					if (name == get_name_of_const(COUNT)) {
						m_data.push_back({ std::to_string(get_value_of_const(COUNT)),Type_of_operation::non_operation ,Priority::constants }); // ����� ���� (������� ���������, ��������� ���������). !����������� �� ���������???
						exitTemp = true;
						break;
					}
				}

				if (name.size() > 1) { // ���������, ������� �� ���� ������ name
					for (size_t COUNT = 0; COUNT < size_of_list_of_funcs(); ++COUNT) {
						if (name == get_name_of_func(COUNT)) {
							m_data.push_back({ name,Type_of_operation::unary_operation ,Priority::functions });
							exitTemp = true;
							break;
						}
					}
					if (!exitTemp) { // ���� ������� �� ������� � ������� m_list_of_fucntions, �� ���������� ������������ � ���������� ����� �������
						std::cout << "IS " << name << " FUNC?" << std::endl;
						std::cout << "IF YES - enter 1. IF NOT - enter 0" << std::endl;
						bool check_for_add_func;
						std::cin >> check_for_add_func;
						if (check_for_add_func)
							add_func(name);
						m_data.push_back({ name,Type_of_operation::unary_operation ,Priority::functions });
						exitTemp = true;
					}
				}
				if (exitTemp) // ���� �������� ��� ������� ��������, �� � ������� "�������" ��������� �� ��������� �������� �����
					continue;
				// ���, ��� ��� �� ���������� � �� ������� -> ������ ���� �����, ���� ���������, ������� ��� � ������� ��������
				// ������� ������� ������������, �������� �� ����� ���������
				bool check_for_add_const; // ������ ���������� ��� �������� ���������� ���������
				std::cout << "IS " << name << " CONST?" << std::endl;
				std::cout << "IF YES - enter 1. IF NOT - enter 0" << std::endl;
				std::cin >> check_for_add_const;

				if (check_for_add_const == true) { // ���� 1, �� ��������� ���������
					std::cout << "ENTER THE VALUE OF " << name << ": ";
					std::string value_of_const; // ���������� ��� ����� �������� ���������
					std::cin >> value_of_const;
					add_const(name, value_of_const); // ��������� ���������, ��� ������ ����������� ��������� �� �����������, ���� ����� ��� ����� �������, ���� ������ �������� �� ����� � �����-������ ��������(���� ����� ��� ���������)
					m_data.push_back({ value_of_const,Type_of_operation::non_operation ,Priority::constants });
				}
				else { // ���� 0, �� ������ 100% �� ���������
					m_data.push_back({ name,Type_of_operation::non_operation ,Priority::letters });
				}
			}
		}
	}
	Lecsems& operator=(const Lecsems& other) {
		if (this == &other)
			return *this;
		m_list_of_operations = other.m_list_of_operations;
		m_list_of_fucntions = other.m_list_of_fucntions;
		m_list_of_constants = other.m_list_of_constants;
		m_data = other.m_data;
		return *this;
	}
	bool check() {
		state status = state::start;
		size_t count_of_breckets = 0;

		for (size_t count = 0; count < m_data.size(); ++count) { // �������� ������, ��������� �� ��� (��������, ��������)
			/*
			������� � ��� ����� ���� ��������� �����:
				1) ������
				2) �������� ��������
				3) ������� �������
				4) �����
				5) ���������
				6) �����
				7) �������
			*/
			switch (status)
			{
			case Lecsems::state::start: // �����, ������ �������, ������ ������� � ������� m_data

				/*
					��� ����� ����:
						1) ������, ������ ( -> ���� ���� ������� ������� - ���� �����, �����, �������, ���������, ���� ������ (
								���� ) -> ����� � error
						2) ���� �������� ������� -> ����� � error
						3) ���� ������� ������� -> ����� � error
						4) �����, ��������� ��� ����� - ��� ��
						5) ������� -> ���� ������ (
				*/

				if (std::get<std::string>(m_data[count]) == "(") { // ����� ( ����� ���� ���� ������� -, ���� �������, ���� ������
					++count_of_breckets;
					status = Lecsems::state::wait_unarn_op_or_operand_or_brackets;
				}
				else if (std::get<Priority>(m_data[count]) == Priority::constants || std::get<Priority>(m_data[count]) == Priority::letters || std::get<Priority>(m_data[count]) == Priority::numbers) // ����� �����, ��������� ��� �����, ����� ���� ������ ������� ��� ������������� ������
					status = Lecsems::state::waiting_operator_or_brackets;
				else if (std::get<Priority>(m_data[count]) == Priority::functions) // ����� ������� �� ���� ������ ������������� ������
					status = Lecsems::state::waiting_open_brackets;
				else if (std::get<std::string>(m_data[count]) == ")") // ���� ������ ), �� �����
					status = Lecsems::state::error;
				else // ��������� ������ = �����
					status = Lecsems::state::error;
				break;

			case Lecsems::state::waiting_operator:
				/*
					��� ����� ����:
						1) �������� -> ���� �����, ���������, �����
						2) ������ -> ������
						3) ... -> ������
				*/
				if (std::get<Type_of_operation>(m_data[count]) == Type_of_operation::binary_operation)
					status = Lecsems::state::start;
				else status = Lecsems::state::error;
				break;

			case Lecsems::state::waiting_open_brackets:

				/*
					��� ����� ����:
						1) ������������� ������, �� ��������� �� �����
						2) ��������� ������ -> �����
				*/

				if (std::get<std::string>(m_data[count]) == "(") {
					++count_of_breckets;
					status = Lecsems::state::start;
				}
				else status = Lecsems::state::error;
				break;

			case Lecsems::state::waiting_operator_or_brackets:

				/*
					��� ����� ����:
						1) ������ ) -> ���� ���� ��������, ���� ������ )
						2) ������ (
						3) �������� �������
						4) �����, ���������, �����
						5) ���������
						6) �������
				*/

				if (std::get<std::string>(m_data[count]) == ")") {
					--count_of_breckets;
					status = Lecsems::state::waiting_operator_or_brackets;
				}
				else if (std::get<Type_of_operation>(m_data[count]) == Type_of_operation::binary_operation)
					status = Lecsems::state::start;
				else if (std::get<std::string>(m_data[count]) == "!")
					status = Lecsems::state::waiting_close_brackets;
				else if (std::get<std::string>(m_data[count]) == "^")
					status = Lecsems::state::wait_operand;
				else status = Lecsems::state::error;
				break;

			case Lecsems::state::wait_numb_or_brackets:
				/*
					��� ����� ���� ���� �������
				*/

				break;

			case Lecsems::state::wait_unarn_op_or_operand_or_brackets:

				/*
					��� ����� ����:
						1) - = ������� �����, ����� ���� ����� ���� ������� ��� �����
						2) �����, �� ���� �������� ��� ������ �������������
						3) �������, �� ���� ������ ������������ ������
						4) (, �� ��������� �� �����
						5) ��������� ������ -> �����
				*/

				if (std::get<std::string>(m_data[count]) == "-")
					status = Lecsems::state::wait_operand;
				else if (std::get<Priority>(m_data[count]) == Priority::constants || std::get<Priority>(m_data[count]) == Priority::letters || std::get<Priority>(m_data[count]) == Priority::numbers)
					status = Lecsems::state::waiting_operator_or_brackets;
				else if (std::get<Priority>(m_data[count]) == Priority::functions)
					status == Lecsems::state::waiting_open_brackets;
				else if (std::get<std::string>(m_data[count]) == "(") {
					status = Lecsems::state::start;
					++count_of_breckets;
				}
				else status = Lecsems::state::error;
				break;

			case Lecsems::state::wait_operand:

				/*
					��� ����� ����:
						1) �����, �� ���� ������ ������������� ������
						2) �������, �� ���� ����� ������������� ������
						3) ��������� ������ -> �����
				*/

				if (std::get<Priority>(m_data[count]) == Priority::constants || std::get<Priority>(m_data[count]) == Priority::letters || std::get<Priority>(m_data[count]) == Priority::numbers) status = Lecsems::state::waiting_close_brackets;
				else if (std::get<Priority>(m_data[count]) == Priority::functions) status = Lecsems::state::waiting_open_brackets;
				else status = Lecsems::state::error;
				break;

			case Lecsems::state::waiting_close_brackets:

				/*
					��� ����� ����:
						1) ������������� ������, �� ���� ������ �������� ��� ������������� ������
						2) ��������� ������ -> �����
				*/

				if (std::get<std::string>(m_data[count]) == ")") {
					status = Lecsems::state::waiting_operator_or_brackets;
					--count_of_breckets;
				}
				else status = Lecsems::state::error;
				break;

			case Lecsems::state::error:
				return false;
			default:
				return false;
			}
		}

		if (!count_of_breckets) { // ���� ����� ������ == 0, �� ��������� true, � �������� ������� ���������� false
			convertExpression();
			return 1;
		}
		else return 0;
	}

	void convertExpression() { // �������, ��� ������������� ��������� � ������� ������� � ���������� ��������� (����� ��� ��������)
		for (size_t count = 0; count < m_data.size(); ++count) {
			if (std::get<std::string>(m_data[count]) == "(" && std::get<std::string>(m_data[count + 1]) == "-" && (std::get<Lecsems::Priority>(m_data[count + 2]) == Lecsems::Priority::numbers || std::get<Lecsems::Priority>(m_data[count + 2]) == Lecsems::Priority::constants || std::get<Lecsems::Priority>(m_data[count + 2]) == Lecsems::Priority::letters) && std::get<std::string>(m_data[count + 3]) == ")") {
				std::get<Lecsems::Type_of_operation>(m_data[count + 1]) = Lecsems::Type_of_operation::unary_operation;
			}
		}
	}

	friend std::ostream& operator<<(std::ostream& out, const Lecsems& other) {
		for (size_t count = 0; count < other.m_data.size(); ++count) {
			out << "LECSEM IS " << std::get<std::string>(other.m_data[count]) << "; PRIORITET IS ";
			switch (std::get<Priority>(other.m_data[count])) {
			case (Priority::numbers):
			case (Priority::letters):
			case (Priority::constants): out << "OPERAND"; break;
			case (Priority::functions): out << "FUNC"; break;
			case (Priority::operations_add_or_sub): out << "ADD OR SUB"; break;
			case (Priority::operations_mult_or_div): out << "MULT OR DIV"; break;
			case (Priority::own_operations): out << "OWN OPERATIONS"; break;
			case (Priority::breackets): out << "BREACKETS"; break;
			default: break;
			}
			out << "; TYPE OF OPERATORS IS ";
			switch (std::get<Type_of_operation>(other.m_data[count]))
			{
			case (Type_of_operation::non_operation): out << "NON OPERATOR\n"; break;
			case (Type_of_operation::unary_operation): out << "UNARY OPERATORS\n"; break;
			case (Type_of_operation::binary_operation): out << "BINARY OPERATORS\n"; break;
			default:
				break;
			}
		}
		return out;
	}
};

class Arithmetic�alculation {
private:
	Lecsems m_expression;
	std::vector<std::tuple<std::string, Lecsems::Type_of_operation, Lecsems::Priority>> m_data;
	std::vector<std::tuple<std::string, Lecsems::Type_of_operation, Lecsems::Priority>> m_postfix;

public:
	Arithmetic�alculation(Lecsems& expression) {
		m_expression = expression;
		if (!m_expression.check())
			throw ("ERROR: wrong expression");
		m_data = m_expression.m_data;
	}

	std::vector<std::tuple<std::string, Lecsems::Type_of_operation, Lecsems::Priority>> getPostfix() {
		std::stack<std::tuple<std::string, Lecsems::Type_of_operation, Lecsems::Priority>> stack;
		for (size_t count = 0; count < m_data.size(); ++count) {
			if (std::get<Lecsems::Priority>(m_data[count]) == Lecsems::Priority::numbers || std::get<Lecsems::Priority>(m_data[count]) == Lecsems::Priority::letters || std::get<Lecsems::Priority>(m_data[count]) == Lecsems::Priority::constants)
				m_postfix.push_back(m_data[count]);
			else if (std::get<Lecsems::Priority>(m_data[count]) == Lecsems::Priority::functions || std::get<Lecsems::Type_of_operation>(m_data[count]) == Lecsems::Type_of_operation::unary_operation)
				stack.push(m_data[count]);
			else if (std::get<std::string>(m_data[count]) == "(")
				stack.push(m_data[count]);
			else if (std::get<std::string>(m_data[count]) == ")") {
				while (std::get<std::string>(stack.top()) != "(") {
					m_postfix.push_back(stack.top());
					stack.pop();
				}
				stack.pop();
			}
			else if (std::get<std::string>(m_data[count]) == "^")
				stack.push(m_data[count]);
			else if (std::get<Lecsems::Priority>(m_data[count]) == Lecsems::Priority::operations_add_or_sub || std::get<Lecsems::Priority>(m_data[count]) == Lecsems::Priority::operations_mult_or_div) {
				while (!stack.empty() && ((std::get<Lecsems::Priority>(m_data[count]) == Lecsems::Priority::operations_add_or_sub && (std::get<Lecsems::Priority>(stack.top()) == Lecsems::Priority::operations_add_or_sub || std::get<Lecsems::Priority>(stack.top()) == Lecsems::Priority::operations_mult_or_div)) || (std::get<Lecsems::Priority>(m_data[count]) == Lecsems::Priority::operations_mult_or_div && std::get<Lecsems::Priority>(stack.top()) == Lecsems::Priority::operations_mult_or_div))) {
					m_postfix.push_back(stack.top());
					stack.pop();
				}
				stack.push(m_data[count]);
			}
		}
		while (!stack.empty()) {
			m_postfix.push_back(stack.top());
			stack.pop();
		}
		/*
		���������:	15 7 1 1 + - / 3 * 2 1 1 + + 15 * 7 200 1 + - / 3 * - 2 1 1 + + 15 7 1 1 + - / 3 * 2 1 1 + + - 15 7 1 1 + - / 3 * + 2 1 1 + + - * -
		����:		15 7 1 1 + - / 3 * 2 1 1 + + 15 * 7 200 1 + - / 3 * - 2 1 1 + + 15 7 1 1 + - / 3 * 2 1 1 + + - 15 7 1 1 + - / 3 * + 2 1 1 + + - * -
		*/
		return m_postfix;
	}

	std::string calculating() {
		getPostfix();

		std::stack<std::string> stack;

		size_t temp = 1;

		for (size_t count = 0; count < m_postfix.size(); ++count) {
			if (std::get<Lecsems::Priority>(m_postfix[count]) == Lecsems::Priority::numbers || std::get<Lecsems::Priority>(m_postfix[count]) == Lecsems::Priority::constants) {
				stack.push(std::get<std::string>(m_postfix[count]));
			}
			else if (std::get<Lecsems::Priority>(m_postfix[count]) == Lecsems::Priority::letters) {
				std::cout << "ENTER THE VALUE OF " << std::get<0>(m_postfix[count]) << ": ";
				std::cin >> std::get<std::string>(m_postfix[count]);
				if (!m_expression.checkDoubleValue(std::get<std::string>(m_postfix[count])))
					throw ("ERROR: wrong number");
				std::get<Lecsems::Priority>(m_postfix[count]) = Lecsems::Priority::numbers;
				stack.push(std::get<std::string>(m_postfix[count]));
			}
			else if (std::get<Lecsems::Type_of_operation>(m_postfix[count]) == Lecsems::Type_of_operation::unary_operation) {
				std::string operand = stack.top();
				stack.pop();
				if (std::get<std::string>(m_postfix[count]) == "sin")
					stack.push(std::to_string(sin(std::stod(operand))));
				else if (std::get<std::string>(m_postfix[count]) == "cos")
					stack.push(std::to_string(cos(std::stod(operand))));
				else if (std::get<std::string>(m_postfix[count]) == "tg")
					stack.push(std::to_string(tan(std::stod(operand))));
				else if (std::get<std::string>(m_postfix[count]) == "ln") {
					if (std::stod(operand) <= 0)
						throw ("ERROR: wrong log expression");
					stack.push(std::to_string(log10(std::stod(operand))));
				}
				else if (std::get<std::string>(m_postfix[count]) == "exp")
					stack.push(std::to_string(exp(std::stod(operand))));
				else if (std::get<std::string>(m_postfix[count]) == "-")
					stack.push(std::to_string((-1) * (std::stod(operand))));
				else if (std::get<std::string>(m_postfix[count]) == "!") {
					std::string reuslt = "1";
					for (size_t fact_iterator = 2; fact_iterator <= std::stod(operand); ++fact_iterator)
						reuslt = std::to_string(std::stod(reuslt) * fact_iterator);
					stack.push(reuslt);
				}
				/*
					���� �� ������� �����-�� ����� �������, ���������� �������� � ����.
					�� �������:
					else if (std::get<std::string>(m_postfix[count]) == "FUNC") {
					if (std::stod(operand) <= 0) // ���� ���� �����-�� �����������
						throw ("ERROR: wrong FUNC expression");
					stack.push(std::to_string(FUNC(std::stod(operand))));
				*/
			}
			else if (std::get<Lecsems::Type_of_operation>(m_postfix[count]) == Lecsems::Type_of_operation::binary_operation) {
				if (std::get<std::string>(m_postfix[count]) == "+") {
					std::string rightOperand = stack.top();
					stack.pop();
					double leftOperand = std::stod(stack.top());
					stack.pop();
					//std::cout << temp++ << ") " << leftOperand << "+" << rightOperand << "=" << leftOperand + std::stod(rightOperand) << std::endl;
					stack.push((std::to_string(leftOperand + std::stod(rightOperand))));
				}
				else if (std::get<std::string>(m_postfix[count]) == "^") {
					std::string rightOperand = stack.top();
					stack.pop();
					double leftOperand = std::stod(stack.top());
					stack.pop();
					//std::cout << temp++ << ") " << leftOperand << "-" << rightOperand << "=" << leftOperand ^ std::stod(rightOperand) << std::endl;
					stack.push((std::to_string(pow(leftOperand, std::stod(rightOperand)))));
				}
				else if (std::get<std::string>(m_postfix[count]) == "-") {
					std::string rightOperand = stack.top();
					stack.pop();
					double leftOperand = std::stod(stack.top());
					stack.pop();
					//std::cout << temp++ << ") " << leftOperand << "-" << rightOperand << "=" << leftOperand - std::stod(rightOperand) << std::endl;
					stack.push((std::to_string(leftOperand - std::stod(rightOperand))));
				}
				else if (std::get<std::string>(m_postfix[count]) == "*") {
					std::string rightOperand = stack.top();
					stack.pop();
					double leftOperand = std::stod(stack.top());
					stack.pop();
					//std::cout << temp++ << ") " << leftOperand << "*" << rightOperand << "=" << leftOperand * std::stod(rightOperand) << std::endl;
					stack.push((std::to_string(leftOperand * std::stod(rightOperand))));
				}
				else if (std::get<std::string>(m_postfix[count]) == "/") {
					std::string rightOperand = stack.top();
					stack.pop();
					double leftOperand = std::stod(stack.top());
					stack.pop();
					//std::cout << temp++ << ") " << leftOperand << "/" << rightOperand << "=" << leftOperand / std::stod(rightOperand) << std::endl;
					stack.push((std::to_string(leftOperand / std::stod(rightOperand))));
				}
			}
		}
		return stack.top();
	}

	void printPostfix() {
		for (size_t count = 0; count < m_postfix.size(); ++count) {
			std::string temp = std::get<std::string>(m_postfix[count]);
			std::cout << temp << " ";
		}
	}

	std::string getStringOfPostfix() {
		std::string temp;
		for (size_t count = 0; count < m_postfix.size(); ++count) {
			temp += std::get<std::string>(m_postfix[count]);
		}
		return temp;
	}
};

#endif // !ARIFMETIC_EXPRESSION_H