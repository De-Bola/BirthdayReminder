/**
 * File:        HW_I_Adebola_Gbiri_174662MVEB.c
 * Author:      Adebola Gbiri
 * Created      10.10.2018
 * Edited:      13.12.2019
 *
 * Description: Update on my solution for homework I(variant C08)
 * Program is meant to display date of birth of employees
 * who will have their birthday within the next 14 days
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAXIN 32
#define UNIQUEMONTHS 4
#define INPUTDATA "C:\\Users\\admin\\CLionProjects\\hw_1\\inputData.txt"

typedef struct birthdate
{
    int day;
    int month;
    int year;
}Date;

typedef struct User
{
    char name[MAXIN];
    Date birth;
}Employee;

Date getUserInput(Date userInput);

void getInputData(Employee employeeData, Date userInput);
void analyzeData(FILE *readPointer, Employee employeeData, Date userInput, FILE *writePointer);
void printOutput(Employee employeeData, FILE *writePointer);
void printNoOutput(FILE *writePointer);
void printStats(int birthdayCount, int employeeCount, FILE *writePointer);
void printEmployee(Employee employeeData);

bool check(Date employeeDate, Date userInput);
bool checkLeapYear(Date year);
bool checkIfMonthHasThirtyDays(Date employeeDate);
bool validateInput(Date userInput);
bool checkRangeSum(int rangeSum);
bool checkForLapses(Date employeeDate, Date userDate);

int getTotalRange(int userMonth, int employeeDay, int userDay);

int main(void)
{
    Employee employeeData;
    Date userInput;

    userInput = getUserInput(userInput);
    getInputData(employeeData,userInput);


    return 0;
}

void getInputData(Employee employeeData, Date userInput)//gets input data from file
{
    FILE *readPointer = fopen(INPUTDATA ,"r");
    FILE *writePointer =fopen("Output.txt","w");

    if (readPointer == NULL)
    {
        printf("read pointer is null");
        perror("error opening Datafile");
        fclose(readPointer);
        exit;
    }

    if (writePointer == NULL)
    {
        perror("error creating Output file");
        fclose(writePointer);
        exit;
    }

    analyzeData(readPointer, employeeData, userInput, writePointer);

    fclose(readPointer);
    fclose(writePointer);
}

Date getUserInput(Date userInput)//reads user input from keyboard
{
    printf("Enter the date (format dd.mm.yyyy:) \n");
    scanf("%2d.%2d.%4d",&userInput.day,&userInput.month,&userInput.year);

    while(validateInput(userInput) == true)//validates userInput
    {
        printf("your input has the wrong format. Please try again!!\n");
        printf("Correct format is : dd.mm.yyyy \n");
        scanf("%2d.%2d.%4d", &userInput.day, &userInput.month, &userInput.year);
    }

    printf("your input was : %d.%d.%d \n",userInput.day,userInput.month,userInput.year);
    return userInput;
}


void analyzeData(FILE *readPointer, Employee employee, Date userInput, FILE *writePointer)//reads input file and check for dates that meets our search criteria
{
    int i;
    int counter = 0;
    bool isMatch = false;

    i =0;

    printf("---------------Employees with birthday within the next 14 days---------------\n");
    fprintf(writePointer,"---------------Employees with birthday within the next 14 days---------------\n");

    while (fscanf(readPointer,"%s %2d %2d %4d", employee.name, &employee.birth.day, &employee.birth.month, &employee.birth.year)==4)
    {
        if(validateInput(employee.birth)) {
            continue;//skips invalid dates in the inputFile
        }
        //printEmployee(employee);
        isMatch = check(employee.birth, userInput);
        if(isMatch == true)
        {
            printOutput(employee, writePointer);
            counter++;
        }
        i++;
    }

    if(counter == 0)
    {
        printNoOutput(writePointer);
    }

    printStats(counter, i, writePointer);
}

bool check(Date employeeDate, Date userDate) //check for dates that meets our search criteria
{
    int monthDifference, dayDifference, rangeSum;
    int daysOfTheMonth[UNIQUEMONTHS] = {31,30,29,28};
    bool isWithinOurRange = false;

    if(employeeDate.month == userDate.month){

        if(userDate.month == 2){
            if ((checkLeapYear(employeeDate) == true) && (checkLeapYear(userDate) == false)){
                isWithinOurRange = false;
                return isWithinOurRange;
            }
        }

        dayDifference = employeeDate.day - userDate.day;//because we are checking for upcoming birthdays

        if ((dayDifference < 15) && (dayDifference >= 0))
        {
            isWithinOurRange = true;
            return isWithinOurRange;
        }
    }

    monthDifference = employeeDate.month - userDate.month;

    if (monthDifference == 1) {

        if(checkForLapses(employeeDate, userDate) == true){

            if(userDate.month == 2){

                if (checkLeapYear(employeeDate) == true) {

                    if (checkLeapYear(userDate) == false){

                        isWithinOurRange = false;
                        return isWithinOurRange;
                    } else {

                        rangeSum = getTotalRange(daysOfTheMonth[2], employeeDate.day, userDate.day);
                        if(checkRangeSum(rangeSum) == true)
                        {
                            isWithinOurRange = true;
                            return isWithinOurRange;
                        }
                    }

                }

                if(employeeDate.day > 28){
                    isWithinOurRange = false;
                    return isWithinOurRange;
                }
            }

            if(checkIfMonthHasThirtyDays(userDate) == false){
                rangeSum = getTotalRange(daysOfTheMonth[0], employeeDate.day, userDate.day);
                if(checkRangeSum(rangeSum) == true)
                {
                    isWithinOurRange = true;
                    return isWithinOurRange;
                }

            }

            rangeSum = getTotalRange(daysOfTheMonth[1], employeeDate.day, userDate.day);
            if(checkRangeSum(rangeSum) == true)
            {
                isWithinOurRange = true;
                return isWithinOurRange;
            }

        }

    }

    return isWithinOurRange;
}

void printOutput(Employee employeeData, FILE *writePointer)
{
    printf("\tCongrats! %s, happy birthday in advance and many happy returns.\n", employeeData.name);
    printf("\t%s's birthday is\t%d.%d.%d\n", employeeData.name, employeeData.birth.day, employeeData.birth.month, employeeData.birth.year);
    printf("-----------------------------------------------------------------------------\n");

    fprintf(writePointer,"\tCongrats!! %s\n", employeeData.name);
    fprintf(writePointer,"\t%s\t%d.%d.%d\n", employeeData.name, employeeData.birth.day, employeeData.birth.month, employeeData.birth.year);
    fprintf(writePointer,"-----------------------------------------------------------------------------\n");

//fclose(writePointer);
}

void printNoOutput(FILE *writePointer)
{
    printf("------------------------------\n");
    printf("No employees have birthdays in the next 14 days\n");
    printf("------------------------------\n");

    fprintf(writePointer,"------------------------------\n");
    fprintf(writePointer,"No employees have birthdays in the next 14 days\n");
    fprintf(writePointer,"------------------------------\n");

//fclose(writePointer);
}

void printStats(int birthdayCount, int employeeCount, FILE *writePointer){
    printf("Out of %d employees, only %d have birthday in the next 14 days.\n", employeeCount, birthdayCount);
    fprintf(writePointer,"Out of %d employees, only %d have birthday in the next 14 days.\n", employeeCount, birthdayCount);
}

bool checkLeapYear(Date userInput){//confirms if a year is leap

    if(((userInput.year % 4 == 0) && (userInput.year % 100 != 0)) || (userInput.year % 400 == 0)){
        return true;
    }
    return false;
}

bool checkIfMonthHasThirtyDays(Date employeeDate){//to confirm if a month has 30 days
    if((employeeDate.month == 4) || (employeeDate.month == 6) || (employeeDate.month == 9) || (employeeDate.month == 11) || (employeeDate.month != 2)){
        return true;
    }
    return false;
}

bool validateInput(Date userInput){//check validity of a given date
    if(((userInput.day < 1) || (userInput.day > 31)) || ((userInput.month < 1) || (userInput.month > 12)))
    {
        return true;
    }

    if(checkIfMonthHasThirtyDays(userInput) == true)
    {
        if(userInput.day > 30)
        {
            return true;
        }
    }

    if(checkLeapYear(userInput) == true)
    {
        if(userInput.month == 2 && userInput.day > 29)
        {
            return true;
        }
    }

    if(checkLeapYear(userInput) == false)
    {
        if(userInput.month == 2 && userInput.day > 28)
        {
            return true;
        }
    }

    return false;
}

int getTotalRange(int userMonth, int employeeDay, int userDay){//calculates the difference in days with respect to the month
    int rangeSum;
    rangeSum = userMonth - userDay;
    rangeSum = (rangeSum > 0) ? rangeSum + employeeDay : employeeDay;
    return rangeSum;
}

bool checkRangeSum(int rangeSum){
    if ((rangeSum < 15) && (rangeSum > 0)) {
        return true;
    }
    return false;
}

bool checkForLapses(Date employeeDate, Date userDate){//checks if input date is in second half of the month
    if(checkIfMonthHasThirtyDays(employeeDate) == false){
        if((employeeDate.day < 15) && (userDate.day > 16)){
            return true;
        }
    }
    if ((employeeDate.day < 15) && (userDate.day > 15)){
        return true;
    }
    return false;
}

void printEmployee(Employee employeeData){
    printf("\t%s's birthday is\t%d.%d.%d\n", employeeData.name, employeeData.birth.day, employeeData.birth.month, employeeData.birth.year);
}