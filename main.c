// �� 1,584 ��



#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "FileIO-call.h"
#include "FileIO-Inquiry.h"

int main() {
    int choice;

    while (1) {
        // �޴��� �׸��ϴ�.
        system("cls");
        drawBox(70, 10);
        gotoxy(5, 3);
        printf(" 1. ���� �ϱ�\n");
        gotoxy(5, 4);
        printf(" 2. ���� ��Ȳ Ȯ��\n");
        gotoxy(5, 5);
        printf(" 3. ���� ���\n");
        gotoxy(5, 6);
        printf(" 4. ���� ���\n");
        gotoxy(5, 7);
        printf(" 5. ����\n");
        gotoxy(5, 8);
        printf(" �������ּ���: ");

        // ������� ������ �޽��ϴ�.
        if (scanf("%d", &choice) != 1) {
            gotoxy(5, 10);
            printf("�ùٸ��� ���� �Է��Դϴ�. �ٽ� �Է� ���ּ���.");
            _getch();
            while (getchar() != '\n'); // �Է� ���� ����
            continue;
        }


        // ������ Ȯ�� �� 6�ð��� ���� ���� ����
        removeConfirmedReservations("D:\\Startups txt\\Reservation.txt");


        // ���ÿ� ���� ó���մϴ�.
        switch (choice) {
        case 1:
            mainFunction(); // ���� ��� ȣ��
            break;
        case 2:
            CheckReservationStatus(); // ���� ��Ȳ Ȯ�� ��� ȣ��
            _getch();
            break;
        case 3:
            RunInquiryApp(); // ���� ��� ȣ��
            break;
        case 4: {
            system("cls");
            drawBox(70, 10);
            gotoxy(5, 8);
            printf("���� ��� - ��ȭ��ȣ�� �Է��Ͽ��ּ���.: ");
            char phoneNumber[14];
            scanf("%13s", phoneNumber);
            if (cancelPhoneNumber("D:\\Startups txt\\Reservation.txt", phoneNumber)) {
                gotoxy(5, 10);
                printf("������ ��ҵǾ����ϴ�.\n");
                _getch();
            }
            else {
                gotoxy(5, 10);
                printf("������ ã�� �� �����ϴ�.\n");
                _getch();
            }
            break;
        }
        case 5:
            return 0; // ����
        default:
            gotoxy(5, 10);
            printf("�ùٸ��� ���� �Է��Դϴ�. �ٽ� �Է� ���ּ���.");
            _getch();
            while (getchar() != '\n'); // �Է� ���� ����
        }
    }

    return 0;
}
