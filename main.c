//
//  main.c
//  infestPath
//
//  Created by Juyeop Kim on 2022/10/20.
//

#include <stdio.h>
#include <string.h>
#include "ifct_element.h"
#include "ifct_database.h"

#define MENU_PATIENT        1
#define MENU_PLACE          2
#define MENU_AGE            3
#define MENU_TRACK          4
#define MENU_EXIT           0

#define TIME_HIDE           2
#define file_name "patientInfo_sample.txt"
#define FILE_LENGTH         4

int trackInfester(int patient_no, int *detected_time, int *place);


int main(int argc, const char * argv[]) {
    
    int menu_selection;
    void *ifct_element;
    FILE* fp;
    int pIndex, age, time;
    int placeHist[N_HISTORY];
    
    //------------- 1. loading patient info file ------------------------------
    //1-1. FILE pointer open
    /* if (argc != 2)
    {
        printf("[ERROR] syntax : infestPath (file path).");
        return -1;
    } */
    /*
    fp = fopen(argv[1],"r");
    if (fp == NULL)
    {
        printf("[ERROR] Failed to open database file!! (%s)\n", argv[1]);
        return -1;
    }
    */
    
    //1-2. loading each patient informations
    fp = fopen(file_name, "r");// ���� �б���� ����
    if (fp == NULL) {
        printf("fail to read file");
    }
    int idx;
    for (idx = 0;idx<=FILE_LENGTH;idx++ ){
        int Index;
        int Age;
        unsigned int Date; 
        int history[5] = {};
        fscanf(fp, "%d %d %d %d %d %d %d %d",&Index, &Age, &Date, &history[0], &history[1], &history[2], &history[3], &history[4]);
    	ifct_element = ifctele_genElement(Index, Age, Date, history);
    	ifctdb_addTail(ifct_element);
    	ifctele_printElement(ifct_element);
    }
    	
    //1-3. FILE pointer close
    fclose(fp); 
    
    do {
        printf("\n=============== INFECTION PATH MANAGEMENT PROGRAM (No. of patients : %i) =============\n", ifctdb_len());
        printf("1. Print details about a patient.\n");                      //MENU_PATIENT
        printf("2. Print list of patients infected at a place.\n");        //MENU_PLACE
        printf("3. Print list of patients in a range of age.\n");          //MENU_AGE
        printf("4. Track the root of the infection\n");                     //MENU_TRACK
        printf("0. Exit.\n");                                               //MENU_EXIT
        printf("=============== ------------------------------------------------------- =============\n\n");
        
        printf("Select a menu :");
        scanf("%d", &menu_selection);
        fflush(stdin);
        switch(menu_selection)
        {
            case MENU_EXIT:;
                printf("Exiting the program... Bye bye.\n");
                break;
                
            case MENU_PATIENT:;
                // ������ ȯ��(��ȣ �Է�)�� ���� ���� ���
                // ��ȣ , ����, ����Ȯ������, �ֱ� 5�� �̵���Ҹ� ���
                
                
                int findNumber = -1;
                while( 0 > findNumber || findNumber > ifctdb_len()){
                    printf("ã���� �ϴ� ȯ���� ��ȣ�� �Է��ϼ���: ");
                    scanf("%d", &findNumber );
                }
                ifctele_printElement(ifctdb_getData(findNumber));
                break;
                
            case MENU_PLACE:;
                // ������ ��� (���ڿ� ����) ���� �ߺ�Ȯ���� �� ȯ�� ����� ���� ���
                
                int placeOfPatient;
                do {
                    printf("�����ϰ��� �ϴ� ������ �̸��� �Է��ϼ���: ");
                    char* Name;
                    scanf("%s", Name) ;
                    //���н� -1 ��ȯ�ϱ� ������ while ó�� ����. 
                    placeOfPatient = getPlaceIndex(Name);
                
                }
                while(placeOfPatient == -1);
                   
                
                //����� �� ���� �Է����� Place�� �������� ��. 
                printf("Finding Patients on Index %d", placeOfPatient);
                for( int MPi = 0; MPi < ifctdb_len(); MPi++){
                    for (int MPj = 0; MPj <= 4 ; MPj++){
                        if (placeOfPatient == ifctele_getHistPlaceIndex(ifctdb_getData(MPi), MPj)){
                             ifctele_printElement(ifctdb_getData(MPi));
                        }
                    }
                }
                
                break;
                
            case MENU_AGE:;
                int minAge, maxAge;
                printf("ã���� �ϴ� ������ ���̸� �Է��ϼ���. \n �ּ� : ");
                scanf("%d", &minAge );
                printf("  �ִ� : ");
                scanf("%d", &maxAge );
                printf("Finding Patients on Age %d �̻� %d ���� \n", minAge, maxAge);
                
                int found = 0;
                for( int MAi = 0; MAi < ifctdb_len(); MAi++){
                    int pAge = ifctele_getAge(ifctdb_getData(MAi));
                    if ( minAge <= pAge && pAge <= maxAge) {
                        ifctele_printElement(ifctdb_getData(MAi));
                        found++;
                    }
                }  
                if (found == 0) {
                    printf("Nothing Found! ");
                }
                // �ּҿ� �ִ� ���� �Է� �޾�, ���� �� ���� ȯ���� ����� ���� ���
                break;
                
            case MENU_TRACK:;
                // ������ ȯ�ڸ� �������� �����ڿ� �������� ���� �� �̵���Ҹ�
                //���������� ����ϰ� ���� �����ڸ� ���� ����ϰ�
                int checknum;
                int smallestBefore;
                int MTCount = 0;
                printf("ã���� �ϴ� ȯ���� ��ȣ�� �Է��ϼ���: ");
                scanf("%d", &checknum );
                if (checknum <= ifctdb_len()) {
                    
                    while(MTCount == 0){
                        printf("�����ϴ� ����� index�� %d �Դϴ�. \n", checknum);
                        void* patientData = ifctdb_getData(checknum);
                        //���ϰ��� �ϴ� ����� ���� ��ҵ�
                        int places[5];
                        for(int MTi =0; MTi< 5; MTi++){
                            places[MTi] = ifctele_getHistPlaceIndex(patientData, MTi);
                        }
                        int detTime = ifctele_getinfestedTime(patientData);
                        //���ϰ��� �ϴ� ����� ���� �ð���
                        int times[5] = { detTime-4, detTime-3, detTime-2,detTime-1, detTime };
                        
                        smallestBefore = trackInfester(checknum, times, places);
                        if (checknum != smallestBefore){
                            checknum = smallestBefore;
                            printf("������� ���� �����ڴ� %d �Դϴ� \n\n", smallestBefore);
                        } else {
                            MTCount = 1;
                            printf("���� ���� �����ڴ� %d �Դϴ� \n\n", smallestBefore);
                        }
                       
                    }
                }
                
                break;
                
            default:;
                printf("[ERROR Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
                break;
        }
    
    } while(menu_selection != 0);
    
    
    return 0;
}

//ȯ�� ������ ������ ������ ��ġ�� ȯ�ڵ� �߿��� ���� detected_time�� ���� ȯ�ڸ� ��ȯ
int trackInfester(int patient_no, int *detected_time, int *place){
    //�־��� ȯ���� ����Ȯ�����ں��� �� ������ ȯ�ڸ� ��ȸ
    ifctele_printElement(ifctdb_getData(patient_no));
    
    int smaller = 100;
    int smallerIndex = -1;
    for(int tii=0; tii < ifctdb_len(); tii++){
        void* old = ifctdb_getData(tii);
        if (ifctele_getinfestedTime(old) < detected_time[4]) {
            printf("Ȯ�������� �� ���� %d ��° index�� ȯ�ڸ� �����ϴ���.... \n", tii);
            //ifctele_printElement(ifctdb_getData(tii));
            for (int oldPlace = 0; oldPlace<5;oldPlace++){
                for (int givenPlace = 0; givenPlace < 5; givenPlace++){
                    if (place[givenPlace] == ifctele_getHistPlaceIndex(old, oldPlace)) {
                        //printf("%d �� %d�� %d ���� ������ ���ɼ��� �ֽ��ϴ�. \n", patient_no, tii, place[givenPlace]);
                        //printf("%d�� %d�� %d ������ �����߽��ϴ�. \n", patient_no, place[givenPlace], detected_time[givenPlace]);
                        //printf("%d�� %d�� %d ������ �����߽��ϴ�. \n", tii, place[givenPlace], ifctele_getinfestedTime(old) -4 + oldPlace);
                        if (detected_time[givenPlace] == (ifctele_getinfestedTime(old)-4 + oldPlace)){
                            printf("%d�� ȯ�ڿ� %d�� ȯ�ڰ� %d ���� %d �������������ϴ�. \n", patient_no, tii, place[givenPlace],detected_time[givenPlace] );
                            if (smaller > ifctele_getinfestedTime(old)) {
                                printf("%d�� ���������� �� ������ %d ���� �̸��ϴ�. �����մϴ�.\n\n ", tii, smallerIndex);
                                smaller = ifctele_getinfestedTime(old);
                                smallerIndex = tii;
                            }
                        }
                    }
                }
            }
            
        }
    }
    if (smallerIndex == -1) {
        return patient_no;
    }
    return smallerIndex;
}


