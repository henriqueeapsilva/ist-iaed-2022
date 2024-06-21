# Project 1 Statement - IAED 2021/22

**Submission Date**: April 20, 2022, at 7:59 PM

**LOG of changes**

    April 4, 2022 - Project statement published.
    April 7, 2022 - Clarification of reservation code and command e.

## 1. Introduction

The goal of this project is to develop additional functionalities in C language to those created in the first project. Interaction with the program should occur through a set of lines composed of a letter (command) and a number of arguments depending on the command to be executed. You can assume that all input provided will comply with the indicated types, for example, where a decimal integer value is expected, a letter will never be entered. The commands from the first project are listed in the following table and maintain the operations to be performed and adhere to the imposed limits, except the upper limit of passengers per flight which is removed.
| Command |                   Action                   |
| :-----: | :----------------------------------------: |
|    q    |            terminates the program          |
|    a    |  adds a new airport to the system          |
|    l    |            lists the airports              |
|    v    |  adds a flight or lists all flights        |
|    p    | lists flights departing from an airport    |
|    c    | lists flights arriving at an airport       |
|    t    |         advances the system date           |

In addition to the commands from the first project, the following commands are added, along with the operations to be performed.
| Command |                        Action                        |
| :-----: | :--------------------------------------------------: |
|    r    | adds a reservation or lists the reservations of a flight |
|    e    |                deletes flights or reservations               |

## 2. Problem Specification

There are no limits on the number of reservations or the length of reservation codes, so you should aim to use memory strictly as necessary. To facilitate data entry, you can assume that each instruction will not exceed 65,535 characters. If the memory runs out, the program should terminate in a controlled manner, printing the message "No memory." Before terminating, the program should release all reserved memory.

## 3. Input Data

During program execution, instructions should be read from standard input in the form of a set of lines starting with a word, referred to as a command, followed by a number of information items depending on the command to be executed. Commands and arguments are separated by spaces or tabs.

Each command indicates a specific action, which is characterized in terms of input format, output format, and errors. In the case of multiple errors for the same command, only the first of these errors should be returned. The additional commands are:

    r - adds a reservation or lists the reservations of a flight:
        Input format: r <flightCode> <date> [<reservationCode> <numberOfPassengers>]
        Output format: <reservationCode> <numberOfPassengers> for each reservation on the flight with code <flightCode> on date <date>. One reservation per line in lexicographical order of the reservation code.
        Errors:
            invalid reservation code if the reservation code is not a string composed only of uppercase letters and digits or if it is less than 10 characters.
            <flightCode>: flight does not exist if there is no flight with the code on the indicated date.
            <reservationCode>: flight reservation already used if there is already a reservation with the indicated <reservationCode>.
            too many reservations if the reservation, when created, exceeds the flight's capacity.
            invalid date if the date is in the past or more than a year in the future.
            invalid passenger number if the number of passengers is not a positive integer.

    e - deletes flights or reservations:
        Input format: e <code>
        Output format: Deletes all flights or the reservation with the indicated <code>. If it is a flight code, all associated reservations should also be deleted.
        Errors:
            not found if the <code> does not exist.

## 4. Output Data

The program should write the responses to the commands presented in the standard input to the standard output. The responses are also lines of text formatted as previously defined in this statement. Pay attention to the number of spaces between elements in your output, as well as spaces at the end of each line. Try to strictly follow the given instructions.

The compiler to use is gcc with the following compilation options: -Wall -Wextra -Werror -ansi -pedantic. To compile the program, you should run the following command:

`$ gcc -Wall -Wextra -Werror -ansi -pedantic -o proj2 *.c`

This command should result in generating the executable file proj2 if there are no compilation errors. The execution of this command should not produce any output in the terminal. If the execution of this command produces any output in the terminal, it is considered that the program did not compile successfully. For example, during the compilation of the program, the compiler should not produce any warning messages.

You may only use the library functions defined in stdio.h, stdlib.h, and string.h.

## 5. Program Execution

The program should be executed as follows:

`$ ./proj2 < test.in > test.myout`

You can later compare your output (\*.myout) with the expected output (\*.out) using the diff command,

`$ diff test.out test.myout`

To test your program, you can execute the steps mentioned above or use the make command in the tests/ folder. To run all tests with valgrind, you can execute make valgrind in the tests/ folder.

## 6. Project Submission

The project submission should follow the procedure below:

    On the course page, access the system for project submissions. The system will be activated one week before the submission deadline. Instructions on how to access the system will be provided in due course.

    Upload a .zip archive file that includes all the source files that constitute the program.
    If your code has only one file, the zip will contain only that file.

    If your code is structured into multiple files (.c and .h), do not forget to include them in the package as well.

    To create a .zip archive file, run the following command in the directory where the .c and .h files are located (if applicable) that were created during the development of the project:

    $ zip proj2.zip *.c *.h

    As a result of the upload process, you will be informed if the submitted resolution presents the expected answer for a set of test cases.

    The system does not allow submissions with less than 10 minutes intervals for the same student. Pay special attention to this fact during the final submission.

    Project submission deadline: April 20, 2022, at 7:59 PM. Until the deadline, you can make as many submissions as you like, with the last submission being used for evaluation purposes. Therefore, make sure that the last submission corresponds to the version of the project you want to be evaluated. There will be no exceptions to this rule.

## 7. Project Evaluation

The project evaluation will consider the following components:

    The first component will be done automatically and evaluates the functionality performance of the completed program. This component is rated between 0 and 16 points.

    The second component evaluates the quality of the delivered code, specifically the following aspects: comments, indentation, dynamic memory allocation, structuring, modularity and division into files, data abstraction, among others. This component may vary between -4 and +4 points relative to the score calculated in the previous item and will be assigned subsequently. Some guidelines on this topic can be found here.

    In the second component, tools such as lizard, valgrind, and the fsanitize option will be used to detect code complexity, memory leaks, or other inaccuracies, which will be penalized. It is advised to use these tools to debug the code and correct any inaccuracies before submitting the project. Some debugging tips can be found here.

    The score for the first component of the project evaluation is obtained through the automatic execution of a set of tests on a computer with the GNU/Linux operating system. Therefore, it is essential that the code compiles correctly and that it respects the input and output data format described earlier. Projects that do not comply with the specified format will be penalized in the automatic evaluation, possibly scoring 0 (zero) if all tests fail. The tests considered for evaluation may include (or not) those made available on the course page, in addition to a set of additional tests. The execution of each program in each test is limited in the amount of memory it can use and the total time available for execution, with the time limit varying for each test.

    Note that the fact that a project successfully passes the set of tests provided on the course page does not imply that the project is entirely correct. It only indicates that it passed some tests successfully, but this set of tests is not exhaustive. It is the students' responsibility to ensure that the produced code is correct.
