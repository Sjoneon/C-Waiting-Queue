#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include<Windows.h>



/*이거 해당 전화번호의 문의를 보낼려면 전화번호#사용자 문의#관리자 문의
이런식으로 해야할듯 그리고 관리자 문의가 되면
한 6시간후 삭제되게 하고 사용자는 문의는 시간이 지나야 문의를 다시 등록할 수 있게 해야 될듯
*/


void InquirywriteData(const char* phoneNumber) {
    FILE* file = fopen("D:\\Startups txt\\Inquiry.txt", "a");

    if (file == NULL) {
        printf("파일 Inquiry.txt을 열 수 없습니다");
        return;
    }

    char question[62];
    size_t len;

    while (1) {
        system("cls");
        drawBox(115, 15);
        gotoxy(5, 7);
        printf("문의 사항 (최대 30자): ");
        fflush(stdin);

        if (fgets(question, sizeof(question), stdin) == NULL) {
            continue;
        }

        len = strlen(question);
        if (len > 0 && question[len - 1] == '\n') {
            question[len - 1] = '\0';
        }
        else {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }

        len = strlen(question);
        if (len > 60) {
            system("cls");
            drawBox(70, 13);
            gotoxy(18, 7);
            printf("30자 이상은 입력이 되지 않습니다.\n");
            _getch();
            system("cls");
        }
        else {
            break;
        }
    }

    fprintf(file, "%s#%s\n", phoneNumber, question);
    fclose(file);

    gotoxy(5, 11);
    printf("소중한 고객님의 문의가 등록 완료되었습니다.\n");
    _getch();
}
int isInquiryRegistered(const char* phoneNumber) {
    char line[100];

    FILE* file = fopen("D:\\Startups txt\\Inquiry.txt", "r");
    if (file == NULL) {
        printf("파일을 열 수 없습니다.");
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        char* token = strtok(line, "#");
        if (strcmp(token, phoneNumber) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void CheckInquiryStatus(const char* phoneNumber) {
    FILE* file = fopen("D:\\Startups txt\\Inquiry.txt", "r");
    if (file == NULL) {
        printf("파일 Inquiry.txt을 열 수 없습니다");
        return;
    }

    bool hasInquiry = false;

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        char storedPhoneNumber[14];
        char userInquiry[62];
        char adminInquiry[256]= "";

        // 수정된 sscanf 부분: 답변 부분에서 #을 포함한 문자열을 읽도록 수정
        sscanf(line, "%[^#]#%[^#]#%[^\n]", storedPhoneNumber, userInquiry, adminInquiry);

        if (strcmp(storedPhoneNumber, phoneNumber) == 0) {
            hasInquiry = true;

            system("cls");
            drawBox(80, 15);
            gotoxy(5, 5);
            printf("고객 문의 내용: %s", userInquiry);

            if (strlen(adminInquiry) > 0) { // 관리자 답변이 있는 경우에만 출력
                gotoxy(5, 8);
                printf("관리자 답변 내용: %s", adminInquiry);
            }
            else {
                gotoxy(5, 8);
                printf("등록된 답변이 없습니다.");
            }
            _getch();
            break;
        }
    }

    fclose(file);

    if (!hasInquiry) {
        system("cls"); // Clear the screen before displaying the message
        drawBox(40, 5);
        gotoxy(5, 2);
        printf("등록된 문의가 없습니다.");
        _getch();
    }
}



void RunInquiryApp() {
    int choice;
    char phoneNumber[14];

    system("cls");
    drawBox(70, 10);
    gotoxy(5, 3);
    printf("전화번호 (예: 010-1234-5678): ");
    scanf("%13s", phoneNumber);

    if (!isReserved(phoneNumber)) {
        gotoxy(5, 5);
        printf("등록되지 않은 전화번호입니다. 다시 확인 후 입력해주세요.");
        return;
    }

    do {
        system("cls");
        drawBox(100, 13);
        gotoxy(5, 5);
        printf("1. 문의 등록");
        gotoxy(5, 6);
        printf("2. 문의 현황");
        gotoxy(5, 7);
        printf("3. 종료");
        gotoxy(5, 8);
        printf("선택하세요: ");
        scanf("%d", &choice);

        while (getchar() != '\n');

            switch (choice) {
            case 1:
                if (isInquiryRegistered(phoneNumber)) {
                    gotoxy(5, 8);
                    printf("해당 전화번호로 이미 문의가 등록 되었습니다. 다음에 문의가 처리된 후 다시 등록 바랍니다.");
                    _getch();
                    break;
                }
                InquirywriteData(phoneNumber);
                break;
            case 2:
                //여기에 관리자가 문의 달았나 확인(멈추는거는 _getch() 쓰면 될듯
                CheckInquiryStatus(phoneNumber);
                _getch();
                break;
            case 3:
                return;
            }
    } while (1);
}
