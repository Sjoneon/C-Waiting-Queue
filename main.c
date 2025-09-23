// 총 1,584 줄



#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "FileIO-call.h"
#include "FileIO-Inquiry.h"

int main() {
    int choice;

    while (1) {
        // 메뉴를 그립니다.
        system("cls");
        drawBox(70, 10);
        gotoxy(5, 3);
        printf(" 1. 예약 하기\n");
        gotoxy(5, 4);
        printf(" 2. 예약 현황 확인\n");
        gotoxy(5, 5);
        printf(" 3. 문의 기능\n");
        gotoxy(5, 6);
        printf(" 4. 예약 취소\n");
        gotoxy(5, 7);
        printf(" 5. 종료\n");
        gotoxy(5, 8);
        printf(" 선택해주세요: ");

        // 사용자의 선택을 받습니다.
        if (scanf("%d", &choice) != 1) {
            gotoxy(5, 10);
            printf("올바르지 않은 입력입니다. 다시 입력 해주세요.");
            _getch();
            while (getchar() != '\n'); // 입력 버퍼 비우기
            continue;
        }


        // 관리자 확인 후 6시간이 지난 예약 삭제
        removeConfirmedReservations("D:\\Startups txt\\Reservation.txt");


        // 선택에 따라 처리합니다.
        switch (choice) {
        case 1:
            mainFunction(); // 예약 기능 호출
            break;
        case 2:
            CheckReservationStatus(); // 예약 현황 확인 기능 호출
            _getch();
            break;
        case 3:
            RunInquiryApp(); // 문의 기능 호출
            break;
        case 4: {
            system("cls");
            drawBox(70, 10);
            gotoxy(5, 8);
            printf("예약 취소 - 전화번호를 입력하여주세요.: ");
            char phoneNumber[14];
            scanf("%13s", phoneNumber);
            if (cancelPhoneNumber("D:\\Startups txt\\Reservation.txt", phoneNumber)) {
                gotoxy(5, 10);
                printf("예약이 취소되었습니다.\n");
                _getch();
            }
            else {
                gotoxy(5, 10);
                printf("예약을 찾을 수 없습니다.\n");
                _getch();
            }
            break;
        }
        case 5:
            return 0; // 종료
        default:
            gotoxy(5, 10);
            printf("올바르지 않은 입력입니다. 다시 입력 해주세요.");
            _getch();
            while (getchar() != '\n'); // 입력 버퍼 비우기
        }
    }

    return 0;
}
