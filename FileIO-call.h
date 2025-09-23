#pragma once

void getCurrentTime(char timeString[]);
void saveDataToFile(const char* filename, const char* phoneNumber, int numberOfPeople);
int calculateWaitTime(const struct Reservation* reservations, int numReservations, const char* phoneNumber);
void mainFunction();
void printReservationInfo(const struct Reservation* reservation);
void checkReservation(const char* phoneNumber, struct Reservation* reservations, int numReservations);
void CheckReservationStatus();
void removeExpiredReservations(struct Reservation* reservations, int* numReservations);
void drawBox(int width, int height);
void gotoxy(int x, int y);
void point(int x, int y);
void setColor(unsigned short text);
int isBlacklisted(const char* phoneNumber);
int isReserved(const char* phoneNumber);

int isOverSixHours(const char* reservationTime);
void removeConfirmedReservations(const char* filename);