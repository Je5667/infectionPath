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
    
    
    fclose(fp);
    */
    
    
    //1-2. loading each patient informations
    fp = fopen(file_name, "r");// 파일 읽기모드로 열기
    if (fp == NULL) {
        printf("fail to read file");
    }
    int idx;
    
	printf("번호 \t나이\t 확인일자 4일전 장소 \t 3일전 장소\t 2일전 장소 \t 1일전 장소 \t 당일장소\n");
	printf("====\t=====\t======== ============\t===========\t============\t============\t=========\n");
    for (idx = 0;idx<=4;idx++ ){
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
            case MENU_EXIT:
                printf("Exiting the program... Bye bye.\n");
                break;
                
            case MENU_PATIENT:
                // 지정된 환자(번호 입력)에 대한 정보 출력
                // 번호 , 나이, 감염확인일자, 최근 5개 이동장소를 출력
                printf("찾고자 하는 환자의 번호를 입력하세요: ");
                int findNumber;
                scanf("%d", &findNumber );
                ifctele_printElement(ifctdb_getData(findNumber));
                //find_patient_by(MENU_PATIENT);
                break;
                
            case MENU_PLACE:
                // 지정된 장소 (문자열 형태) 에서 발병확인이 된 환자 모두의 정보 출력
                printf("찾고자 하는 환자의 번호를 입력하세요: ");
                char city[MAX_PLACENAME];
                scanf("%s", &city );
                
                break;
                
            case MENU_AGE:
                // 최소와 최대 값을 입력 받아, 범위 내 나이 환자의 모두의 정보 출력
                break;
                
            case MENU_TRACK:
                // 지정된 환자를 시작으로 전파자와 감염단한 시점 및 이동장소를
                //순차적으로 출력하고 최초 전파자를 최종 출력하고
                break;
                
            default:
                printf("[ERROR Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
                break;
        }
    
    } while(menu_selection != 0);
    
    
    return 0;
}

