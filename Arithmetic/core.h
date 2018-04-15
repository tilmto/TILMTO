#include <string>

/*
 * set_setting
 *		max_operators: the maximum number of the operators				default value: 5
 *		max_range: the range of the numbers are [0,max_range)			default value: 1000
 *		precision: the precision of decimals							default value: 2
 *		has_fraction: 1 for supporting fractions, 0 for not supporting	default value: 1
 *		has_real: 1 for supporting decimals, 0 for not supporting		default value: 1
 *		has_mul_div: 1 for supporting *,/ , 0 for not supporting		default value: 1
 *		has_power: 1 for supporting ^, 0 for not supporting				default value: 1
 * if you pass -1, then the setting won't be modified.
 *
 * example:
 *		set_setting(4,2000,3,-1,-1,-1,0);
 *		set_setting(6,-1,-1,-1,-1,-1,-1);			// I just want to chage the first parameter.
 */
void set_setting(int max_opearators, long max_range, int precision,
							int has_fraction, int has_real,int has_mul_div,int has_power);

/*
 * generate
 * 
 * example:
 		int n = 10;
 		for(int i=0;i<n;i++)
		{			
 			string question;				
 			string* answer = new string();
			generate(&question,answer);
			cout<<question<<" = "<<answer<<endl;
		}
 */
void generate(std::string* question, std::string* answer);
