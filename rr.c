#include<stdio.h>
#include<stdlib.h>
#define size 10

int n, tq, endtime=0, done=0;

void input(int active[], int arrival[], int burst[], int remaining[]){
	int i;
	printf("Enter the number of jobs: ");
	scanf("%d", &n);
	printf("Enter job details, in the order of arrival\n");
	for (i=0; i<n; i++){
		printf("Job %d- Arrival, Burst: ", i);
		scanf("%d %d", &arrival[i], &burst[i]);
		active[i] = 1;
		remaining[i] = burst[i];
	}
	printf("Enter time quantum: ");
	scanf("%d", &tq);
}

void output(int active[], int arrival[], int burst[], int remaining[]){
	int i;
	printf("\nJobId\tActive\tArrival\tRemaining\n");
	for (i=0; i<n; i++){
		printf("%d\t%d\t%d\t%d\n", i, active[i], arrival[i], remaining[i]);
	}
}

int verify(int active[], int arrival[], int burst[], int remaining[]){
	int i;
	for (i=0; i<n; i++){
		if (burst[i] <= 0) return 1;			//If burst time is not a natural number
		endtime+=burst[i];
	}
	endtime+=arrival[n-1];
	/* Worst case: all process start at time n,
	 * so the last time period in the Gantt chart
	 * will be n + sum(burst)
	 */
	for (i=0; i<n-1; i++){
		if (arrival[i]>arrival[i+1]) return 1;		//If arrival times aren't ordered
	}
	return 0;
}

int sequence(int active[], int arrival[], int burst[], int remaining[], int wt[]){
	int *timeline, i, j=0, temp, minId, minTime, added;
	float avgWt=0.0, avgTa=0.0;
	timeline=(int *)malloc((endtime+1)*2);
	while (j<=endtime){
		added=0;				//Flag to check if any job is available for that second
		for (i=0; i<n; i++){
			if (active[i] && j >= arrival[i]){
				added=1;
				if (remaining[i] >= tq){
					for (temp=0; temp<n; temp++){	//Waiting time increases for all other jobs
						if (temp != i && active[temp] && j >= arrival[temp]){
							wt[temp] += tq;
						}
					}
					for (temp=0; temp<tq; temp++){	//Add current job to Gantt chart
						timeline[j]=i;
						j++;
					}
					remaining[i]-=tq;
				}
				else{			//If remaining time is less than time quantum
					for (temp=0; temp<n; temp++){
						if (temp != i && active[temp] && j >= arrival[temp]){
							wt[temp] += remaining[i];
						}
					}
					for (temp=0; temp<remaining[i]; temp++){
						timeline[j]=i;
						j++;
					}
					remaining[i] = 0;
				}
				if (remaining[i] == 0){	//If job is over
					active[i] = 0;
				}
			}
		}
		if(!added) timeline[j++]=-1;		//If no job is available for that second
		done = 1;
		for (i=0; i<n; i++){
			if (active[i]) done = 0;
		}
		if (done) {			//Check if all are done
			printf("\nGantt Chart:\n");
			for ( i=0; i<j; i++){
				printf("%d    ", timeline[i]);
			}
			printf("\n\nJobId\tWating\tTurn around\n");
			for ( i=0; i<n; i++){
				printf("%d\t%d\t%d\n", i, wt[i], wt[i] + burst[i]);
				avgWt += wt[i];
				avgTa += wt[i] + burst[i];
			}
			printf("\n\nAvg WT: %.2f\tAvg TA: %.2f\n", avgWt/n, avgTa/n);
			printf("All jobs done.\n\n");
			return 0;
		}
	}
}
	

int main(){
	int active[size], arrival[size], burst[size], remaining[size], wt[size]={0}, ta[size]={0};
	printf("Round robin\n\n");
	input(active, arrival, burst, remaining);
	output(active, arrival, burst, remaining);
	if (verify(active, arrival, burst, remaining)){
		printf("Error in input.\n");
		return 1;
	}
	sequence(active, arrival, burst, remaining, wt);
	return 0;
}
