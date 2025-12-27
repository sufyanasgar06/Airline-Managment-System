#include<iostream>
#include<string>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <map>
using namespace std;

const int MAX_PASSENGERS = 100;
const int MAX_FLIGHTS = 100;
const int MAX_BOOKINGS = 100;
const int MAX_ADMINS = 5;

struct Date {
    int day;
    int month;
    int year;
};

struct Time {
    int hour;
    int minute;
};

struct Passenger {
    int id;
    char name[50];
    char password[30];
    char email[50];
    char phone[15];
    int totalBookings;
    float totalSpent;
};

struct Flight {
    int flightNo;
    char destination[50];
    char origin[50];
    Date departureDate;
    Time departureTime;
    Date arrivalDate;
    Time arrivalTime;
    int economySeats;
    int businessSeats;
    int firstClassSeats;
    float economyFare;
    float businessFare;
    float firstClassFare;
    int totalSeats;
    int availableSeats;
    float distance;
    char status[20];
    int timesBooked;
    float totalRevenue;
    double baseFare;
};

struct Booking {
    int bookingId;
    int passengerId;
    int flightNo;
    Date bookingDate;
    Date travelDate;
    int seatsBooked;
    char classType[20];
    float farePaid;
    char status[20]; 
};

struct Admin {
    int id;
    char username[30];
    char password[30];
    char email[50];
};

// Global arrays
Passenger passengers[MAX_PASSENGERS];
Flight flights[MAX_FLIGHTS];
Booking bookings[MAX_BOOKINGS];
Admin admins[MAX_ADMINS];

// Global counters
int passengerCount = 0;
int flightCount = 0;
int bookingCount = 0;
int adminCount = 0;
int currentPassengerId = -1;
int currentAdminId = -1;

// Function prototypes
void viewAvailableFlights();
void bookFlight();
void showPassengerMenu();
void passengerLogin();
bool isValidEmail(const char* email);
void PassengerRegistration();
void mainMenu();
bool isValidDate(const Date& date);
bool isFutureDate(const Date& date);
float calculateFare(const Flight& flight, int seats, const string& classType);
int generateBookingId();
void addSampleData();

Booking* findBookingById(int bookingId, int& index);
Flight* findFlightByNumber(int flightNo, int& index);
float calculateRefundAmount(const Booking& booking);
void displayPassengerBookings();
void cancelBooking();
void generatePersonalReport();

// Admin functions
void adminMenu();
void adminLoginPanel();
void addFlight(Flight flights[], int &flightCount);
void viewFlights(Flight flights[], int flightCount);
void updateFlight(Flight flights[], int flightCount);
void deleteFlight(Flight flights[], int &flightCount);
void viewAllBookings();

// Helper functions
string formatDate(const Date& date);
string formatTime(const Time& time);
int countBookingsByStatus(const char* status);
float getTotalSpentOnBookings();
void displayPassengerInfo();
void displayBookingSummary();
void displayBookingHistory();
void displayRecentBookings();

// Profile update functions
void updateProfile();
void displayCurrentProfile();
void updateName();
void updateEmail();
void updatePhone();
void updatePassword();
bool isValidPhone(const char* phone);
bool isValidDate(int day, int month, int year);
bool isValidTime(int hour, int minute);

// ========== VALIDATION FUNCTIONS ==========

bool isValidDate(const Date& date) {
    if (date.day < 1 || date.day > 31) return false;
    if (date.month < 1 || date.month > 12) return false;
    if (date.year < 2024) return false;
    return true;
}

bool isFutureDate(const Date& date) {
    time_t now = time(0);
    tm* currentTime = localtime(&now);
    
    int currentYear = currentTime->tm_year + 1900;
    int currentMonth = currentTime->tm_mon + 1;
    int currentDay = currentTime->tm_mday;
    
    if (date.year < currentYear) return false;
    if (date.year == currentYear && date.month < currentMonth) return false;
    if (date.year == currentYear && date.month == currentMonth && date.day <= currentDay) return false;
    
    return true;
}

bool isValidEmail(const char* email) {
    int atCount = 0;
    int dotCount = 0;
    int len = strlen(email);
    
    for (int i = 0; i < len; i++) {
        if (email[i] == '@') atCount++;
        if (email[i] == '.') dotCount++;
    }
    
    return (atCount == 1 && dotCount >= 1 && len >= 5);
}

bool isValidPhone(const char* phone) {
    int len = strlen(phone);
    if (len < 10 || len > 15) return false;
    
    for (int i = 0; i < len; i++) {
        if (i == 0 && phone[i] == '+') continue;
        if (!isdigit(phone[i])) return false;
    }
    return true;
}

bool isValidDate(int day, int month, int year) {
    if (year < 2024 || year > 2100) return false;
    if (month < 1 || month > 12) return false;

    int maxDays;
    if (month == 2)
        maxDays = (year % 4 == 0) ? 29 : 28;
    else if (month == 4 || month == 6 || month == 9 || month == 11)
        maxDays = 30;
    else
        maxDays = 31;

    return (day >= 1 && day <= maxDays);
}

bool isValidTime(int hour, int minute) {
    return (hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59);
}

// ========== FORMATTING FUNCTIONS ==========

string formatDate(const Date& date) {
    return to_string(date.day) + "/" + 
           to_string(date.month) + "/" + 
           to_string(date.year);
}

string formatTime(const Time& time) {
    string hour = to_string(time.hour);
    string minute = (time.minute < 10) ? "0" + to_string(time.minute) : to_string(time.minute);
    return hour + ":" + minute;
}

// ========== CALCULATION FUNCTIONS ==========

float calculateFare(const Flight& flight, int seats, const string& classType) {
    float multiplier;
    
    if (classType == "Economy") multiplier = 1.0;
    else if (classType == "Business") multiplier = 2.0;
    else if (classType == "First") multiplier = 3.5;
    else multiplier = 1.0;
    
    return flight.baseFare * seats * multiplier;
}

int generateBookingId() {
    static int lastId = 1000;
    return ++lastId;
}

float calculateRefundAmount(const Booking& booking) {
    time_t now = time(0);
    tm* currentTime = localtime(&now);
    Date currentDate = {currentTime->tm_mday, currentTime->tm_mon + 1, currentTime->tm_year + 1900};
    
    int daysBefore = (booking.travelDate.year - currentDate.year) * 365 +
                     (booking.travelDate.month - currentDate.month) * 30 +
                     (booking.travelDate.day - currentDate.day);
    
    if (daysBefore >= 7) return booking.farePaid * 0.9;
    else if (daysBefore >= 3) return booking.farePaid * 0.5;
    else if (daysBefore >= 1) return booking.farePaid * 0.2;
    else return 0.0;
}

// ========== SEARCH FUNCTIONS ==========

Booking* findBookingById(int bookingId, int& index) {
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].bookingId == bookingId && 
            bookings[i].passengerId == currentPassengerId) {
            index = i;
            return &bookings[i];
        }
    }
    return nullptr;
}

Flight* findFlightByNumber(int flightNo, int& index) {
    for (int i = 0; i < flightCount; i++) {
        if (flights[i].flightNo == flightNo) {
            index = i;
            return &flights[i];
        }
    }
    return nullptr;
}

// ========== VIEW FUNCTIONS ==========

void viewAvailableFlights() {
    cout << "\n=== AVAILABLE FLIGHTS ===\n";
    
    if (flightCount == 0) {
        cout << "Sorry! No flights available at the moment.\n";
        return;
    }
    
    cout << left << setw(10) << "Flight #" 
         << setw(15) << "From" 
         << setw(15) << "To" 
         << setw(10) << "Seats" 
         << setw(10) << "Fare" 
         << setw(12) << "Status" << "\n";
    
    for (int i = 0; i < flightCount; i++) {
        if (strcmp(flights[i].status, "Available") == 0 && flights[i].availableSeats > 0) {
            cout << left << setw(10) << flights[i].flightNo
                 << setw(15) << flights[i].origin
                 << setw(15) << flights[i].destination
                 << setw(10) << flights[i].availableSeats
                 << setw(10) << "$" << flights[i].baseFare
                 << setw(12) << flights[i].status << "\n";
        }
    }
}

void viewFlights(Flight flights[], int flightCount) {
    if (flightCount == 0) {
        cout << "\nNo flights available.\n";
        return;
    }

    cout << "\n========== AVAILABLE FLIGHTS ==========\n";

    for (int i = 0; i < flightCount; i++) {
        cout << "\n--------------------------------------\n";
        cout << "Flight Number   : " << flights[i].flightNo << endl;
        cout << "Origin          : " << flights[i].origin << endl;
        cout << "Destination     : " << flights[i].destination << endl;
        cout << "Departure Date  : " << flights[i].departureDate.day << "/"
             << flights[i].departureDate.month << "/" << flights[i].departureDate.year << endl;
        cout << "Departure Time  : " << flights[i].departureTime.hour << ":"
             << flights[i].departureTime.minute << endl;
        cout << "Arrival Date    : " << flights[i].arrivalDate.day << "/"
             << flights[i].arrivalDate.month << "/" << flights[i].arrivalDate.year << endl;
        cout << "Arrival Time    : " << flights[i].arrivalTime.hour << ":"
             << flights[i].arrivalTime.minute << endl;
        cout << "\n--- Seats & Fares ---\n";
        cout << "Economy Seats   : " << flights[i].economySeats
             << " | Fare: " << flights[i].economyFare << endl;
        cout << "Business Seats  : " << flights[i].businessSeats
             << " | Fare: " << flights[i].businessFare << endl;
        cout << "First Class Seats: " << flights[i].firstClassSeats
             << " | Fare: " << flights[i].firstClassFare << endl;
        cout << "\nTotal Seats     : " << flights[i].totalSeats << endl;
        cout << "Available Seats : " << flights[i].availableSeats << endl;
        cout << "Distance        : " << flights[i].distance << " km" << endl;
        cout << "Status          : " << flights[i].status << endl;
        cout << "Times Booked    : " << flights[i].timesBooked << endl;
        cout << "Total Revenue   : " << flights[i].totalRevenue << endl;
    }
    cout << "\n======================================\n";
}

void viewAllBookings() {
    cout << "\n=== ALL BOOKINGS ===\n";
    
    if (bookingCount == 0) {
        cout << "No bookings found.\n";
        return;
    }
    
    cout << left << setw(12) << "Booking ID"
         << setw(15) << "Passenger ID"
         << setw(10) << "Flight #"
         << setw(12) << "Travel Date"
         << setw(10) << "Seats"
         << setw(12) << "Class"
         << setw(12) << "Fare Paid($)"
         << setw(12) << "Status" << "\n";
    
    for (int i = 0; i < bookingCount; i++) {
        string travelDateStr = to_string(bookings[i].travelDate.day) + "/" +
                              to_string(bookings[i].travelDate.month) + "/" +
                              to_string(bookings[i].travelDate.year);
        
        cout << left << setw(12) << bookings[i].bookingId
             << setw(15) << bookings[i].passengerId
             << setw(10) << bookings[i].flightNo
             << setw(12) << travelDateStr
             << setw(10) << bookings[i].seatsBooked
             << setw(12) << bookings[i].classType
             << setw(12) << fixed << setprecision(2) << bookings[i].farePaid
             << setw(12) << bookings[i].status << "\n";
    }
}

// ========== BOOKING FUNCTIONS ==========

void bookFlight() {
    if (currentPassengerId == -1) {
        cout << "You must login first!\n";
        return;
    }
    
    cout << "\n=== BOOK A FLIGHT ===\n";
    viewAvailableFlights();
    
    if (flightCount == 0) {
        cout << "No flights available to book.\n";
        return;
    }
    
    int flightChoice;
    cout << "\nEnter Flight Number to book (0 to cancel): ";
    cin >> flightChoice;
    
    if (flightChoice == 0) return;
    
    Flight* selectedFlight = nullptr;
    int flightIndex = -1;
    
    for (int i = 0; i < flightCount; i++) {
        if (flights[i].flightNo == flightChoice && 
            strcmp(flights[i].status, "Available") == 0 && 
            flights[i].availableSeats > 0) {
            selectedFlight = &flights[i];
            flightIndex = i;
            break;
        }
    }
    
    if (!selectedFlight) {
        cout << "Invalid flight selection or flight not available!\n";
        return;
    }
    
    int seats;
    cout << "Number of seats to book (1-" << selectedFlight->availableSeats << "): ";
    cin >> seats;
    
    while (seats < 1 || seats > selectedFlight->availableSeats) {
        cout << "Invalid! Enter between 1 and " << selectedFlight->availableSeats << " seats: ";
        cin >> seats;
    }
    
    cout << "\nSelect Class Type:\n";
    cout << "1. Economy (1.0x)\n";
    cout << "2. Business (2.0x)\n";
    cout << "3. First Class (3.5x)\n";
    
    int classChoice;
    cout << "Enter choice (1-3): ";
    cin >> classChoice;
    
    string classType;
    switch(classChoice) {
        case 1: classType = "Economy"; break;
        case 2: classType = "Business"; break;
        case 3: classType = "First"; break;
        default: 
            cout << "Invalid choice! Defaulting to Economy.\n";
            classType = "Economy";
            break;
    }
    
    Date travelDate;
    cout << "\nEnter travel date (DD MM YYYY): ";
    cin >> travelDate.day >> travelDate.month >> travelDate.year;
    
    if (!isValidDate(travelDate)) {
        cout << "Invalid date!\n";
        return;
    }
    
    if (!isFutureDate(travelDate)) {
        cout << "Travel date must be in the future!\n";
        return;
    }
    
    float fare = calculateFare(*selectedFlight, seats, classType);
    
    cout << "\n=== BOOKING SUMMARY ===\n";
    cout << "Flight: " << selectedFlight->origin << " to " << selectedFlight->destination << "\n";
    cout << "Date: " << travelDate.day << "/" << travelDate.month << "/" << travelDate.year << "\n";
    cout << "Seats: " << seats << " (" << classType << " class)\n";
    cout << "Total Fare: $" << fixed << setprecision(2) << fare << "\n";
    
    char confirm;
    cout << "\nConfirm booking? (Y/N): ";
    cin >> confirm;
    
    if (confirm != 'Y' && confirm != 'y') {
        cout << "Booking cancelled.\n";
        return;
    }
    
    Booking newBooking;
    newBooking.bookingId = generateBookingId();
    newBooking.passengerId = currentPassengerId;
    newBooking.flightNo = flightChoice;
    
    time_t now = time(0);
    tm* currentTime = localtime(&now);
    newBooking.bookingDate.day = currentTime->tm_mday;
    newBooking.bookingDate.month = currentTime->tm_mon + 1;
    newBooking.bookingDate.year = currentTime->tm_year + 1900;
    
    newBooking.travelDate = travelDate;
    newBooking.seatsBooked = seats;
    strcpy(newBooking.classType, classType.c_str());
    newBooking.farePaid = fare;
    strcpy(newBooking.status, "Confirmed");
    
    bookings[bookingCount++] = newBooking;
    
    flights[flightIndex].availableSeats -= seats;
    flights[flightIndex].timesBooked++;
    flights[flightIndex].totalRevenue += fare;
    
    if (flights[flightIndex].availableSeats == 0) {
        strcpy(flights[flightIndex].status, "Full");
    }
    
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == currentPassengerId) {
            passengers[i].totalBookings++;
            passengers[i].totalSpent += fare;
            break;
        }
    }
    
    cout << "\nBooking confirmed! Booking ID: " << newBooking.bookingId << "\n";
    cout << "Receipt generated successfully!\n";
}

void displayPassengerBookings() {
    cout << "\n===== YOUR BOOKINGS =====\n";
    
    bool found = false;
    
    cout << left << setw(12) << "Booking ID"
         << setw(10) << "Flight #"
         << setw(15) << "Travel Date"
         << setw(10) << "Seats"
         << setw(12) << "Class"
         << setw(12) << "Fare Paid($)"
         << setw(12) << "Status" << "\n";
    
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].passengerId == currentPassengerId) {
            found = true;
            string travelDateStr = to_string(bookings[i].travelDate.day) + "/" +
                                  to_string(bookings[i].travelDate.month) + "/" +
                                  to_string(bookings[i].travelDate.year);
            
            cout << left << setw(12) << bookings[i].bookingId
                 << setw(10) << bookings[i].flightNo
                 << setw(15) << travelDateStr
                 << setw(10) << bookings[i].seatsBooked
                 << setw(12) << bookings[i].classType
                 << setw(12) << fixed << setprecision(2) << bookings[i].farePaid
                 << setw(12) << bookings[i].status << "\n";
        }
    }
    
    if (!found) {
        cout << "No bookings found.\n";
    }
}

void cancelBooking() {
    if (currentPassengerId == -1) {
        cout << "You must login first!\n";
        return;
    }
    
    cout << "\n=== CANCEL BOOKING ===\n";
    displayPassengerBookings();
    
    bool hasBookings = false;
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].passengerId == currentPassengerId && 
            strcmp(bookings[i].status, "Confirmed") == 0) {
            hasBookings = true;
            break;
        }
    }
    
    if (!hasBookings) {
        cout << "\nYou have no active bookings to cancel.\n";
        return;
    }
    
    int bookingId;
    cout << "\nEnter Booking ID to cancel (0 to go back): ";
    cin >> bookingId;
    
    if (bookingId == 0) return;
    
    int bookingIndex = -1;
    Booking* bookingToCancel = findBookingById(bookingId, bookingIndex);
    
    if (!bookingToCancel) {
        cout << "Invalid Booking ID or booking not found!\n";
        return;
    }
    
    if (strcmp(bookingToCancel->status, "Cancelled") == 0) {
        cout << "This booking is already cancelled.\n";
        return;
    }
    
    if (strcmp(bookingToCancel->status, "Completed") == 0) {
        cout << "Cannot cancel a completed booking.\n";
        return;
    }
    
    float refundAmount = calculateRefundAmount(*bookingToCancel);
    
    cout << "\n=== CANCELLATION DETAILS ===\n";
    cout << "Booking ID: " << bookingToCancel->bookingId << "\n";
    cout << "Flight Number: " << bookingToCancel->flightNo << "\n";
    cout << "Travel Date: " << bookingToCancel->travelDate.day << "/" 
         << bookingToCancel->travelDate.month << "/" 
         << bookingToCancel->travelDate.year << "\n";
    cout << "Seats: " << bookingToCancel->seatsBooked << "\n";
    cout << "Class: " << bookingToCancel->classType << "\n";
    cout << "Original Fare: $" << fixed << setprecision(2) << bookingToCancel->farePaid << "\n";
    cout << "Refund Amount: $" << fixed << setprecision(2) << refundAmount << "\n";
    
    if (refundAmount == 0) {
        cout << "Note: No refund as cancellation is too close to travel date.\n";
    }
    
    char confirm;
    cout << "\nAre you sure you want to cancel this booking? (Y/N): ";
    cin >> confirm;
    
    if (confirm != 'Y' && confirm != 'y') {
        cout << "Cancellation cancelled.\n";
        return;
    }
    
    int flightIndex = -1;
    Flight* flight = findFlightByNumber(bookingToCancel->flightNo, flightIndex);
    
    if (flight) {
        flight->availableSeats += bookingToCancel->seatsBooked;
        if (strcmp(flight->status, "Full") == 0) {
            strcpy(flight->status, "Available");
        }
        flight->totalRevenue -= refundAmount;
        flight->timesBooked--;
    }
    
    strcpy(bookingToCancel->status, "Cancelled");
    
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == currentPassengerId) {
            passengers[i].totalSpent -= refundAmount;
            passengers[i].totalBookings--;
            break;
        }
    }
    
    cout << "\n=== CANCELLATION SUCCESSFUL ===\n";
    cout << "Booking ID " << bookingToCancel->bookingId << " has been cancelled.\n";
    
    if (refundAmount > 0) {
        cout << "Refund of $" << fixed << setprecision(2) << refundAmount 
             << " will be processed to your account.\n";
    }
}

// ========== REPORT FUNCTIONS ==========

int countBookingsByStatus(const char* status) {
    int count = 0;
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].passengerId == currentPassengerId && 
            strcmp(bookings[i].status, status) == 0) {
            count++;
        }
    }
    return count;
}

float getTotalSpentOnBookings() {
    float total = 0.0;
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].passengerId == currentPassengerId) {
            total += bookings[i].farePaid;
        }
    }
    return total;
}

void displayPassengerInfo() {
    cout << "\n=== PASSENGER INFORMATION ===\n";
    
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == currentPassengerId) {
            cout << "Passenger ID: " << passengers[i].id << "\n";
            cout << "Name: " << passengers[i].name << "\n";
            cout << "Email: " << passengers[i].email << "\n";
            cout << "Phone: " << passengers[i].phone << "\n";
            cout << "Total Bookings: " << passengers[i].totalBookings << "\n";
            cout << "Total Spent: $" << fixed << setprecision(2) << passengers[i].totalSpent << "\n";
            break;
        }
    }
    cout << "------------------------------\n";
}

void displayBookingSummary() {
    cout << "\n=== BOOKING SUMMARY ===\n";
    
    int confirmed = countBookingsByStatus("Confirmed");
    int cancelled = countBookingsByStatus("Cancelled");
    int totalBookings = confirmed + cancelled;
    float totalSpent = getTotalSpentOnBookings();
    
    cout << "Total Bookings: " << totalBookings << "\n";
    cout << "Active Bookings: " << confirmed << "\n";
    cout << "Cancelled Bookings: " << cancelled << "\n";
    cout << "Total Amount Spent: $" << fixed << setprecision(2) << totalSpent << "\n";
    cout << "------------------------------\n";
}

void displayBookingHistory() {
    cout << "\n=== BOOKING HISTORY ===\n";
    
    bool hasBookings = false;
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].passengerId == currentPassengerId) {
            hasBookings = true;
            break;
        }
    }
    
    if (!hasBookings) {
        cout << "No booking history found.\n";
        return;
    }
    
    cout << left << setw(12) << "Booking ID" 
         << setw(10) << "Flight #" 
         << setw(12) << "Book Date" 
         << setw(12) << "Travel Date" 
         << setw(8) << "Seats" 
         << setw(10) << "Class" 
         << setw(10) << "Fare($)" 
         << setw(12) << "Status" << "\n";
    
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].passengerId == currentPassengerId) {
            string bookDate = formatDate(bookings[i].bookingDate);
            string travelDate = formatDate(bookings[i].travelDate);
            
            cout << left << setw(12) << bookings[i].bookingId
                 << setw(10) << bookings[i].flightNo
                 << setw(12) << bookDate
                 << setw(12) << travelDate
                 << setw(8) << bookings[i].seatsBooked
                 << setw(10) << bookings[i].classType
                 << setw(10) << fixed << setprecision(2) << bookings[i].farePaid
                 << setw(12) << bookings[i].status << "\n";
        }
    }
    cout << "------------------------------\n";
}

void displayRecentBookings() {
    cout << "\n=== RECENT BOOKINGS ===\n";
    
    int count = 0;
    for (int i = bookingCount - 1; i >= 0 && count < 3; i--) {
        if (bookings[i].passengerId == currentPassengerId) {
            count++;
            string origin = "Unknown";
            string destination = "Unknown";
            
            for (int j = 0; j < flightCount; j++) {
                if (flights[j].flightNo == bookings[i].flightNo) {
                    origin = flights[j].origin;
                    destination = flights[j].destination;
                    break;
                }
            }
            
            string travelDate = formatDate(bookings[i].travelDate);
            
            cout << count << ". Booking #" << bookings[i].bookingId << "\n";
            cout << "   Flight: " << origin << " to " << destination << "\n";
            cout << "   Travel Date: " << travelDate << "\n";
            cout << "   Seats: " << bookings[i].seatsBooked << " (" << bookings[i].classType << ")\n";
            cout << "   Fare: $" << fixed << setprecision(2) << bookings[i].farePaid << "\n";
            cout << "   Status: " << bookings[i].status << "\n";
            cout << "   ------------------------------\n";
        }
    }
    
    if (count == 0) {
        cout << "No recent bookings found.\n";
    }
}

void generatePersonalReport() {
    if (currentPassengerId == -1) {
        cout << "You must login first!\n";
        return;
    }
    
    cout << "\n========================================\n";
    cout << "      PERSONAL BOOKING REPORT\n";
    cout << "========================================\n";
    
    displayPassengerInfo();
    displayBookingSummary();
    displayBookingHistory();
    displayRecentBookings();
    
    time_t now = time(0);
    tm* currentTime = localtime(&now);
    cout << "\nReport Generated: " 
         << formatDate({currentTime->tm_mday, currentTime->tm_mon + 1, currentTime->tm_year + 1900})
         << " at " << formatTime({currentTime->tm_hour, currentTime->tm_min}) << "\n";
    
    cout << "========================================\n";
    cout << "          END OF REPORT\n";
    cout << "========================================\n";
}

// ========== PROFILE UPDATE FUNCTIONS ==========

void displayCurrentProfile() {
    cout << "\n=== YOUR CURRENT PROFILE ===\n";
    
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == currentPassengerId) {
            cout << "1. Name: " << passengers[i].name << "\n";
            cout << "2. Email: " << passengers[i].email << "\n";
            cout << "3. Phone: " << passengers[i].phone << "\n";
            cout << "4. Password: ********\n";
            cout << "------------------------------\n";
            cout << "Total Bookings: " << passengers[i].totalBookings << "\n";
            cout << "Total Spent: $" << fixed << setprecision(2) << passengers[i].totalSpent << "\n";
            break;
        }
    }
}

void updateName() {
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == currentPassengerId) {
            char newName[50];
            cout << "\nCurrent Name: " << passengers[i].name << "\n";
            cout << "Enter new name: ";
            cin.ignore();
            cin.getline(newName, 50);
            
            if (strlen(newName) > 0) {
                strcpy(passengers[i].name, newName);
                cout << "Name updated successfully!\n";
            } else {
                cout << "Name cannot be empty!\n";
            }
            return;
        }
    }
}

void updateEmail() {
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == currentPassengerId) {
            char newEmail[50];
            cout << "\nCurrent Email: " << passengers[i].email << "\n";
            
            while (true) {
                cout << "Enter new email: ";
                cin.ignore();
                cin.getline(newEmail, 50);
                
                if (strlen(newEmail) == 0) {
                    cout << "Email cannot be empty!\n";
                    continue;
                }
                
                if (isValidEmail(newEmail)) {
                    bool emailExists = false;
                    for (int j = 0; j < passengerCount; j++) {
                        if (j != i && strcmp(passengers[j].email, newEmail) == 0) {
                            emailExists = true;
                            break;
                        }
                    }
                    
                    if (emailExists) {
                        cout << "This email is already registered!\n";
                    } else {
                        strcpy(passengers[i].email, newEmail);
                        cout << "Email updated successfully!\n";
                        break;
                    }
                } else {
                    cout << "Invalid email format! Use format: user@domain.com\n";
                }
            }
            return;
        }
    }
}

void updatePhone() {
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == currentPassengerId) {
            char newPhone[15];
            cout << "\nCurrent Phone: " << passengers[i].phone << "\n";
            cout << "Enter new phone number: ";
            cin.ignore();
            cin.getline(newPhone, 15);
            
            if (strlen(newPhone) > 0) {
                strcpy(passengers[i].phone, newPhone);
                cout << "Phone number updated successfully!\n";
            } else {
                cout << "Phone number cannot be empty!\n";
            }
            return;
        }
    }
}

void updatePassword() {
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == currentPassengerId) {
            char currentPass[30];
            char newPass[30];
            char confirmPass[30];
            
            cout << "\n=== CHANGE PASSWORD ===\n";
            cout << "Enter current password: ";
            cin.ignore();
            cin.getline(currentPass, 30);
            
            if (strcmp(passengers[i].password, currentPass) != 0) {
                cout << "Current password is incorrect!\n";
                return;
            }
            
            while (true) {
                cout << "Enter new password (min 6 characters): ";
                cin.getline(newPass, 30);
                
                if (strlen(newPass) < 6) {
                    cout << "Password must be at least 6 characters!\n";
                    continue;
                }
                
                cout << "Confirm new password: ";
                cin.getline(confirmPass, 30);
                
                if (strcmp(newPass, confirmPass) != 0) {
                    cout << "Passwords do not match!\n";
                } else {
                    strcpy(passengers[i].password, newPass);
                    cout << "Password changed successfully!\n";
                    break;
                }
            }
            return;
        }
    }
}

void updateProfile() {
    if (currentPassengerId == -1) {
        cout << "You must login first!\n";
        return;
    }
    
    int choice;
    bool updating = true;
    
    while (updating) {
        cout << "\n========================================\n";
        cout << "          UPDATE PROFILE\n";
        cout << "========================================\n";
        
        displayCurrentProfile();
        
        cout << "\nWhat would you like to update?\n";
        cout << "1. Update Name\n";
        cout << "2. Update Email\n";
        cout << "3. Update Phone Number\n";
        cout << "4. Change Password\n";
        cout << "5. Back to Main Menu\n";
        cout << "----------------------------------------\n";
        cout << "Enter your choice (1-5): ";
        
        cin >> choice;
        
        switch(choice) {
            case 1: updateName(); break;
            case 2: updateEmail(); break;
            case 3: updatePhone(); break;
            case 4: updatePassword(); break;
            case 5: 
                cout << "Returning to main menu...\n";
                updating = false;
                break;
            default:
                cout << "Invalid choice! Please enter 1-5.\n";
        }
        
        if (choice >= 1 && choice <= 4) {
            char more;
            cout << "\nDo you want to update something else? (Y/N): ";
            cin >> more;
            
            if (more != 'Y' && more != 'y') {
                updating = false;
                cout << "Profile update completed.\n";
            }
        }
    }
}

// ========== ADMIN FUNCTIONS ==========

void adminMenu() {
    int choice;
    bool loggedIn = true;
    
    while (loggedIn) {
        cout << "\n===== ADMIN PANEL =====\n";
        cout << "1. Add Flight\n";
        cout << "2. View All Flights\n";
        cout << "3. Update Flight\n";
        cout << "4. Delete Flight\n";
        cout << "5. View All Bookings\n";
        cout << "6. View Passenger Details\n";
        cout << "7. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;
        
        switch(choice) {
            case 1:
                addFlight(flights, flightCount);
                break;
            case 2:
                viewFlights(flights, flightCount);
                break;
            case 3:
                updateFlight(flights, flightCount);
                break;
            case 4:
                deleteFlight(flights, flightCount);
                break;
            case 5:
                viewAllBookings();
                break;
            case 6:
                cout << "\n=== PASSENGER DETAILS ===\n";
                for (int i = 0; i < passengerCount; i++) {
                    cout << "ID: " << passengers[i].id 
                         << " | Name: " << passengers[i].name
                         << " | Email: " << passengers[i].email
                         << " | Bookings: " << passengers[i].totalBookings
                         << " | Spent: $" << passengers[i].totalSpent << endl;
                }
                break;
            case 7:
                cout << "Logging out...\n";
                loggedIn = false;
                currentAdminId = -1;
                break;
            default:
                cout << "Invalid choice!\n";
        }
    }
}

void adminLoginPanel() {
    cout << "\n=== ADMIN LOGIN ===\n";
    
    string username, password;
    cout << "Enter Admin Username: ";
    cin >> username;
    cout << "Enter Admin Password: ";
    cin >> password;
    
    if (username == "admin" && password == "admin123") {
        cout << "\nLogin successful! Welcome Admin!\n";
        currentAdminId = 1;
        adminMenu();
    } else {
        cout << "Access Denied! Invalid credentials.\n";
    }
}

void addFlight(Flight flights[], int &flightCount) {
    if (flightCount >= MAX_FLIGHTS) {
        cout << "Cannot add more flights. Maximum capacity reached.\n";
        return;
    }
    
    int flightNo;
    do {
        cout << "Enter Flight Number (positive integer): ";
        cin >> flightNo;
        if (flightNo <= 0) cout << "Invalid flight number!\n";
    } while (flightNo <= 0);
    
    flights[flightCount].flightNo = flightNo;
    cin.ignore();
    
    cout << "Enter Origin: ";
    cin.getline(flights[flightCount].origin, 50);
    
    cout << "Enter Destination: ";
    cin.getline(flights[flightCount].destination, 50);
    
    int day, month, year;
    do {
        cout << "Enter Departure Date (dd mm yyyy): ";
        cin >> day >> month >> year;
        if (!isValidDate(day, month, year)) cout << "Invalid date! Try again.\n";
    } while (!isValidDate(day, month, year));
    flights[flightCount].departureDate = {day, month, year};
    
    int hour, minute;
    do {
        cout << "Enter Departure Time (hh mm, 0-23 & 0-59): ";
        cin >> hour >> minute;
        if (!isValidTime(hour, minute)) cout << "Invalid time! Try again.\n";
    } while (!isValidTime(hour, minute));
    flights[flightCount].departureTime = {hour, minute};
    
    do {
        cout << "Enter Arrival Date (dd mm yyyy): ";
        cin >> day >> month >> year;
        if (!isValidDate(day, month, year)) cout << "Invalid date! Try again.\n";
    } while (!isValidDate(day, month, year));
    flights[flightCount].arrivalDate = {day, month, year};
    
    do {
        cout << "Enter Arrival Time (hh mm, 0-23 & 0-59): ";
        cin >> hour >> minute;
        if (!isValidTime(hour, minute)) cout << "Invalid time! Try again.\n";
    } while (!isValidTime(hour, minute));
    flights[flightCount].arrivalTime = {hour, minute};
    
    do {
        cout << "Enter Economy Seats: ";
        cin >> flights[flightCount].economySeats;
    } while (flights[flightCount].economySeats < 0);
    
    do {
        cout << "Enter Business Seats: ";
        cin >> flights[flightCount].businessSeats;
    } while (flights[flightCount].businessSeats < 0);
    
    do {
        cout << "Enter First Class Seats: ";
        cin >> flights[flightCount].firstClassSeats;
    } while (flights[flightCount].firstClassSeats < 0);
    
    do {
        cout << "Enter Economy Fare: ";
        cin >> flights[flightCount].economyFare;
    } while (flights[flightCount].economyFare < 0);
    
    do {
        cout << "Enter Business Fare: ";
        cin >> flights[flightCount].businessFare;
    } while (flights[flightCount].businessFare < 0);
    
    do {
        cout << "Enter First Class Fare: ";
        cin >> flights[flightCount].firstClassFare;
    } while (flights[flightCount].firstClassFare < 0);
    
    flights[flightCount].totalSeats = flights[flightCount].economySeats +
                                      flights[flightCount].businessSeats +
                                      flights[flightCount].firstClassSeats;
    flights[flightCount].availableSeats = flights[flightCount].totalSeats;
    
    do {
        cout << "Enter Distance (positive): ";
        cin >> flights[flightCount].distance;
        if (flights[flightCount].distance < 0) cout << "Invalid distance!\n";
    } while (flights[flightCount].distance < 0);
    
    flights[flightCount].baseFare = flights[flightCount].economyFare;
    strcpy(flights[flightCount].status, "Available");
    flights[flightCount].timesBooked = 0;
    flights[flightCount].totalRevenue = 0.0;
    
    flightCount++;
    cout << "\nFlight added successfully!\n";
}

void updateFlight(Flight flights[], int flightCount) {
    if (flightCount == 0) {
        cout << "No flights available to update.\n";
        return;
    }
    
    int flightNo;
    cout << "Enter Flight Number to update: ";
    cin >> flightNo;
    
    int index = -1;
    for (int i = 0; i < flightCount; i++) {
        if (flights[i].flightNo == flightNo) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        cout << "Flight not found.\n";
        return;
    }
    
    cout << "\nUpdating Flight #" << flights[index].flightNo << ":\n";
    cin.ignore();
    
    cout << "Enter new Origin (current: " << flights[index].origin << "): ";
    cin.getline(flights[index].origin, 50);
    
    cout << "Enter new Destination (current: " << flights[index].destination << "): ";
    cin.getline(flights[index].destination, 50);
    
    int day, month, year;
    do {
        cout << "Enter new Departure Date (dd mm yyyy): ";
        cin >> day >> month >> year;
        if (!isValidDate(day, month, year)) cout << "Invalid date! Try again.\n";
    } while (!isValidDate(day, month, year));
    flights[index].departureDate = {day, month, year};
    
    int hour, minute;
    do {
        cout << "Enter new Departure Time (hh mm): ";
        cin >> hour >> minute;
        if (!isValidTime(hour, minute)) cout << "Invalid time! Try again.\n";
    } while (!isValidTime(hour, minute));
    flights[index].departureTime = {hour, minute};
    
    do {
        cout << "Enter new Arrival Date (dd mm yyyy): ";
        cin >> day >> month >> year;
        if (!isValidDate(day, month, year)) cout << "Invalid date! Try again.\n";
    } while (!isValidDate(day, month, year));
    flights[index].arrivalDate = {day, month, year};
    
    do {
        cout << "Enter new Arrival Time (hh mm): ";
        cin >> hour >> minute;
        if (!isValidTime(hour, minute)) cout << "Invalid time! Try again.\n";
    } while (!isValidTime(hour, minute));
    flights[index].arrivalTime = {hour, minute};
    
    cout << "Flight updated successfully!\n";
}

void deleteFlight(Flight flights[], int &flightCount) {
    if (flightCount == 0) {
        cout << "No flights available to delete.\n";
        return;
    }
    
    int flightNo;
    cout << "Enter Flight Number to delete: ";
    cin >> flightNo;
    
    int index = -1;
    for (int i = 0; i < flightCount; i++) {
        if (flights[i].flightNo == flightNo) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        cout << "Flight not found.\n";
        return;
    }
    
    for (int i = index; i < flightCount - 1; i++) {
        flights[i] = flights[i + 1];
    }
    
    flightCount--;
    cout << "Flight #" << flightNo << " deleted successfully!\n";
}

// ========== PASSENGER FUNCTIONS ==========

void showPassengerMenu() {
    int choice;
    bool loggedIn = true;
    
    while (loggedIn) {
        cout << "\n=== PASSENGER MENU ===\n";
        cout << "1. View Available Flights\n";
        cout << "2. Book a Flight\n";
        cout << "3. Cancel Booking\n";
        cout << "4. Generate Personal Report\n";
        cout << "5. Update Profile\n";
        cout << "6. Logout\n";
        cout << "\nEnter your choice (1-6): ";
        
        cin >> choice;
        
        switch(choice) {
            case 1: viewAvailableFlights(); break;
            case 2: bookFlight(); break;
            case 3: cancelBooking(); break;
            case 4: generatePersonalReport(); break;
            case 5: updateProfile(); break;
            case 6: 
                cout << "Logged out successfully!\n";
                loggedIn = false;
                currentPassengerId = -1;
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
}

void passengerLogin() {
    cout << "\n=== PASSENGER LOGIN ===\n";
    
    int id;
    char password[30];
    
    cout << "Enter Passenger ID: ";
    cin >> id;
    cin.ignore();
    
    cout << "Enter Password: ";
    cin.getline(password, 30);
    
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == id && strcmp(passengers[i].password, password) == 0) {
            currentPassengerId = id;
            cout << "\nLogin successful! Welcome " << passengers[i].name << "!\n";
            showPassengerMenu();
            return;
        }
    }
    
    cout << "Invalid credentials! Please try again.\n";
}

void PassengerRegistration() {
    if (passengerCount >= MAX_PASSENGERS) {
        cout << "Maximum passenger limit reached!\n";
        return;
    }

    Passenger newPassenger;
    
    cout << "\n=== PASSENGER REGISTRATION ===\n";
    
    newPassenger.id = 1000 + passengerCount + 1;
    cout << "Your Passenger ID: " << newPassenger.id << " (Remember this for login)\n";
    
    cout << "Enter your name: ";
    cin.ignore();
    cin.getline(newPassenger.name, 50);
    
    cout << "Enter your Password: ";
    cin.getline(newPassenger.password, 30);
    
    char email[50];
    while (true) {
        cout << "Enter E-mail: ";
        cin.getline(email, 50);
        if (isValidEmail(email)) {
            strcpy(newPassenger.email, email);
            break;
        } else {
            cout << "Invalid email format! Please enter a valid email (e.g., user@domain.com)\n";
        }
    }
    
    cout << "Enter Phone Number: ";
    cin.getline(newPassenger.phone, 15);
    
    newPassenger.totalBookings = 0;
    newPassenger.totalSpent = 0.0;
    
    passengers[passengerCount] = newPassenger;
    passengerCount++;
    
    cout << "\nRegistration successful!\n";
    cout << "\n=== REGISTRATION DETAILS ===\n";
    cout << "Passenger ID: " << newPassenger.id << "\n";
    cout << "Name: " << newPassenger.name << "\n";
    cout << "Email: " << newPassenger.email << "\n";
    cout << "Phone: " << newPassenger.phone << "\n";
    cout << "\nIMPORTANT: Save your Passenger ID for login: " << newPassenger.id << "\n";
}

// ========== MAIN MENU ==========

void mainMenu() {
    int choice;
    
    do {
        cout << "\n=== AIRLINE RESERVATION SYSTEM ===\n";
        cout << "1. Passenger Registration\n";
        cout << "2. Passenger Login\n";
        cout << "3. Admin Login\n";
        cout << "4. Exit System\n";
        cout << "\nEnter your choice (1-4): ";
        
        cin >> choice;
        
        switch(choice) {
            case 1: PassengerRegistration(); break;
            case 2: passengerLogin(); break;
            case 3: adminLoginPanel(); break;
            case 4: cout << "Thank you for using the system!\n"; break;
            default: cout << "Invalid choice! Please try again.\n";
        }
    } while(choice != 4);
}

// ========== MAIN FUNCTION ==========

int main() {
    // Add sample data
    passengerCount = 1;
    passengers[0].id = 1001;
    strcpy(passengers[0].name, "John Doe");
    strcpy(passengers[0].password, "password123");
    strcpy(passengers[0].email, "john@example.com");
    strcpy(passengers[0].phone, "1234567890");
    passengers[0].totalBookings = 0;
    passengers[0].totalSpent = 0.0;
    
    flightCount = 2;
    // Flight 1
    flights[0].flightNo = 101;
    strcpy(flights[0].destination, "New York");
    strcpy(flights[0].origin, "London");
    flights[0].departureDate = {15, 12, 2024};
    flights[0].departureTime = {8, 0};
    flights[0].arrivalDate = {15, 12, 2024};
    flights[0].arrivalTime = {16, 30};
    flights[0].economySeats = 100;
    flights[0].businessSeats = 50;
    flights[0].firstClassSeats = 20;
    flights[0].economyFare = 300.0;
    flights[0].businessFare = 600.0;
    flights[0].firstClassFare = 1050.0;
    flights[0].totalSeats = 170;
    flights[0].availableSeats = 170;
    flights[0].baseFare = 300.0;
    flights[0].distance = 5567.0;
    strcpy(flights[0].status, "Available");
    flights[0].timesBooked = 0;
    flights[0].totalRevenue = 0.0;
    
    // Flight 2
    flights[1].flightNo = 102;
    strcpy(flights[1].destination, "Paris");
    strcpy(flights[1].origin, "Tokyo");
    flights[1].departureDate = {20, 12, 2024};
    flights[1].departureTime = {14, 30};
    flights[1].arrivalDate = {20, 12, 2024};
    flights[1].arrivalTime = {23, 45};
    flights[1].economySeats = 120;
    flights[1].businessSeats = 40;
    flights[1].firstClassSeats = 15;
    flights[1].economyFare = 400.0;
    flights[1].businessFare = 800.0;
    flights[1].firstClassFare = 1400.0;
    flights[1].totalSeats = 175;
    flights[1].availableSeats = 175;
    flights[1].baseFare = 400.0;
    flights[1].distance = 9714.0;
    strcpy(flights[1].status, "Available");
    flights[1].timesBooked = 0;
    flights[1].totalRevenue = 0.0;
    
    mainMenu();
    return 0;
}