#include<stdio.h>
#include<stdlib.h>
#define size 10

int res, jobs;
//int **allo, **max, **need, *avail, *work, *done, *sequence;
int allo[size][size], max[size][size], need[size][size];
int avail[size], work[size], done[size], sequence[size];

void input(){
	int i, j;
	printf("Enter the number of jobs: ");
	scanf("%d", &jobs);
	printf("Enter the number of resources: ");
	scanf("%d", &res);

//Use this for dynamic memory allocation
//Make appropriate changes above

/*	avail = (int*)malloc(jobs*sizeof(int));
	work = (int*)malloc(jobs*sizeof(int));
	done = (int*)malloc(jobs*sizeof(int));
	sequence = (int*)malloc(jobs*sizeof(int));
	allo = (int**)malloc(jobs*sizeof(*allo));
	max = (int**)malloc(jobs*sizeof(*max));
	need = (int**)malloc(jobs*sizeof(*max));

	for (i=0; i<jobs; i++){
		allo[i] = (int*)malloc(res*sizeof(int));
	}
	for (i=0; i<jobs; i++){
		max[i] = (int*)malloc(res*sizeof(int));
	}
	for (i=0; i<jobs; i++){
		need[i] = (int*)malloc(res*sizeof(int));
	}
*/

	printf("Enter the allocation matrix:\n");
	for (i=0; i<jobs; i++){
		printf("Job %d: ",i);
		for (j=0; j<res; j++)
			scanf("%d", &allo[i][j]);
	}
	printf("Enter the max matrix:\n");
	for (i=0; i<jobs; i++){
		printf("Job %d: ",i);
		for (j=0; j<res; j++)
			scanf("%d", &max[i][j]);
	}
	printf("Enter the available resources: ");
	for (j=0; j<res; j++)
		scanf("%d", &avail[j]);

	for (i=0; i<jobs; i++){
		done[i] = 0;				//None of the jobs are done
		work[i] = avail[i];
		for (j=0; j<res; j++){			//Calculate need matrix
			need[i][j] = max[i][j] - allo[i][j];
		}
	}
}

void output(){
	int i, j;
	printf("\nAllocated:\n");
	for (i=0; i<jobs; i++){
		printf("Job %d:\t",i);
		for (j=0; j<res; j++)
			printf("%d\t",allo[i][j]);
		printf("\n");
	}
	printf("\nMax:\n");
	for (i=0; i<jobs; i++){
		printf("Job %d:\t",i);
		for (j=0; j<res; j++)
			printf("%d\t",max[i][j]);
		printf("\n");
	}
	printf("\nNeed:\n");
	for (i=0; i<jobs; i++){
		printf("Job %d:\t",i);
		for (j=0; j<res; j++)
			printf("%d\t",need[i][j]);
		printf("\n");
	}
	printf("\nAvailable: ");
	for (i=0; i<res; i++)
		printf("%d\t", avail[i]);
	printf("\nWork: ");
	for (i=0; i<res; i++)
		printf("%d\t", work[i]);
	printf("\n");
}

int verify(){	//Check if input is correct
	int i, j;
	for (i=0; i<jobs; i++){
		if (avail[i] < 0) return 0;		//If available is negative
		for (j=0; j<res; j++){
			if (need[i][j] < 0) return 0;	//If allo > max
		}
	}
	return 1;
}

int notOver(){	//Check if all jobs are done
	int i;
	for (i=0; i<jobs; i++){
		if (done[i] == 0){			//If any one of the jobs is not done
			return 1;
		}
	}
	return 0;					//All done
}

int jobDoable(int jobId){	//Check if the job can be done
	if (done[jobId]) return 0;
	int j;
	for (j=0; j<res; j++){
		if (need[jobId][j] > work[j]){		//If need > work
			return 0;
		}
	}
	return 1;					//If need < work
}

void doJob(int jobId){
	int j;
	for (j=0; j<res; j++){
		work[j] += allo[jobId][j];		//Take back the resources that were allocated
	}
	done[jobId]=1;
}

void safety(){
	int i, j, s=0, flag;
	while (notOver()){	//Check if any job is left
		flag=1;		//Flag to check if the system has enterred deadlock state
		for (i=0; i<jobs; i++){
			if (jobDoable(i)){
				flag=0;	//There has been a change in the current iteration
				sequence[s++]=i;	//Update sequence
				doJob(i);
			}
		}
		if (flag) {	//No change in the current iteration => deadlock
			printf("\nNo safe sequence.\n");
			exit(0);
		}
	}
	//All jobs done
	printf("Safe sequence exists:\t");
	for (i=0; i<jobs; i++)
		printf("%d\t",sequence[i]);
	printf("\n");
}

void main(){
	int i, j;
	input();
	output();
	if (!verify()){
		printf("Invalid input.\n");
		exit(1);
	}
	safety();
}
