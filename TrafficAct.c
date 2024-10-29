#include <stdio.h>
#include <stdlib.h>

#define MAX 5

typedef enum {
    MainStraight = 0, DivStraight, MainLeft, DivLeft, MainRight, DivRight, MainPedestrian, DivPedestrian
} Lanes;

typedef struct {
    int time;
    Lanes lane;
} Traffic;

typedef struct {
    Traffic data[MAX];
    int lastNdx;
} PriorityQueue;

Traffic createTrafficList(int time, Lanes lane) {
    Traffic list;
    list.time = time;
    list.lane = lane;
    return list;
}

void display(Traffic* t) {
    printf("%-20s %-10s\n", "Time", "Lane");
    for (int x = 0; x < 5; x++) {
        printf("%-20d %-10d\n", t[x].time, t[x].lane);
    }
}

void enqueuePriorityQueue(PriorityQueue* pq, Traffic t) {
    if (pq->lastNdx < MAX - 1) {
        pq->data[++pq->lastNdx] = t;

        int childNdx = pq->lastNdx;
        int parentNdx = (childNdx - 1) / 2;

        while (childNdx != 0 && pq->data[childNdx].lane < pq->data[parentNdx].lane) {
            Traffic temp = pq->data[childNdx];
            pq->data[childNdx] = pq->data[parentNdx];
            pq->data[parentNdx] = temp;

            childNdx = parentNdx;
            parentNdx = (childNdx - 1) / 2;
        }
    } 
}

int totalTime(PriorityQueue pq) {
    int retval = 0;
	int x, index = 0;
    FILE* fp = fopen("Dequeue.dat", "wb");
    for ( x = 0; x <= pq.lastNdx; x++) {
        if (pq.data[index].lane != MainPedestrian && pq.data[index].lane != DivPedestrian) {
            retval += pq.data[index].time;
            if(fp!=NULL){
            fwrite(&pq.data[index], sizeof(Traffic),1,fp);
        }
        } 
        index = (index + 1) % MAX;
        
    }
    fclose(fp);
    return retval;
}

void displayQueue(PriorityQueue* pq) {
    int index = 0, x;
    printf("Priority Queue:\n");
    for (x = 0; x <= pq->lastNdx; x++) {
        printf("Lane: %d, Time: %d\n", pq->data[index].lane, pq->data[index].time);
        index = (index + 1) % MAX;
    }
}

int main() {
    Traffic t[5];
    PriorityQueue stuff = {.lastNdx = -1};

    t[0] = createTrafficList(5, MainPedestrian);
    t[1] = createTrafficList(9, MainRight);
    t[2] = createTrafficList(20, MainStraight);
    t[3] = createTrafficList(31, DivRight);
    t[4] = createTrafficList(60, DivStraight);

	FILE* fp = fopen("Traffic.dat", "wb");
	if(fp!=NULL){
		fwrite(&t, sizeof(Traffic), MAX, fp);
	}
	fclose(fp);

    printf("Traffic List:\n");
    display(t);

    for (int x = 0; x < MAX; x++) {
        enqueuePriorityQueue(&stuff, t[x]);
    }
    displayQueue(&stuff);

	int result = totalTime(stuff);
	printf("The calculated time is : %d", result);

	FILE* fpResults = fopen("traffic_result.dat", "wb");
	if(fp!=NULL){
		fprintf(fpResults,"The calculated time is : %d ", result);
	}
	fclose(fpResults);

}
