#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

// ���ڸ� �׸��� �Լ��Դϴ�.
void drawBox(int width, int height) {
    printf("��");
    for (int i = 0; i < width; i++) {
        printf("��");
    }
    printf("��\n");

    for (int i = 0; i < height; i++) {
        printf("��");
        for (int j = 0; j < width; j++) {
            printf(" ");
        }
        printf("��\n");
    }

    printf("��");
    for (int i = 0; i < width; i++) {
        printf("��");
    }
    printf("��\n");
}

// Ŀ���� �̵��ϴ� �Լ��Դϴ�.
void gotoxy(int x, int y) {
    COORD pos;
    pos.X = x - 1;
    pos.Y = y - 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// ���� ������ �����ϴ� ����ü
struct Reservation {
    char phoneNumber[14];
    int numberOfPeople;
    char time[20];
    int confirmed;
};

// ��� �ð��� ����ϴ� �Լ�
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

// ���� �ð��� �������� �Լ�
void getCurrentTime(char timeString[]) {
    time_t currentTime;
    struct tm* timeInfo;
    time(&currentTime);
    timeInfo = localtime(&currentTime);
    strftime(timeString, 20, "%H:%M:%S", timeInfo);
}

// �����͸� ���Ͽ� �����ϴ� �Լ�
void saveDataToFile(const char* filename, const char* phoneNumber, int numberOfPeople) {
    char currentTime[20];
    getCurrentTime(currentTime);

    char dataString[100];
    sprintf(dataString, "%s#%d#%s#0", phoneNumber, numberOfPeople, currentTime);

    FILE* file = fopen(filename, "a");

    if (file == NULL) {
        printf("������ �� �� �����ϴ�.");
        return;
    }

    fprintf(file, "%s\n", dataString);
    fclose(file);
}


// ���� ������ ����ϴ� �Լ�
void printReservationInfo(const struct Reservation* reservation, struct Reservation* reservations, int numReservations) {
    system("cls");  // ���� ������ ����� ���ο� ������ ���

    drawBox(70, 13);
    gotoxy(5, 5);
    printf("��ȭ��ȣ: %s\n", reservation->phoneNumber);
    gotoxy(5, 6);
    printf("������ �ο�: %d��\n", reservation->numberOfPeople);
    gotoxy(5, 7);
    printf("���� �ð�: %s\n", reservation->time);

    if (reservation->confirmed) {
        gotoxy(5, 8);
        printf("���� Ȯ��: O\n");
        gotoxy(5, 9);
        printf("������ �Ϸ�Ǿ����ϴ�. 5�� �ȿ� �������ּ���.\n");
    }
    else {
        gotoxy(5, 8);
        printf("���� Ȯ��: X\n");
        int waitTime = calculateWaitTime(reservations, numReservations, reservation->phoneNumber);
        gotoxy(5, 9);
        printf("��� �ð�: %d��\n", waitTime);
    }

    gotoxy(5, 10);
    printf("�ƹ� Ű�� ��������...");
    _getch();  // �ƹ� Ű�� ���� ������ ���
    system("cls");
}


// ������ Ȯ���ϴ� �Լ�
void checkReservation(const char* phoneNumber, struct Reservation* reservations, int numReservations) {
    int found = 0;

    for (int i = 0; i < numReservations; i++) {
        if (strcmp(reservations[i].phoneNumber, phoneNumber) == 0) {
            found = 1;
            printf("���� ����:\n");
            printReservationInfo(&reservations[i], reservations, numReservations); // ������ �κ�

            if (!reservations[i].confirmed) {
                // ������ �Ϸ���� ���� ��쿡�� ��� �ð��� ǥ��
                int waitTime = calculateWaitTime(reservations, numReservations, phoneNumber);
                //printf("��� �ð�: %d��\n", waitTime);
            }
            break;
        }
    }

    if (!found) {
        gotoxy(5, 8);
        printf("�ش� ��ȭ��ȣ�� ����� ������ ã�� �� �����ϴ�.\n");
    }
}


// ���� ���¸� Ȯ���ϴ� �Լ��Դϴ�.
void CheckReservationStatus() {
    struct Reservation reservations[100];
    int numReservations = 0;

    FILE* file = fopen("D:\\Startups txt\\Reservation.txt", "r");
    if (file == NULL) {
        printf("������ �� �� �����ϴ�.");
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
    printf("��ȭ��ȣ�� �Է��ϼ��� (��: 010-1234-5678): ");
    scanf("%13s", phoneNumberToSearch);

    checkReservation(phoneNumberToSearch, reservations, numReservations);
}

int cancelPhoneNumber(const char* filename, const char* phoneNumber) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("������ �� �� �����ϴ�");
        return 0;
    }

    char tempFileName[] = "temp.txt";
    FILE* tempFile = fopen(tempFileName, "w");
    if (tempFile == NULL) {
        perror("�ӽ� ������ �� �� �����ϴ�");
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


// ������Ʈ�� ��ϵ� ��ȣ���� Ȯ���ϴ� �Լ�               ���� ���� �ҽ�
int isBlacklisted(const char* phoneNumber) {
    FILE* blacklistFile = fopen("D:\\Startups txt\\black.txt", "r");

    if (blacklistFile == NULL) {
        perror("������Ʈ ������ �� �� �����ϴ�.");
        return 0;  // ������ �� �� ���� ���, ������Ʈ�� ���ٰ� ����
    }

    char blacklistedNumber[14];
    while (fscanf(blacklistFile, "%s", blacklistedNumber) != EOF) {
        if (strcmp(blacklistedNumber, phoneNumber) == 0) {
            fclose(blacklistFile);  // ������ �ݾ���
            return 1;  // ������Ʈ�� ��ϵ� ��ȣ�� ��� 1 ��ȯ
        }
    }

    fclose(blacklistFile);  // ������ �ݾ���
    return 0;  // ������Ʈ�� ���� ��ȣ�� ��� 0 ��ȯ
}

int isReserved(const char* phoneNumber) {
    struct Reservation reservations[100];
    int numReservations = 0;

    FILE* file = fopen("D:\\Startups txt\\Reservation.txt", "r");
    if (file == NULL) {
        printf("������ �� �� �����ϴ�.");
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


// ���� �ð��� ���� �ð��� ���̰� 6�ð� �̻����� Ȯ���ϴ� �Լ�
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

//�̰� ����
// Ư�� ��ȭ��ȣ�� ��ϵ� ���Ǹ� inquiry.txt���� �����ϴ� �Լ�
void removeInquiryByPhoneNumber(const char* phoneNumber) {
    FILE* inquiryFile = fopen("inquiry.txt", "r");
    if (inquiryFile == NULL) {
        perror("inquiry.txt ������ �� �� �����ϴ�");
        return;
    }

    FILE* tempFile = fopen("tempInquiry.txt", "w");
    if (tempFile == NULL) {
        perror("�ӽ� ������ �� �� �����ϴ�");
        fclose(inquiryFile);
        return;
    }

    char line[100];

    while (fgets(line, sizeof(line), inquiryFile) != NULL) {
        char inquiryPhoneNumber[14];
        sscanf(line, "%[^#]", inquiryPhoneNumber);

        if (strcmp(inquiryPhoneNumber, phoneNumber) == 0) {
            // ��ȭ��ȣ�� ��ġ�ϴ� ���Ǹ� ����
            continue;
        }

        fprintf(tempFile, "%s", line);
    }

    fclose(inquiryFile);
    fclose(tempFile);

    remove("inquiry.txt");
    rename("tempInquiry.txt", "inquiry.txt");
}

// �����ڰ� Ȯ��(������ Ȯ�ο��� 1)�Ͽ���, ��ϵ� �ð��� 6�ð��� ���� ������ �����ϴ� �Լ�
void removeConfirmedReservations(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("������ �� �� �����ϴ�");
        return;
    }

    char tempFileName[] = "temp.txt";
    FILE* tempFile = fopen(tempFileName, "w");
    if (tempFile == NULL) {
        perror("�ӽ� ������ �� �� �����ϴ�");
        fclose(file);
        return;
    }

    char line[100];

    while (fgets(line, sizeof(line), file) != NULL) {
        char phoneNumber[14], reservationTime[20];
        int numberOfPeople, confirmed;
        sscanf(line, "%[^#]#%d#%[^#]#%d", phoneNumber, &numberOfPeople, reservationTime, &confirmed);

        if (confirmed && isOverSixHours(reservationTime)) {
            // ������ Ȯ���� �Ǿ���, ���� �ð����κ��� 6�ð��� ���� ��� �ش� ������ �����ϰ� ���ÿ� �ش� ��ȭ��ȣ�� ��ϵ� ���Ǹ� �����մϴ�.
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
        printf("��ȭ��ȣ (��: 010-1234-5678): ");
        scanf("%13s", phoneNumber);

        // ������Ʈ�� ��ϵ� ��ȣ���� Ȯ��
        if (isBlacklisted(phoneNumber)) {
            gotoxy(5, 5);
            printf("�ش� ��ȭ��ȣ�� ������Ʈ�� ��ϵ� ��ȣ�Դϴ�.\n");
            gotoxy(5, 6);
            printf("������ �Ұ����մϴ�. �ڼ��� ���Ǵ� ���忡 ��ȭ���� ��Ź�帳�ϴ�.\n");
            _getch();
            return;
        }

        if (isReserved(phoneNumber)) {
            gotoxy(5, 5);
            printf("�ش� ��ȭ��ȣ�� �̹� ����� ��ȣ�Դϴ�.");
                _getch();
                return;
        }

        if (strlen(phoneNumber) != 13) {
            gotoxy(5, 5);
            printf("�߸��� ������ ��ȭ��ȣ�Դϴ�. �ٽ� �Է��ϼ���.\n");
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
            printf("�߸��� ������ ��ȭ��ȣ�Դϴ�. �ٽ� �Է��ϼ���.\n");
        }
        else {
            break;
        }
    }

    while (1) {
        gotoxy(5, 4);
        printf("�ο� (1~4��)�� �Է��ϼ���: ");
        if (scanf("%d", &numberOfPeople) != 1) {
            gotoxy(5, 5);
            printf("�߸��� �Է��Դϴ�. ���ڸ� �Է��ϼ���.       \n");
            while (getchar() != '\n'); // �Է� ���� ����
            continue;
        }

        if (numberOfPeople < 1 || numberOfPeople > 4) {
            gotoxy(5, 5);
            printf("�ִ� 4�α����� �����մϴ�. �ٽ� �Է��ϼ���.\n");
        }
        else {
            gotoxy(5, 5);
            printf("�Է��� �Ϸ�Ǿ����ϴ�. �ƹ� Ű�� ��������.\n");
            _getch(); // �ƹ� Ű�� ���� ������ ���
            system("cls");
            break;
        }
    }
    char currentTime[20];
    getCurrentTime(currentTime);

    saveDataToFile("D:\\Startups txt\\Reservation.txt", phoneNumber, numberOfPeople);
}
