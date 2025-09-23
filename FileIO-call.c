#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

// 상자를 그리는 함수입니다.
void drawBox(int width, int height) {
    printf("┌");
    for (int i = 0; i < width; i++) {
        printf("─");
    }
    printf("┐\n");

    for (int i = 0; i < height; i++) {
        printf("│");
        for (int j = 0; j < width; j++) {
            printf(" ");
        }
        printf("│\n");
    }

    printf("└");
    for (int i = 0; i < width; i++) {
        printf("─");
    }
    printf("┘\n");
}

// 커서를 이동하는 함수입니다.
void gotoxy(int x, int y) {
    COORD pos;
    pos.X = x - 1;
    pos.Y = y - 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// 예약 정보를 저장하는 구조체
struct Reservation {
    char phoneNumber[14];
    int numberOfPeople;
    char time[20];
    int confirmed;
};

// 대기 시간을 계산하는 함수
int calculateWaitTime(const struct Reservation* reservations, int numReservations, const char* phoneNumber) {
    int totalNumberOfPeople = 0;
    int found = 0;
    int reservationIndex = 0;

    for (int i = 0; i < numReservations; i++) {
        if (strcmp(reservations[i].phoneNumber, phoneNumber) == 0) {
            found = 1;
            reservationIndex = i;
            break;
        }
    }

    if (found) {
        for (int i = 0; i <= reservationIndex; i++) {
            totalNumberOfPeople += reservations[i].numberOfPeople;
        }
    }

    return 7 + totalNumberOfPeople;
}

// 현재 시간을 가져오는 함수
void getCurrentTime(char timeString[]) {
    time_t currentTime;
    struct tm* timeInfo;
    time(&currentTime);
    timeInfo = localtime(&currentTime);
    strftime(timeString, 20, "%H:%M:%S", timeInfo);
}

// 데이터를 파일에 저장하는 함수
void saveDataToFile(const char* filename, const char* phoneNumber, int numberOfPeople) {
    char currentTime[20];
    getCurrentTime(currentTime);

    char dataString[100];
    sprintf(dataString, "%s#%d#%s#0", phoneNumber, numberOfPeople, currentTime);

    FILE* file = fopen(filename, "a");

    if (file == NULL) {
        printf("파일을 열 수 없습니다.");
        return;
    }

    fprintf(file, "%s\n", dataString);
    fclose(file);
}


// 예약 정보를 출력하는 함수
void printReservationInfo(const struct Reservation* reservation, struct Reservation* reservations, int numReservations) {
    system("cls");  // 기존 내용을 지우고 새로운 정보를 출력

    drawBox(70, 13);
    gotoxy(5, 5);
    printf("전화번호: %s\n", reservation->phoneNumber);
    gotoxy(5, 6);
    printf("예약한 인원: %d명\n", reservation->numberOfPeople);
    gotoxy(5, 7);
    printf("예약 시간: %s\n", reservation->time);

    if (reservation->confirmed) {
        gotoxy(5, 8);
        printf("예약 확인: O\n");
        gotoxy(5, 9);
        printf("예약이 완료되었습니다. 5분 안에 입장해주세요.\n");
    }
    else {
        gotoxy(5, 8);
        printf("예약 확인: X\n");
        int waitTime = calculateWaitTime(reservations, numReservations, reservation->phoneNumber);
        gotoxy(5, 9);
        printf("대기 시간: %d분\n", waitTime);
    }

    gotoxy(5, 10);
    printf("아무 키나 누르세요...");
    _getch();  // 아무 키나 누를 때까지 대기
    system("cls");
}


// 예약을 확인하는 함수
void checkReservation(const char* phoneNumber, struct Reservation* reservations, int numReservations) {
    int found = 0;

    for (int i = 0; i < numReservations; i++) {
        if (strcmp(reservations[i].phoneNumber, phoneNumber) == 0) {
            found = 1;
            printf("예약 정보:\n");
            printReservationInfo(&reservations[i], reservations, numReservations); // 수정된 부분

            if (!reservations[i].confirmed) {
                // 예약이 완료되지 않은 경우에만 대기 시간을 표시
                int waitTime = calculateWaitTime(reservations, numReservations, phoneNumber);
                //printf("대기 시간: %d분\n", waitTime);
            }
            break;
        }
    }

    if (!found) {
        gotoxy(5, 8);
        printf("해당 전화번호로 예약된 정보를 찾을 수 없습니다.\n");
    }
}


// 예약 상태를 확인하는 함수입니다.
void CheckReservationStatus() {
    struct Reservation reservations[100];
    int numReservations = 0;

    FILE* file = fopen("D:\\Startups txt\\Reservation.txt", "r");
    if (file == NULL) {
        printf("파일을 열 수 없습니다.");
        return;
    }

    while (fscanf(file, "%[^#]#%d#%[^#]#%d\n", reservations[numReservations].phoneNumber, &reservations[numReservations].numberOfPeople, reservations[numReservations].time, &reservations[numReservations].confirmed) != EOF) {
        numReservations++;
    }
    fclose(file);

    char phoneNumberToSearch[14];

    system("cls");
    drawBox(70, 10);
    gotoxy(5, 3);
    printf("전화번호를 입력하세요 (예: 010-1234-5678): ");
    scanf("%13s", phoneNumberToSearch);

    checkReservation(phoneNumberToSearch, reservations, numReservations);
}

int cancelPhoneNumber(const char* filename, const char* phoneNumber) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("파일을 열 수 없습니다");
        return 0;
    }

    char tempFileName[] = "temp.txt";
    FILE* tempFile = fopen(tempFileName, "w");
    if (tempFile == NULL) {
        perror("임시 파일을 열 수 없습니다");
        fclose(file);
        return 0;
    }

    char line[100];
    int canceled = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        char savedPhoneNumber[14];
        sscanf(line, "%[^#]", savedPhoneNumber);

        if (strcmp(savedPhoneNumber, phoneNumber) == 0) {
            canceled = 1;
        }
        else {
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(filename);
    rename(tempFileName, filename);

    return canceled;
}


// 블랙리스트에 등록된 번호인지 확인하는 함수               삭제 예정 소스
int isBlacklisted(const char* phoneNumber) {
    FILE* blacklistFile = fopen("D:\\Startups txt\\black.txt", "r");

    if (blacklistFile == NULL) {
        perror("블랙리스트 파일을 열 수 없습니다.");
        return 0;  // 파일을 열 수 없는 경우, 블랙리스트에 없다고 간주
    }

    char blacklistedNumber[14];
    while (fscanf(blacklistFile, "%s", blacklistedNumber) != EOF) {
        if (strcmp(blacklistedNumber, phoneNumber) == 0) {
            fclose(blacklistFile);  // 파일을 닫아줌
            return 1;  // 블랙리스트에 등록된 번호인 경우 1 반환
        }
    }

    fclose(blacklistFile);  // 파일을 닫아줌
    return 0;  // 블랙리스트에 없는 번호인 경우 0 반환
}

int isReserved(const char* phoneNumber) {
    struct Reservation reservations[100];
    int numReservations = 0;

    FILE* file = fopen("D:\\Startups txt\\Reservation.txt", "r");
    if (file == NULL) {
        printf("파일을 열 수 없습니다.");
        return 0;
    }

    while (fscanf(file, "%[^#]#%d#%[^#]#%d\n", reservations[numReservations].phoneNumber, &reservations[numReservations].numberOfPeople, reservations[numReservations].time, &reservations[numReservations].confirmed) != EOF) {
        numReservations++;
    }
    fclose(file);

    for (int i = 0; i < numReservations; i++) {
        if (strcmp(reservations[i].phoneNumber, phoneNumber) == 0) {
            return 1;
        }
    }

    return 0;
}


// 현재 시간과 예약 시간의 차이가 6시간 이상인지 확인하는 함수
int isOverSixHours(const char* reservationTime) {
    int reservationHour, reservationMinute, reservationSecond;

    sscanf(reservationTime, "%d:%d:%d", &reservationHour, &reservationMinute, &reservationSecond);

    time_t currentTime;
    struct tm* currentTimeInfo;

    time(&currentTime);
    currentTimeInfo = localtime(&currentTime);

    int currentTotalSeconds = currentTimeInfo->tm_hour * 3600 + currentTimeInfo->tm_min * 60 + currentTimeInfo->tm_sec;
    int reservationTotalSeconds = reservationHour * 3600 + reservationMinute * 60 + reservationSecond;

    if (currentTotalSeconds < reservationTotalSeconds) {
        // If the current time is less than the reservation time (which could happen if the reservation was for late in the day and it's now past midnight), add 24 hours' worth of seconds to the current time.
        currentTotalSeconds += 24 * 3600;
    }

    return (currentTotalSeconds - reservationTotalSeconds) >= 1 * 60 * 60;
}

//이거 실패
// 특정 전화번호로 등록된 문의를 inquiry.txt에서 삭제하는 함수
void removeInquiryByPhoneNumber(const char* phoneNumber) {
    FILE* inquiryFile = fopen("inquiry.txt", "r");
    if (inquiryFile == NULL) {
        perror("inquiry.txt 파일을 열 수 없습니다");
        return;
    }

    FILE* tempFile = fopen("tempInquiry.txt", "w");
    if (tempFile == NULL) {
        perror("임시 파일을 열 수 없습니다");
        fclose(inquiryFile);
        return;
    }

    char line[100];

    while (fgets(line, sizeof(line), inquiryFile) != NULL) {
        char inquiryPhoneNumber[14];
        sscanf(line, "%[^#]", inquiryPhoneNumber);

        if (strcmp(inquiryPhoneNumber, phoneNumber) == 0) {
            // 전화번호가 일치하는 문의를 삭제
            continue;
        }

        fprintf(tempFile, "%s", line);
    }

    fclose(inquiryFile);
    fclose(tempFile);

    remove("inquiry.txt");
    rename("tempInquiry.txt", "inquiry.txt");
}

// 관리자가 확인(관리자 확인여부 1)하였고, 등록된 시간이 6시간이 지난 예약을 삭제하는 함수
void removeConfirmedReservations(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("파일을 열 수 없습니다");
        return;
    }

    char tempFileName[] = "temp.txt";
    FILE* tempFile = fopen(tempFileName, "w");
    if (tempFile == NULL) {
        perror("임시 파일을 열 수 없습니다");
        fclose(file);
        return;
    }

    char line[100];

    while (fgets(line, sizeof(line), file) != NULL) {
        char phoneNumber[14], reservationTime[20];
        int numberOfPeople, confirmed;
        sscanf(line, "%[^#]#%d#%[^#]#%d", phoneNumber, &numberOfPeople, reservationTime, &confirmed);

        if (confirmed && isOverSixHours(reservationTime)) {
            // 관리자 확인이 되었고, 예약 시간으로부터 6시간이 지난 경우 해당 예약을 삭제하고 동시에 해당 전화번호로 등록된 문의를 삭제합니다.
            removeInquiryByPhoneNumber(phoneNumber);
            continue;
        }

        fprintf(tempFile, "%s", line);
    }

    fclose(file);
    fclose(tempFile);

    remove(filename);
    rename(tempFileName, filename);
}



void mainFunction() {
    char phoneNumber[14];
    int numberOfPeople;

    while (1) {
        system("cls");
        drawBox(70, 10);
        gotoxy(5, 3);
        printf("전화번호 (예: 010-1234-5678): ");
        scanf("%13s", phoneNumber);

        // 블랙리스트에 등록된 번호인지 확인
        if (isBlacklisted(phoneNumber)) {
            gotoxy(5, 5);
            printf("해당 전화번호는 블랙리스트에 등록된 번호입니다.\n");
            gotoxy(5, 6);
            printf("예약이 불가능합니다. 자세한 문의는 업장에 전화문의 부탁드립니다.\n");
            _getch();
            return;
        }

        if (isReserved(phoneNumber)) {
            gotoxy(5, 5);
            printf("해당 전화번호는 이미 예약된 번호입니다.");
                _getch();
                return;
        }

        if (strlen(phoneNumber) != 13) {
            gotoxy(5, 5);
            printf("잘못된 형식의 전화번호입니다. 다시 입력하세요.\n");
            _getch();
            continue;
        }

        int valid = 1;
        for (int i = 0; i < 13; i++) {
            if (i == 3 || i == 8) {
                if (phoneNumber[i] != '-') {
                    valid = 0;
                    break;
                }
            }
            else {
                if (!isdigit(phoneNumber[i])) {
                    valid = 0;
                    break;
                }
            }
        }

        if (!valid) {
            printf("잘못된 형식의 전화번호입니다. 다시 입력하세요.\n");
        }
        else {
            break;
        }
    }

    while (1) {
        gotoxy(5, 4);
        printf("인원 (1~4명)을 입력하세요: ");
        if (scanf("%d", &numberOfPeople) != 1) {
            gotoxy(5, 5);
            printf("잘못된 입력입니다. 숫자를 입력하세요.       \n");
            while (getchar() != '\n'); // 입력 버퍼 비우기
            continue;
        }

        if (numberOfPeople < 1 || numberOfPeople > 4) {
            gotoxy(5, 5);
            printf("최대 4인까지만 가능합니다. 다시 입력하세요.\n");
        }
        else {
            gotoxy(5, 5);
            printf("입력이 완료되었습니다. 아무 키나 누르세요.\n");
            _getch(); // 아무 키나 누를 때까지 대기
            system("cls");
            break;
        }
    }
    char currentTime[20];
    getCurrentTime(currentTime);

    saveDataToFile("D:\\Startups txt\\Reservation.txt", phoneNumber, numberOfPeople);
}
