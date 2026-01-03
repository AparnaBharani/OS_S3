#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <direct.h>
    #define CREATE_FOLDER(name) _mkdir(name)
    #define REMOVE_FILE(name) remove(name)
#else
    #include <sys/stat.h>
    #define CREATE_FOLDER(name) mkdir(name, 0777)
    #define REMOVE_FILE(name) remove(name)
#endif

#define MAX 100
char projectFolder[100] = "MyProject";

// ================= CPU PROCESS STRUCT =================
typedef struct {
    int pid, bt, at, priority;
    char action[50];
} Process;

Process processes[100];
int processCount = 0;  // number of actions (processes)

void addProcess(const char *actionName, int burstTime, int priority) {
    processes[processCount].pid = processCount + 1;
    processes[processCount].bt = burstTime;
    processes[processCount].at = processCount; // arrival order
    processes[processCount].priority = priority;
    strcpy(processes[processCount].action, actionName);
    processCount++;
}

// ================= FILE OPERATIONS =================
void createProjectFolder(const char *folderName) {
    int result = CREATE_FOLDER(folderName);
    if (result == 0)
        printf("✅ Project folder '%s' created successfully.\n", folderName);
    else {
        if (errno == EEXIST)
            printf("⚠️ Folder '%s' already exists. Using it.\n", folderName);
        else
            perror("❌ Error creating folder");
    }
    addProcess("Create Folder", 2, 5);
}

void addFileToProject(const char *fileName) {
    char path[200];
    sprintf(path, "%s/%s.txt", projectFolder, fileName);

    FILE *fp = fopen(path, "w");
    if (fp == NULL) {
        perror("❌ Error creating file");
        return;
    }
    fclose(fp);
    printf("✅ File '%s' created inside '%s'.\n", fileName, projectFolder);
    addProcess("Add File", 1, 4);
}

void writeContentToFile(const char *fileName) {
    char path[200];
    sprintf(path, "%s/%s.txt", projectFolder, fileName);

    FILE *fp = fopen(path, "a");
    if (fp == NULL) {
        perror("❌ Error opening file");
        return;
    }
    printf("Enter content (end with ~ on a new line):\n");
    char ch;
    int count = 0;
    while ((ch = getchar()) != '~') {
        fputc(ch, fp);
        count++;
    }
    fclose(fp);
    printf("✅ Content written successfully.\n");

    int burstTime = count / 5 + 1;
    addProcess("Write Content", burstTime, 3);
}

void readContentFromFile(const char *fileName) {
    char path[200];
    sprintf(path, "%s/%s.txt", projectFolder, fileName);

    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        perror("❌ Error reading file");
        return;
    }
    printf("📄 Content of '%s':\n", fileName);
    char ch;
    int count = 0;
    while ((ch = fgetc(fp)) != EOF) {
        putchar(ch);
        count++;
    }
    fclose(fp);
    printf("\n");

    int burstTime = count / 5 + 1;
    addProcess("Read Content", burstTime, 2);
}

void renameFile(const char *oldName, const char *newName) {
    char oldPath[200], newPath[200];
    sprintf(oldPath, "%s/%s.txt", projectFolder, oldName);
    sprintf(newPath, "%s/%s.txt", projectFolder, newName);

    if (rename(oldPath, newPath) == 0)
        printf("✅ File renamed from '%s' to '%s'.\n", oldName, newName);
    else
        perror("❌ Error renaming file");

    addProcess("Rename File", 1, 3);
}

void deleteFile(const char *fileName) {
    char path[200];
    sprintf(path, "%s/%s.txt", projectFolder, fileName);

    if (REMOVE_FILE(path) == 0)
        printf("✅ File '%s' deleted.\n", fileName);
    else
        perror("❌ Error deleting file");

    addProcess("Delete File", 1, 2);
}

void searchFile(const char *fileName) {
    char path[200];
    sprintf(path, "%s/%s.txt", projectFolder, fileName);

    FILE *fp = fopen(path, "r");
    if (fp) {
        printf("🔍 File '%s' exists in the project.\n", fileName);
        fclose(fp);
    } else {
        printf("⚠️ File '%s' not found.\n", fileName);
    }
    addProcess("Search File", 1, 1);
}

void backupProject() {
    char backupFolder[150];
    sprintf(backupFolder, "%s_backup", projectFolder);

    int result = CREATE_FOLDER(backupFolder);
    if (result == 0)
        printf("✅ Backup folder '%s' created.\n", backupFolder);
    else if (errno == EEXIST)
        printf("⚠️ Backup folder '%s' already exists.\n", backupFolder);
    else
        perror("❌ Error creating backup folder");

    addProcess("Backup Project", 3, 4);
}

// ================= Improved Gantt Chart =================
void ganttChart(int gantt[], int n, int bt[], int ct[]) {
    printf("\n📊 Gantt Chart:\n\n");

    // Top border
    printf(" ");
    for (int i = 0; i < n; i++) {
        printf("------");
    }
    printf("-\n|");

    // Process IDs inside boxes
    for (int i = 0; i < n; i++) {
        printf(" P%-3d |", gantt[i]);
    }

    // Bottom border
    printf("\n ");
    for (int i = 0; i < n; i++) {
        printf("------");
    }
    printf("-\n");

    // Timeline values
    printf("0");
    for (int i = 0; i < n; i++) {
        printf("%6d", ct[i]);
    }
    printf("\n");
}

// ================= CPU SCHEDULING =================
void fcfs(int n, Process p[]) {
    int wt[n], tat[n], ct[n], gantt[n], bt[n];
    int time = 0;

    for (int i = 0; i < n; i++) {
        if (time < p[i].at) time = p[i].at;
        time += p[i].bt;
        ct[i] = time;
        tat[i] = ct[i] - p[i].at;
        wt[i] = tat[i] - p[i].bt;
        gantt[i] = p[i].pid;
        bt[i] = p[i].bt;
    }

    printf("\nFCFS Scheduling:\n");
    printf("PID\tAction\t\tBT\tAT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++)
        printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].action, p[i].bt, p[i].at, ct[i], tat[i], wt[i]);

    ganttChart(gantt, n, bt, ct);
}

void roundRobin(int n, Process p[], int quantum) {
    int rem[n], ct[n], tat[n], wt[n], bt[n];
    int gantt[100], gIndex = 0;
    for (int i = 0; i < n; i++) { 
        rem[i] = p[i].bt; 
        bt[i] = p[i].bt;
    }

    int time = 0, done;
    do {
        done = 1;
        for (int i = 0; i < n; i++) {
            if (rem[i] > 0 && p[i].at <= time) {
                done = 0;
                gantt[gIndex] = p[i].pid;

                if (rem[i] > quantum) {
                    time += quantum;
                    rem[i] -= quantum;
                    ct[i] = time;
                } else {
                    time += rem[i];
                    ct[i] = time;
                    tat[i] = ct[i] - p[i].at;
                    wt[i] = tat[i] - p[i].bt;
                    rem[i] = 0;
                }
                gIndex++;
            }
        }
    } while (!done);

    printf("\nRound Robin (q=%d):\n", quantum);
    printf("PID\tAction\t\tBT\tAT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++)
        printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].action, p[i].bt, p[i].at, ct[i], tat[i], wt[i]);

    ganttChart(gantt, gIndex, bt, ct);
}

void priorityScheduling(int n, Process p[]) {
    int ct[n], tat[n], wt[n], gantt[n], bt[n];
    int completed = 0, time = 0;
    int isDone[n];
    for(int i=0;i<n;i++){ isDone[i]=0; bt[i]=p[i].bt; }

    while(completed < n) {
        int idx = -1;
        int highest = -1;
        for(int i=0;i<n;i++) {
            if(!isDone[i] && p[i].at <= time && p[i].priority > highest) {
                highest = p[i].priority;
                idx = i;
            }
        }

        if(idx == -1) { time++; continue; }

        time += p[idx].bt;
        ct[idx] = time;
        tat[idx] = ct[idx] - p[idx].at;
        wt[idx] = tat[idx] - p[idx].bt;
        gantt[completed] = p[idx].pid;
        completed++;
        isDone[idx] = 1;
    }

    printf("\nPriority Scheduling (Non-preemptive):\n");
    printf("PID\tAction\tBT\tAT\tPriority\tCT\tTAT\tWT\n");
    for(int i=0;i<n;i++)
        printf("%d\t%s\t%d\t%d\t%d\t\t%d\t%d\t%d\n",
               p[i].pid, p[i].action, p[i].bt, p[i].at, p[i].priority, ct[i], tat[i], wt[i]);

    ganttChart(gantt, n, bt, ct);
}

// ================= MAIN MENU =================
int main() {
    int choice;
    char name[50], newName[50];

    while (1) {
        printf("\n==============================\n");
        printf("📌 PROJECT MANAGER SYSTEM (C)\n");
        printf("==============================\n");
        printf("1. Create New Project Folder\n");
        printf("2. Add File to Project\n");
        printf("3. Write Content to File\n");
        printf("4. Read Content from File\n");
        printf("5. Rename File\n");
        printf("6. Delete File\n");
        printf("7. Search File\n");
        printf("8. Backup Project\n");
        printf("9. Show CPU Scheduling of Actions\n");
        printf("10. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // clear buffer

        switch (choice) {
            case 1:
                printf("Enter folder name: ");
                scanf("%s", projectFolder);
                createProjectFolder(projectFolder);
                break;
            case 2:
                printf("Enter file name: ");
                scanf("%s", name);
                addFileToProject(name);
                break;
            case 3:
                printf("Enter file name: ");
                scanf("%s", name);
                getchar();
                writeContentToFile(name);
                break;
            case 4:
                printf("Enter file name: ");
                scanf("%s", name);
                readContentFromFile(name);
                break;
            case 5:
                printf("Enter old file name: ");
                scanf("%s", name);
                printf("Enter new file name: ");
                scanf("%s", newName);
                renameFile(name, newName);
                break;
            case 6:
                printf("Enter file name to delete: ");
                scanf("%s", name);
                deleteFile(name);
                break;
            case 7:
                printf("Enter file name to search: ");
                scanf("%s", name);
                searchFile(name);
                break;
            case 8:
                backupProject();
                break;
            case 9:
                if (processCount == 0) { printf("⚠️ No actions recorded yet.\n"); break; }
                fcfs(processCount, processes);
                int quantum;
                printf("\nEnter time quantum for RR: ");
                scanf("%d", &quantum);
                roundRobin(processCount, processes, quantum);
                priorityScheduling(processCount, processes);
                break;
            case 10:
                exit(0);
        }
    }
}
