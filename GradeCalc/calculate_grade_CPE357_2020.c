

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct item
	{
	int n;//accomplished so far
	float points;
	}item;
int main()
	{
	item labs, programs, quizzes, midterm, final;
	printf("\nCalculate current grade CPE 357 / winter 2020\n");
	printf("\nCheck your points on PolyLearn for this calculation\n");
	printf("\nThere are no error checks here, so be careful to enter the correct info\n");
	printf("-----------------------------------------------\n\n");
	printf("Enter the number of labs so far: "); scanf("%d", &labs.n);
	labs.points = 0;
	for (int i = 0; i < labs.n; i++)
		{
		int p;
		printf("Points on lab %d: ", i + 1); scanf("%d", &p);
		labs.points += p;
		}
	printf("Enter the number of programs so far: "); scanf("%d", &programs.n);
	programs.points = 0;
	for (int i = 0; i < programs.n; i++)
		{
		int p;
		printf("Points on program %d: ", i + 1); scanf("%d", &p);
		programs.points += p;
		}
	printf("Enter the number of quizzes so far: "); scanf("%d", &quizzes.n);
	quizzes.points = 0;
	for (int i = 0; i < quizzes.n; i++)
		{
		float p;
		printf("Points on quiz %d: ", i + 1); scanf("%f", &p);
		if (i == 0) p *= 2.0; //quiz 1 was set to 5 points instead of 10
		quizzes.points += p;
		}
	printf("Enter if the midterm already happened (0 no, 1 yes): "); scanf("%d", &midterm.n);
	if (midterm.n) { printf("Points on midterm: "); scanf("%f", &midterm.points); }
	printf("Enter if the final exam already happened (0 no, 1 yes): "); scanf("%d", &final.n);
	if (final.n)	{printf("Points on final: "); scanf("%f", &final.points);}

	double ratio_labs = ((double)labs.points / (double)labs.n) * 0.01;
	double ratio_prog = ((double)programs.points / (double)programs.n) * 0.01;
	double ratio_quizzes = ((double)quizzes.points / (double)quizzes.n) *0.1;
	double ratio_midterm = (double)midterm.points * 0.01;
	double ratio_final = (double)final.points * 0.01;
	
	double momentary_grade = 0;					//Where are you right now?
	double total_complete = 0;					//Where are you right now in sum?
	double total_potential_complete = 0;		//What could have been achieved so far?
	int items_present = 0;
	if (labs.n)		items_present++;
	if (quizzes.n)	items_present++;
	if (programs.n)	items_present++;	
	if (midterm.n)	items_present++;
	if (final.n)	items_present++;

	//															BASED ON THE SYLLABUS!

	switch (items_present)
		{
		case 0:
		case 1: printf("\nnot enough data present\n"); return 0;
		case 2: total_complete = ratio_labs * 0.2 + ratio_quizzes * 0.1; momentary_grade = total_complete * 3.333333; total_potential_complete = 0.3; break;
		case 3: total_complete = ratio_labs * 0.2 + ratio_quizzes * 0.1 + ratio_prog * 0.3; momentary_grade = total_complete * 1.666666; total_potential_complete = 0.6; break;
		case 4: total_complete = ratio_labs * 0.2 + ratio_quizzes * 0.1 + ratio_prog * 0.3 + ratio_midterm * 0.2; momentary_grade = total_complete * 1.25; total_potential_complete = 0.8; break;
		case 5: momentary_grade = total_complete = ratio_labs * 0.2 + ratio_quizzes * 0.1 + ratio_prog * 0.3 + ratio_midterm * 0.2 + ratio_final * 0.2; total_potential_complete = 1.0; break;
		}

	printf("\n\n-----------------------------------------------\n\n");
	printf("your momentary grade in %%: %.1f\n", momentary_grade * 100.);
	printf("your completion is %.1f %% out of potential %.1f %%\n", total_complete * 100., total_potential_complete * 100.);
	
	char lettergrade[3];
	if (momentary_grade >= 0.95)		strcpy(lettergrade, "A");
	else if (momentary_grade >= 0.91)	strcpy(lettergrade, "A-");
	else if (momentary_grade >= 0.87)	strcpy(lettergrade, "B+");
	else if (momentary_grade >= 0.83)	strcpy(lettergrade, "B");
	else if (momentary_grade >= 0.8)	strcpy(lettergrade, "B-");
	else if (momentary_grade >= 0.76)	strcpy(lettergrade, "C+");
	else if (momentary_grade >= 0.70)	strcpy(lettergrade, "C");
	else if (momentary_grade >= 0.60)	strcpy(lettergrade, "D");
	else 								strcpy(lettergrade, "F");

	printf("your momentary letter grade is %s", lettergrade);
	printf("\n\n-----------------------------------------------\n\n");
	return 0;
	}



