#include<stdio.h>
#include<stdlib.h>
#define size 10

int n, endtime=0, done=0;

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
	int *timeline, i, j, minId, minTime;
	float avgWt=0.0, avgTa=0.0;
	timeline=(int *)malloc((endtime+1)*2);
	for (j=0; j<=endtime; j++){
		minTime = endtime;
		minId = -1;
		for (i=0; i<n; i++){
			if (active[i] == 1 && j >= arrival[i] && minTime >= remaining[i]){
				minTime = remaining[i];
				minId = i;
			}
		}
		if (minId == -1){
			done = 1;
			for (i=0; i<n; i++){
				if (active[i]){
					done = 0;
					break;
				}
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
			else{				//Nothing is currently available
				timeline[j] = -1;
			}
		}
		else{
			timeline[j]=minId;
			remaining[minId]-=1;
			if (remaining[minId] == 0) active[minId]=0; //Job done
			for (i=0; i<n; i++){
				if (active[i] && j >= arrival[i] && i != minId){
					wt[i] += 1;
				}
			}
		}
	}
	return 1;
}

int main(){
	int active[size], arrival[size], burst[size], remaining[size], wt[size]={0}, ta[size]={0};
	printf("Shortest remaining time first\n\n");
	input(active, arrival, burst, remaining);
	output(active, arrival, burst, remaining);
	if (verify(active, arrival, burst, remaining)){
		printf("Error in input.\n");
		return 1;
	}
	sequence(active, arrival, burst, remaining, wt);
	return 0;
}
