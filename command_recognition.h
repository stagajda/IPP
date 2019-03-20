#ifndef command_recognition_h
#define command_recognition_h
#define TYP_ERROR 0
#define TYP_DECLARE 1
#define TYP_REMOVE 2
#define TYP_VALID 3
#define TYP_ENERGY 4
#define TYP_EQUAL 5
#define TYP_EOF 6
#define TYP_IGNORE 7

int recognize_command(my_string *);

// 0 if not, 1 otherwise
int is_legal_quant_history(my_string *);

uint64_t is_legal_energy_number(my_string *);

#endif