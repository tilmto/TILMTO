/*
* core.cpp
* author:
*   PB16120853 赵瑞
*   JL17110067 隆晋威
* date:
*  2018/4/5
*/

#include <iostream>
#include <string>
#include <cstdio>
#include <tuple>
#include <cmath>
#include <sstream>
#include <set>

using namespace std;

/*
* global setting
*/
struct settings {
	int max_opearators = 5;
	long max_num = 100;			// max_range / 10
	long max_range = 1000;
	int precision = 2;
    bool has_fraction = false;
	bool has_real = true;
	bool has_mul_div = true;
	bool has_power = true;
};
settings global_setting;


/*
* fraction
*/
class fraction;
bool is_bad_value(const fraction& x);
bool is_bad_value(const double& x);



class fraction {
private:
	long gcd(long u, long v) {
		if (!(u&&v)) {
			return 1;
		}
		while (v != 0) {
			long r = u % v;
			u = v;
			v = r;
		}
		return u;
	}
public:
	long numerator, denominator;


	fraction(long m = 1, long n = 1) {
		this->numerator = m;
		this->denominator = n;
		this->reduction();
	}

	void reduction() {
		long x = gcd(this->numerator, this->denominator);
		if ((llabs(this->denominator) > global_setting.max_range) || (llabs(this->numerator) > global_setting.max_range)) {
			this->numerator = 1;
			this->denominator = 0;
			x = 1;
		}
		this->numerator /= x;
		this->denominator /= x;
		if (this->denominator < 0) {
			this->numerator *= -1;
			this->denominator *= -1;
		}
		if (!this->numerator) {
			this->denominator = 1;
		}
		if (!this->denominator) {
			this->numerator = 1;
		}
		if ((abs(this->denominator)>global_setting.max_range) || (abs(this->numerator) > global_setting.max_range)) {
			this->numerator = 1;
			this->denominator = 0;
		}
		return;
	}

	fraction operator + (const fraction x) const {
		if (is_bad_value(*this))return *this;
		if (is_bad_value(x))return x;
		fraction result;
		result.numerator = this->numerator * x.denominator + this->denominator * x.numerator;
		result.denominator = this->denominator * x.denominator;
		result.reduction();
		return result;
	}


	fraction operator - (const fraction& x) const {
		if (is_bad_value(*this))return *this;
		if (is_bad_value(x))return x;
		fraction result;
		result.numerator = this->numerator * x.denominator - this->denominator * x.numerator;
		result.denominator = this->denominator * x.denominator;
		result.reduction();
		return result;
	}

	fraction operator * (const fraction& x) const {
		if (is_bad_value(*this))return *this;
		if (is_bad_value(x))return x;
		fraction result;
		result.numerator = this->numerator * x.numerator;
		result.denominator = this->denominator * x.denominator;
		result.reduction();
		return result;
	}

	fraction operator / (const fraction& x) const {
		if (is_bad_value(*this))return *this;
		if (is_bad_value(x))return x;
		fraction stan_bad_value(1, 0);
		if (x.numerator == 0) {
			return stan_bad_value;
		}
		fraction result;
		result.numerator = this->numerator * x.denominator;
		result.denominator = this->denominator * x.numerator;
		result.reduction();
		if (is_bad_value(result)) {
			result.numerator = 1;
			result.denominator = 0;
		}
		return result;
	}

	fraction operator ^ (fraction x) const {
		if (is_bad_value(*this))return *this;
		if (is_bad_value(x))return x;

		x.reduction();
		if (x.denominator != 1) {
			fraction bad_value;
			bad_value.numerator = 1;
			bad_value.denominator = 0;
			return bad_value;
		}
		long index = x.numerator;

		fraction result;
		result.numerator = (long)powl(this->numerator, abs(index));
		result.denominator = (long)powl(this->denominator, abs(index));
		if (index < 0) {
			long temp;
			temp = result.numerator;
			result.numerator = result.denominator;
			result.denominator = temp;
		}
		result.reduction();
		return result;
	}
};

ostream& operator << (ostream& out, const fraction& frac) {
	long long n = frac.numerator;
	long long d = frac.denominator;
	long long integer = n / d;
	long long f = n % d;

	if (f == 0) {
		out << integer;
	}
	else {
		if (integer) {
			out << integer << '\'' << f << '/' << d;
		}
		else {
			out << f << '/' << d;
		}
	}
	return out;
}

/*
* unit test for fraction
*/
void fraction_test() {
	fraction a(1, 0), b(0, 1), c(2, 3), d(5, 6), e(8, 4), g(18, 9);
	fraction x;
	x = a + b;
	x = a * b;
	x = a - b;
	x = a / b;
	x = a / c;
	x = a + c;
	x = a * c;
	x = a - c;
	x = b + c;
	x = b - c;
	x = b * c;
	x = b / c;
	x = c * d;
	x = c / d;
	x = c + d;
	x = c - d;
	x = a ^ b;
	x = a ^ c;
	x = a ^ d;
	x = a ^ e;
	x = a ^ g;
	x = b ^ c;
	x = b ^ a;
	x = b ^ d;
	x = b ^ e;
	x = b ^ g;
	x = e * g;
	x = e - g;
	x = e + g;
	x = e / g;
}

/*
* is_bad_value
*/
bool is_bad_value(const fraction& x) {
	if (!x.denominator) {
		return true;
	}
	else {
		return false;
	}
}

bool is_bad_value(const double& x) {
	// todo: error
	if (isnan(x) || isinf(x)) {
		return true;
	}
	else {
		return false;
	}
}

/*
* AST
*/
enum ASTNodeType { TYPE_ADD = 0, TYPE_MINUS = 1, TYPE_MUL = 2, TYPE_DIV = 3, TYPE_POWER = 4, TYPE_FRACTION = 5, TYPE_DOUBLE = 6 };

struct ASTNode;

union NodeData {
	fraction frac;
	double real;
	pair<ASTNode*, ASTNode*> node;

	NodeData() {
		real = 0;
	}
};

struct ASTNode {
	ASTNodeType type;
	NodeData data;

	ASTNode() {
		type = TYPE_DOUBLE;
	}

	~ASTNode() {
		if (type != TYPE_FRACTION && type != TYPE_DOUBLE) {
			delete data.node.first;
			delete data.node.second;
		}
	}
};



/*
* random ast
*/
enum cal_mode { MODE_FRACTION, MODE_REAL };

inline ASTNode* random_value(cal_mode mode) {
	ASTNode* node = new ASTNode();
	int m, n;
	switch (mode) {
	case MODE_FRACTION:
		node->type = TYPE_FRACTION;
		m = rand() % (global_setting.max_num - 1) + 1;
		n = rand() % (m * 5);
		if (global_setting.has_fraction) {
			node->data.frac = fraction(n, m);
		}
		else {
			node->data.frac = fraction(m);
		}
		break;

	case MODE_REAL:
		node->type = TYPE_DOUBLE;
		double base = pow(10, global_setting.precision);
		node->data.real = floor((rand() / (double)RAND_MAX)*global_setting.max_num*base) / base;
		break;
	}
	return node;
}

ASTNode* random_ast(cal_mode mode) {
	int n = global_setting.max_opearators <= 1 ? 1 : rand() % (global_setting.max_opearators - 1) + 1;
	ASTNode* num1 = random_value(mode);
	for (int i = 0; i<n; i++) {
		ASTNode* num2 = random_value(mode);
		if (rand() % 2) swap(num1, num2);
		int r = rand() % 17;
		ASTNode* new_node = new ASTNode();


		if (r-- == 16 && (num2->type == TYPE_FRACTION || num2->type == TYPE_FRACTION) && (num1->type != TYPE_POWER) && global_setting.has_power) {
			if (mode == MODE_FRACTION) num2->data.frac = fraction(rand() % 4 + 1);
			else num2->data.real = rand() % 2 + 2;

			new_node->type = TYPE_POWER;
			new_node->data.node.first = num1;
			new_node->data.node.second = num2;
		}
		else {
			if (global_setting.has_mul_div) {
				new_node->type = (ASTNodeType)(r / 4);
				if (mode == MODE_FRACTION && !global_setting.has_fraction) {
					r = rand() % 10;
					if (r-- == 9) new_node->type = TYPE_DIV;
					else new_node->type = (ASTNodeType)(r / 3);
				}
			}
			else {
				new_node->type = (ASTNodeType)(r / 8);
			}
			new_node->data.node.first = num1;
			new_node->data.node.second = num2;
		}

		num1 = new_node;
	}
	return num1;
}


long long hash_value;
ASTNode* calc_asttree(ASTNode* root) {
	ASTNode* result = new ASTNode();
	result->type = TYPE_FRACTION;
	result->data.frac;
	ASTNode* temp_a = new ASTNode();
	ASTNode* temp_b = new ASTNode();
	switch (root->type) {
	case TYPE_FRACTION:
		result->type = TYPE_FRACTION;
		result->data.frac = root->data.frac;
		break;
	case TYPE_DOUBLE:
		result->type = TYPE_DOUBLE;
		result->data.real = root->data.real;
		break;
	case TYPE_ADD:
		temp_a = calc_asttree(root->data.node.first);
		temp_b = calc_asttree(root->data.node.second);
		if (temp_a->type == TYPE_FRACTION && temp_b->type == TYPE_FRACTION) {
			result->type = TYPE_FRACTION;
			result->data.frac = temp_a->data.frac + temp_b->data.frac;
		}
		else {
			result->type = TYPE_DOUBLE;
			double a, b;
			if (temp_a->type == TYPE_FRACTION) {
				a = (double)temp_a->data.frac.numerator / (double)temp_a->data.frac.denominator;
			}
			else if (temp_a->type == TYPE_DOUBLE) {
				a = temp_a->data.real;
			}
			if (temp_b->type == TYPE_FRACTION) {
				b = (double)temp_b->data.frac.numerator / (double)temp_b->data.frac.denominator;
			}
			else if (temp_b->type == TYPE_DOUBLE) {
				b = temp_b->data.real;
			}
			result->data.real = is_bad_value(a) || is_bad_value(b) ? INFINITY : (a + b);
		}
		break;
	case TYPE_MINUS:
		temp_a = calc_asttree(root->data.node.first);
		temp_b = calc_asttree(root->data.node.second);
		if (temp_a->type == TYPE_FRACTION && temp_b->type == TYPE_FRACTION) {
			result->type = TYPE_FRACTION;
			result->data.frac = temp_a->data.frac - temp_b->data.frac;

		}
		else {
			result->type = TYPE_DOUBLE;
			double a, b;
			if (temp_a->type == TYPE_FRACTION) {
				a = (double)temp_a->data.frac.numerator / (double)temp_a->data.frac.denominator;
			}
			else if (temp_a->type == TYPE_DOUBLE) {
				a = temp_a->data.real;
			}
			if (temp_b->type == TYPE_FRACTION) {
				b = (double)temp_b->data.frac.numerator / (double)temp_b->data.frac.denominator;
			}
			else if (temp_b->type == TYPE_DOUBLE) {
				b = temp_b->data.real;
			}
			result->data.real = is_bad_value(a) || is_bad_value(b) ? INFINITY : (a - b);
		}
		break;
	case TYPE_MUL:
		temp_a = calc_asttree(root->data.node.first);
		temp_b = calc_asttree(root->data.node.second);
		if (temp_a->type == TYPE_FRACTION && temp_b->type == TYPE_FRACTION) {
			result->type = TYPE_FRACTION;
			result->data.frac = temp_a->data.frac * temp_b->data.frac;

		}
		else {
			result->type = TYPE_DOUBLE;
			double a, b;
			if (temp_a->type == TYPE_FRACTION) {
				a = (double)temp_a->data.frac.numerator / (double)temp_a->data.frac.denominator;
			}
			else if (temp_a->type == TYPE_DOUBLE) {
				a = temp_a->data.real;
			}
			if (temp_b->type == TYPE_FRACTION) {
				b = (double)temp_b->data.frac.numerator / (double)temp_b->data.frac.denominator;
			}
			else if (temp_b->type == TYPE_DOUBLE) {
				b = temp_b->data.real;
			}
			result->data.real = is_bad_value(a) || is_bad_value(b) ? INFINITY : (a*b);
		}
		break;
	case TYPE_DIV:
		temp_a = calc_asttree(root->data.node.first);
		temp_b = calc_asttree(root->data.node.second);
		if (temp_a->type == TYPE_FRACTION && temp_b->type == TYPE_FRACTION) {
			result->type = TYPE_FRACTION;
			result->data.frac = temp_a->data.frac / temp_b->data.frac;

		}
		else {
			result->type = TYPE_DOUBLE;
			double a, b;
			if (temp_a->type == TYPE_FRACTION) {
				a = (double)temp_a->data.frac.numerator / (double)temp_a->data.frac.denominator;
			}
			else if (temp_a->type == TYPE_DOUBLE) {
				a = temp_a->data.real;
			}
			if (temp_b->type == TYPE_FRACTION) {
				b = (double)temp_b->data.frac.numerator / (double)temp_b->data.frac.denominator;
			}
			else if (temp_b->type == TYPE_DOUBLE) {
				b = temp_b->data.real;
			}
			result->data.real = is_bad_value(a) || is_bad_value(b) || fabs(b) <= 1e-3 ? INFINITY : (a / b);
		}
		break;
	case TYPE_POWER:
		temp_a = calc_asttree(root->data.node.first);
		temp_b = calc_asttree(root->data.node.second);
		if (temp_a->type == TYPE_FRACTION && temp_b->type == TYPE_FRACTION) {
			result->type = TYPE_FRACTION;
			result->data.frac = temp_a->data.frac ^ temp_b->data.frac;

		}
		else {
			result->type = TYPE_DOUBLE;
			double a, b;
			if (temp_a->type == TYPE_FRACTION) {
				a = (double)temp_a->data.frac.numerator / (double)temp_a->data.frac.denominator;
			}
			else if (temp_a->type == TYPE_DOUBLE) {
				a = temp_a->data.real;
			}
			if (temp_b->type == TYPE_FRACTION) {
				b = (double)temp_b->data.frac.numerator / (double)temp_b->data.frac.denominator;
			}
			else if (temp_b->type == TYPE_DOUBLE) {
				b = temp_b->data.real;
			}
			result->data.real = is_bad_value(a) || is_bad_value(b) ? INFINITY : powl(a, b);
		}
		break;
	}
	long long value = (long long)(root->type == TYPE_FRACTION ? (root->data.frac.numerator / (double)root->data.frac.denominator) : root->data.real);
	hash_value = (hash_value * 19260817 + value) % (long long)(1e9 + 7);
	delete temp_a;
	delete temp_b;
	if (result->type == TYPE_FRACTION) {
		if ( (result->data.frac.denominator > global_setting.max_range*100 || result->data.frac.numerator < 0) || 
			 (result->data.frac.denominator != 1 && !global_setting.has_fraction)) {
			result->data.frac.numerator = 1;
			result->data.frac.denominator = 0;
		} 
	} else if (result->type == TYPE_DOUBLE && (result->data.real <0|| result->data.real>global_setting.max_range*10)) {
		result->data.real = INFINITY;
	}
	return result;
}

ASTNode* ast_eval(ASTNode* root) {
	hash_value = 0;
	return calc_asttree(root);
}

/*
* Expr := AddExpr | Expr + AddExpr
* AddExpr := MulExpr | AddExpr * MulExpr
* MulExpr := PowExpr | MulExpr ^ PowExpr
* PowExpr := Number | (Expr)
*/
enum ExprType { EXPR_EXPR, EXPR_ADDEXPR, EXPR_MULEXPR, EXPR_POWEXPR };

void ast_output_expr(ASTNode* root, stringstream& ss);
void ast_output_addexpr(ASTNode* root, stringstream& ss);
void ast_output_mulexpr(ASTNode* root, stringstream& ss);
void ast_output_powexpr(ASTNode* root, stringstream& ss);
void ast_output_number(ASTNode* root, stringstream& ss);

void ast_output_expr(ASTNode* root, stringstream& ss) {
	switch (root->type) {
	case TYPE_ADD:case TYPE_MINUS:
		ast_output_expr(root->data.node.first, ss);
		ss << (root->type == TYPE_ADD ? " + " : " - ");
		ast_output_addexpr(root->data.node.second, ss);
		break;

	default:
		ast_output_addexpr(root, ss);
		break;
	}
}

void ast_output_addexpr(ASTNode* root, stringstream& ss) {
	switch (root->type) {
	case TYPE_MUL:case TYPE_DIV:
		ast_output_addexpr(root->data.node.first, ss);
		ss << (root->type == TYPE_MUL ? " * " : " / ");
		ast_output_mulexpr(root->data.node.second, ss);
		break;

	default:
		ast_output_mulexpr(root, ss);
		break;
	}
}

void ast_output_mulexpr(ASTNode* root, stringstream& ss) {
	switch (root->type) {
	case TYPE_POWER:
		ast_output_mulexpr(root->data.node.first, ss);	
		ss << "^";
		ast_output_powexpr(root->data.node.second, ss);
		break;
	default:
		ast_output_powexpr(root, ss);
		break;
	}
}

void ast_output_powexpr(ASTNode* root, stringstream& ss) {
	switch (root->type) {
	case TYPE_FRACTION:
		ss << root->data.frac;
		break;
	case TYPE_DOUBLE:
		ss << root->data.real;
		break;
	default:
		ss << '(';
		ast_output_expr(root, ss);
		ss << ')';
		break;
	}
}

set<long long> ans_set;

void set_setting(
	int max_opearators,
	long max_range,
	int precision,
	int has_fraction,
	int has_real,
	int has_mul_div,
	int has_power) {

	if (max_opearators != -1) global_setting.max_opearators = max_opearators;
	if (max_range != -1) global_setting.max_range = max_range;
	if (precision != -1) global_setting.precision = precision;
	if (has_fraction != -1) global_setting.has_fraction = has_fraction != 0;
	if (has_real != -1) global_setting.has_real = has_real != 0;
	if (has_mul_div != -1) global_setting.has_mul_div = has_mul_div != 0;
	if (has_power != -1) global_setting.has_power = has_power != 0;
	global_setting.max_num = max_range / 10;
}

void generate(string* question, string* answer) {
	cal_mode mode;
	int magic = global_setting.has_fraction ? 200 : 3;
	if (global_setting.has_real && rand() % magic == 0) {
		mode = MODE_REAL;
	} else{
		mode = MODE_FRACTION;
	}
	question->clear();
	answer->clear();

	ASTNode* node = random_ast(mode);
	ASTNode* ret = ast_eval(node);
	bool bad_value = false;

	stringstream s1, s2;
	s1.setf(std::ios::fixed, std::ios::floatfield);
	s2.setf(std::ios::fixed, std::ios::floatfield);

	s2.precision(global_setting.precision);
	if (ret->type == TYPE_DOUBLE && !is_bad_value(ret->data.real)) {
		s2 << ret->data.real;
	}
	else if (ret->type == TYPE_FRACTION && !is_bad_value(ret->data.frac)) {
		s2 << ret->data.frac;
	}
	else {
		bad_value = true;
	}
	*answer = s2.str();

	if (bad_value || ans_set.find(hash_value) != ans_set.end()) {
		generate(question, answer);
		delete node;
		delete ret;
		return;
	}
	else {
		ans_set.insert(hash_value);
	}

	s1.precision(global_setting.precision);
	ast_output_expr(node, s1);
	*question = s1.str();

	delete node;
	delete ret;

	return;
}
