#ifndef C_PROJECT_OPENU_HANDLE_ERRORS_H
#define C_PROJECT_OPENU_HANDLE_ERRORS_H

/**
 * This function prints an error message related to a specific file and line number.
 * @param file_name Pointer to the null-terminated string containing the name of the file where the error occurred.
 * @param line      The line number in the file where the error was detected.
 * @param error_msg Pointer to the null-terminated string containing the error message to be displayed.
 */
void input_errors(char file_name[], int line, char error_msg[]);

/**
 * @brief Prints a general error message.
 *
 * This function prints a general error message.
 * @param error_msg Pointer to the null-terminated string containing the error message to be displayed.
 */
void general_errors(char error_msg[]);

#endif
