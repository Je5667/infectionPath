//
//  ifs_element.c
//  InfestPath
//
//  Created by Juyeop Kim on 2020/10/20.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ifct_element.h"

#define file_name "patientInfo_sample.txt"

typedef enum place {
    Seoul,          //0
    Jeju,           //1
    Tokyo,          //2
    LosAngeles,     //3
    NewYork,        //4
    Texas,          //5
    Toronto,        //6
    Paris,          //7
    Nice,           //8
    Rome,           //9
    Milan,          //10
    London,         //11
    Manchester,     //12
    Basel,          //13
    Luzern,         //14
    Munich,         //15
    Frankfurt,      //16
    Berlin,         //17
    Barcelona,      //18
    Madrid,         //19
    Amsterdam,      //20
    Stockholm,      //21
    Oslo,           //22
    Hanoi,          //23
    Bangkok,        //24
    KualaLumpur,    //25
    Singapore,      //26
    Sydney,         //27
    SaoPaulo,       //28
    Cairo,          //29
    Beijing,        //30
    Nairobi,        //31
    Cancun,         //32
    BuenosAires,    //33
    Reykjavik,      //34
    Glasgow,        //35
    Warsow,         //36
    Istanbul,       //37
    Dubai,          //38
    CapeTown        //39
} place_t;

char countryName[N_PLACE+1][MAX_PLACENAME] =
{   "Seoul",
    "Jeju",
    "Tokyo",
    "LosAngeles",
    "NewYork",
    "Texas",
    "Toronto",
    "Paris",
    "Nice",
    "Rome",
    "Milan",
    "London",
    "Manchester",
    "Basel",
    "Luzern",
    "Munich",
    "Frankfurt",
    "Berlin",
    "Barcelona",
    "Madrid",
    "Amsterdam",
    "Stockholm",
    "Oslo",
    "Hanoi",
    "Bangkok",
    "KualaLumpur",
    "Singapore",
    "Sydney",
    "SaoPaulo",
    "Cairo",
    "Beijing",
    "Nairobi",
    "Cancun",
    "BuenosAires",
    "Reykjavik",
    "Glasgow",
    "Warsow",
    "Istanbul",
    "Dubai",
    "CapeTown",
    "Unrecognized"
};

//환자 구조체
typedef struct patient 
{
    int index; //번호
    int age; //나이
    int detected_time; //확인시점
    int history_place[N_HISTORY];
    //감염 확인일자는 환자의 발병이 확인된 시점 , 창궐 후 N 일째의 정수로 표현
    
} PATIENT;

PATIENT* AllocPerson() 
{ 
    PATIENT* pSomeone = malloc( sizeof( PATIENT ) ); 
    // 구조체 채움 
    return pSomeone; 
} 
/*
int main() 
{ 
PATIENT * pTom = AllocPerson(); 
// ... 
free( pTom ); // 할당한 메모리 해제 잊지 맙시다! 
return 0; 
} 
*/

//main 함수에서 txt 파일을 읽어서 얻은 데이터를 통해 PATIENT 항목을 생성
//생성한 정보의 위치를 void* 형태로 저장. 
// 추후
// PATIENT (*patient) = ifctele_genElement(~~) 로 사용
void* ifctele_genElement(int index, int age, unsigned int detected_time, int history_place[N_HISTORY]){
    //printf("%d, %d ,%d ", history_place[0], history_place[1], history_place[2]);
    PATIENT * patient = AllocPerson(); 
    *patient = (PATIENT) { .index = index, .age = age, .detected_time = detected_time,
                            .history_place[0] = history_place[0], .history_place[1] = history_place[1], .history_place[2] = history_place[2],
                            .history_place[3] = history_place[3], .history_place[4] = history_place[4]};
    //patient = {index, age, detected_time, history_place};
    return patient;
}

// 위치의 index -> 문자열로 변환. 포인터 형태로 전달
char* ifctele_getPlaceName(int placeIndex){
    char * a = strdup(countryName[placeIndex]);
    return a;
}
// PATIENT의 history_place의 index 에서의 위치(Int)를 반환
int ifctele_getHistPlaceIndex(void* obj, int index){
    PATIENT (*patient) = obj;
    return patient->history_place[index];
}
//PATIENT가 감염된 날짜를 구하기 
unsigned int ifctele_getinfestedTime(void* obj){
    PATIENT (*patient) = obj;
    int date = patient->detected_time;
    return date-3;
}

//PATIENT 의 나이를 반환
int ifctele_getAge(void* obj){
    PATIENT (*patient) = obj;
    return patient->age;
}

//Professor Bold체 
//환자의 모든 정보를 반환
void ifctele_printElement(void* obj){
    PATIENT (*patient) = obj;
    printf("확진번호 : %d, 나이: %2d, 확진시점: %2d, 장소: %-12s -> %-12s -> %-12s -> %-12s ->%-12s \n",
        patient->index, patient->age, patient->detected_time, countryName[patient->history_place[0]],
		countryName[patient->history_place[1]], countryName[patient->history_place[2]],
        countryName[patient->history_place[3]], countryName[patient->history_place[4]]);
}
/*

PATIENT* patient_info();
void find_patient_by(int menu);
void printPatientInfo(PATIENT patient);

PATIENT* patient_info() {
    
    static PATIENT patients[10];
    FILE* fp = fopen(file_name, "r");// 파일 읽기모드로 열기
    if (fp == NULL) {
        printf("fail to read file");
    }
    int idx;
	printf("번호 \t나이\t 확인일자 4일전 장소 \t 3일전 장소\t 2일전 장소 \t 1일전 장소 \t 당일장소\n");
	printf("====\t=====\t======== ============\t===========\t============\t============\t=========\n");
    for (idx = 0;idx<=4;idx++ ){
         fscanf(fp, "%d %d %d %d %d %d %d %d",
    		&patients[idx].ID, &patients[idx].Age, &patients[idx].CheckDate, &patients[idx].exPlaces[0], &patients[idx].exPlaces[1], 
    		&patients[idx].exPlaces[2],&patients[idx].exPlaces[3],&patients[idx].exPlaces[4]);
    	printPatientInfo(patients[idx]);
    }
    fclose(fp); // 파일 닫기
    return patients;
    
}


void printPatientInfo(PATIENT patient){
    //printf("번호 \t나이\t 확인일자 4일전 장소 \t 3일전 장소\t 2일전 장소 \t 1일전 장소 \t 당일장소\n");
    //printf("====\t=====\t======== ============\t===========\t============\t============\t=========\n");
    printf("%d\t  %2d \t %2d \t %-12s \t %-12s \t %-12s \t %-12s \t %-12s\n",
        patient.ID, patient.Age, patient.CheckDate, countryName[patient.exPlaces[0]],
		countryName[patient.exPlaces[1]], countryName[patient.exPlaces[2]],
        countryName[patient.exPlaces[3]], countryName[patient.exPlaces[4]]);
}

void find_patient_by( int menu) {
    PATIENT* patients = patient_info();
    switch(menu)
        {
            case 1: // 환자의 ID로 찾는 경우
                printf("찾고자 하는 환자의 번호를 입력하세요: ");
                int findNumber;
                scanf("%d", &findNumber );
                for(int i = 0; i < 6 ; i++){
                    if (patients[i].ID == findNumber) {
                        printf("번호 \t나이\t 확인일자 4일전 장소 \t 3일전 장소\t 2일전 장소 \t 1일전 장소 \t 당일장소\n");
	                    printf("====\t=====\t======== ============\t===========\t============\t============\t=========\n");
                        printPatientInfo(patients[i]);
                    }
                }
        }
    
    return;
}
*/


