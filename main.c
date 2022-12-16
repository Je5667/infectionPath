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
    fp = fopen(file_name, "r");// 파일 읽기모드로 열기
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
                // 지정된 환자(번호 입력)에 대한 정보 출력
                // 번호 , 나이, 감염확인일자, 최근 5개 이동장소를 출력
                
                
                int findNumber = -1;
                while( 0 > findNumber || findNumber > ifctdb_len()){
                    printf("찾고자 하는 환자의 번호를 입력하세요: ");
                    scanf("%d", &findNumber );
                }
                ifctele_printElement(ifctdb_getData(findNumber));
                break;
                
            case MENU_PLACE:;
                // 지정된 장소 (문자열 형태) 에서 발병확인이 된 환자 모두의 정보 출력
                
                int placeOfPatient;
                do {
                    printf("조사하고자 하는 도시의 이름를 입력하세요: ");
                    char* Name;
                    scanf("%s", Name) ;
                    //실패시 -1 반환하기 때문에 while 처리 가능. 
                    placeOfPatient = getPlaceIndex(Name);
                
                }
                while(placeOfPatient == -1);
                   
                
                //제대로 된 도시 입력으로 Place가 구해졌을 때. 
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
                printf("찾고자 하는 구간의 나이를 입력하세요. \n 최소 : ");
                scanf("%d", &minAge );
                printf("  최대 : ");
                scanf("%d", &maxAge );
                printf("Finding Patients on Age %d 이상 %d 이하 \n", minAge, maxAge);
                
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
                // 최소와 최대 값을 입력 받아, 범위 내 나이 환자의 모두의 정보 출력
                break;
                
            case MENU_TRACK:;
                // 지정된 환자를 시작으로 전파자와 감염단한 시점 및 이동장소를
                //순차적으로 출력하고 최초 전파자를 최종 출력하고
                int checknum;
                int smallestBefore;
                int MTCount = 0;
                printf("찾고자 하는 환자의 번호를 입력하세요: ");
                scanf("%d", &checknum );
                if (checknum <= ifctdb_len()) {
                    
                    while(MTCount == 0){
                        printf("조사하는 대상의 index는 %d 입니다. \n", checknum);
                        void* patientData = ifctdb_getData(checknum);
                        //구하고자 하는 사람의 지난 장소들
                        int places[5];
                        for(int MTi =0; MTi< 5; MTi++){
                            places[MTi] = ifctele_getHistPlaceIndex(patientData, MTi);
                        }
                        int detTime = ifctele_getinfestedTime(patientData);
                        //구하고자 하는 사람의 지난 시간들
                        int times[5] = { detTime-4, detTime-3, detTime-2,detTime-1, detTime };
                        
                        smallestBefore = trackInfester(checknum, times, places);
                        if (checknum != smallestBefore){
                            checknum = smallestBefore;
                            printf("현재까지 최초 전파자는 %d 입니다 \n\n", smallestBefore);
                        } else {
                            MTCount = 1;
                            printf("따라서 최초 전파자는 %d 입니다 \n\n", smallestBefore);
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

//환자 정보를 넣으면 동선이 겹치는 환자들 중에서 제일 detected_time이 낮은 환자를 반환
int trackInfester(int patient_no, int *detected_time, int *place){
    //주어진 환자의 감염확인일자보다 더 과거의 환자를 조회
    ifctele_printElement(ifctdb_getData(patient_no));
    
    int smaller = 100;
    int smallerIndex = -1;
    for(int tii=0; tii < ifctdb_len(); tii++){
        void* old = ifctdb_getData(tii);
        if (ifctele_getinfestedTime(old) < detected_time[4]) {
            printf("확진시점이 더 작은 %d 번째 index의 환자를 조사하는중.... \n", tii);
            //ifctele_printElement(ifctdb_getData(tii));
            for (int oldPlace = 0; oldPlace<5;oldPlace++){
                for (int givenPlace = 0; givenPlace < 5; givenPlace++){
                    if (place[givenPlace] == ifctele_getHistPlaceIndex(old, oldPlace)) {
                        //printf("%d 와 %d가 %d 에서 만났을 가능성이 있습니다. \n", patient_no, tii, place[givenPlace]);
                        //printf("%d는 %d에 %d 시점에 도달했습니다. \n", patient_no, place[givenPlace], detected_time[givenPlace]);
                        //printf("%d는 %d에 %d 시점에 도달했습니다. \n", tii, place[givenPlace], ifctele_getinfestedTime(old) -4 + oldPlace);
                        if (detected_time[givenPlace] == (ifctele_getinfestedTime(old)-4 + oldPlace)){
                            printf("%d번 환자와 %d번 환자가 %d 에서 %d 시점에만났습니다. \n", patient_no, tii, place[givenPlace],detected_time[givenPlace] );
                            if (smaller > ifctele_getinfestedTime(old)) {
                                printf("%d의 감염시점이 더 기존의 %d 보다 이릅니다. 갱신합니다.\n\n ", tii, smallerIndex);
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


